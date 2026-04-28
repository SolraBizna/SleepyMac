#include <Controls.h>
#include <Dialogs.h>
#include <Gestalt.h>
#include <Processes.h>
#include <Resources.h>
#include <Sound.h>
#include <TextUtils.h>
#include <Traps.h>

#include "comm.h"

#define kOffButton 1
#define kOnButton 2

#define kAlertID -4063
#define kFailMsgResourceID -4063
#define kFailMsgNoGestalt 1
#define kFailMsgNo68k 2
#define kFailMsgAUX 3
#define kFailMsgNoTrap 4
#define kFailMsgNotInstalled 5

static void alertMessage(short stringId) {
	Str255 str;
	GetIndString(str, kFailMsgResourceID, stringId);
	ParamText(str, "\p", "\p", "\p");
	Alert(kAlertID, NULL);
}

static Handle getPatch() {
	return Get1Resource(kSleepyMacPatchResourceType, kSleepyMacPatchResourceID);
}

static void updateBoxes(DialogPtr cpDialog, short numItems, short flagValue) {
	Handle h;
	GetDialogItem(cpDialog, numItems+kOffButton, NULL, &h, NULL);
	SetControlValue((ControlHandle)h, flagValue == kSleepyMacDisabled ? 1 : 0);
	GetDialogItem(cpDialog, numItems+kOnButton, NULL, &h, NULL);
	SetControlValue((ControlHandle)h, flagValue == kSleepyMacEnabled ? 1 : 0);
}

static long initialize(long cdevValue, DialogPtr cpDialog, short numItems) {
	long result;
	if(getPatch() == NULL) {
		return cdevResErr; // will display a "missing resource" error
	}
	if(GetOSTrapAddress(_Gestalt) == GetToolboxTrapAddress(_Unimplemented)) {
		// Gestalt not available
		alertMessage(kFailMsgNoGestalt);
		return 0;
	}
	if(Gestalt(gestaltSysArchitecture, &result) == noErr && result != gestalt68k) {
		// Not running on a 680x0 processor
		alertMessage(kFailMsgNo68k);
		return cdevGenErr;
	}
	if(Gestalt(gestaltAUXVersion, &result) == noErr) {
		// Running in A/UX
		alertMessage(kFailMsgAUX);
		return cdevGenErr;
	}
	/*
	// This check not needed anymore.
	if(GetToolboxTrapAddress(kTargetTrap) == GetToolboxTrapAddress(_Unimplemented)) {
		// SynchIdleTime not present, can't patch it
		alertMessage(kFailMsgNoTrap);
		return cdevGenErr;
	}
	*/
	if(GetSleepyMacEnabledFlag() == NULL) {
		// The INIT didn't run (and it's too late to do its work ourselves)
		alertMessage(kFailMsgNotInstalled);
		return cdevGenErr;
	}
	updateBoxes(cpDialog, numItems, **(short**)getPatch());
	return cdevValue;
}

static void maybeFlush() {
	if(GetOSTrapAddress(_HWPriv) != GetToolboxTrapAddress(_Unimplemented)) {
		FlushInstructionCache();
	}
}

static void setFlag(DialogPtr cpDialog, short numItems, short newValue) {
	Handle h = getPatch();
	short* p;
	if(h) {
		**(short**)h = newValue;
		ChangedResource(h);
	}
	p = GetSleepyMacEnabledFlag();
	if(p) {
		*p = newValue;
		maybeFlush();
	}
	updateBoxes(cpDialog, numItems, newValue);
}

static void itemWasHit(DialogPtr cpDialog, short item, short numItems) {
	if(item - numItems == kOffButton) {
		// turn off
		setFlag(cpDialog, numItems, kSleepyMacDisabled);
	} else if(item - numItems == kOnButton) {
		// turn on
		setFlag(cpDialog, numItems, kSleepyMacEnabled);
	}
}

extern pascal long CDEV(
	short message, // message type
	short item, // number of the item that was hit, if relevant
	short numItems, // number of items preceding us in the dialog
	short panelId, // resource ID of the control panel (????)
	EventRecord* evt, // event that triggered this message, if any
	long cdevValue, // (mostly) the value returned from the last CDEV call
	DialogPtr cpDialog // the control panel dialog
) {
	(void)evt;
	(void)panelId;
	switch(message) {
		case initDev:
			// Our cdev is being initialized
			return initialize(cdevValue, cpDialog, numItems);
		case hitDev:
			// A dialog item was clicked
			itemWasHit(cpDialog, item, numItems);
			break;
		case closeDev:
			// Our cdev is being closed
			break;
		case nulDev:
			// called once-ish every time through the event loop
			break;
		case updateDev:
			// "update event"
			break;
		case activDev:
			// "activate event"
			updateBoxes(cpDialog, numItems, **(short**)getPatch());
			break;
		case deactivDev:
			// "deactivate event"
			break;
		case keyEvtDev:
			// "key-down or auto-key event"
			break;
		case macDev:
			// determine if we can be run on this machine
			return 1; // we will try! ...so we can give a nice error if we aren't working
		case undoDev:
		case cutDev:
		case copyDev:
		case pasteDev:
		case clearDev:
			// standard Edit menu stuff
			break;
	}
	// carry the passed value through, if we don't return something else specific
	return cdevValue;
}