#include <Gestalt.h>
#include <LowMem.h>
#include <Patches.h>
#include <Traps.h>

#include "comm.h"

extern short* GetSleepyMacEnabledFlag() {
	short* ret;
	if(Gestalt(kSleepyMacGestalt, (long*)&ret) != noErr) {
		// Selector (and therefore patch) not installed
		return NULL;
	}
	return ret;
}

