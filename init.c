#include <Gestalt.h>
#include <Patches.h>
#include <Resources.h>
#include <Traps.h>

#include "comm.h"

#define kIconOK -4064
#define kIconBad -4063

extern pascal void SHOWINIT(short iconId, short moveX);

extern pascal void INIT() {
	unsigned long prevAddress;
	long result;
	Handle patchHandle;
	int useMultifinder;
	Ptr patchPtr;
	if(GetOSTrapAddress(_Gestalt) == GetToolboxTrapAddress(_Unimplemented)) {
		// Gestalt not available
		goto fail;
	}
	if(Gestalt(gestaltSysArchitecture, &result) == noErr && result != gestalt68k) {
		// Not running on a 680x0 processor
		goto fail;
	}
	if(Gestalt(gestaltAUXVersion, &result) == noErr) {
		// Running in A/UX
		goto fail;
	}
	if(Gestalt(gestaltSystemVersion, &result) == noErr && result >= 0x700) {
		// try to patch SynchIdleTime on System 7
		useMultifinder = 1;
	} else {
		// always fall back to SystemTask on System 6
		useMultifinder = 0;
	}
	if(useMultifinder) {
		prevAddress = (unsigned long)GetToolboxTrapAddress(kTargetTrap);
		if(prevAddress == (unsigned long)GetToolboxTrapAddress(_Unimplemented)) {
			// SynchIdleTime not present. Patch _SystemTask. (This shouldn't happen)
			prevAddress = (unsigned long)GetToolboxTrapAddress(_SystemTask);
			useMultifinder = 0;
		}
	} else {
		prevAddress = (unsigned long)GetToolboxTrapAddress(_SystemTask);
	}
	if(prevAddress == (unsigned long)GetToolboxTrapAddress(_Unimplemented)) {
		// this should not happen
		goto fail;
	}
	patchHandle = Get1Resource(kSleepyMacPatchResourceType, kSleepyMacPatchResourceID);
	if(patchHandle == NULL) {
		// Missing a required resource
		goto fail;
	}
	patchPtr = NewPtrSys(GetHandleSize(patchHandle));
	if(patchPtr == NULL) {
		// Out of memory
		ReleaseResource(patchHandle);
		goto fail;
	}
	BlockMove(*patchHandle, patchPtr, GetHandleSize(patchHandle));
	ReleaseResource(patchHandle);
	// Record address of trap we are patching
	*(unsigned long*)(patchPtr + 0x1A) = prevAddress;
	// Record address of our memory block
	// (so that our Gestalt selector will return it)
	*(unsigned long*)(patchPtr + 0x2A) = (unsigned long)patchPtr;
	if(NewGestalt(kSleepyMacGestalt, (void*)(patchPtr+0x22)) != noErr) {
		// Unable to install Gestalt selector
		DisposePtr(patchPtr);
		goto fail;
	}
	// Patch :)
	SetToolboxTrapAddress((void*)patchPtr, useMultifinder ? kTargetTrap : _SystemTask);
	SHOWINIT(kIconOK, -1);
	return;
fail:
	SHOWINIT(kIconBad, -1);
}
