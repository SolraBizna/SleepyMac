#ifndef COMMH
#define COMMH

#define kSleepyMacGestalt 'ßlì'
#define kSleepyMacPatchResourceType 'PTCH'
#define kSleepyMacPatchResourceID -4064
#define kTargetTrap 0xABF7 // SynchIdleTime
#define kSleepyMacDisabled 0x6016 // BRA.S +$18
#define kSleepyMacEnabled 0x4E71 // NOP

extern short* GetSleepyMacEnabledFlag();

#endif
