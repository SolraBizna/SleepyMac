SleepyMac is a simple control panel that reduces the energy consumption of your 680x0-based Macintosh.

## Installation

To install, move SleepyMac into your System Folder and restart your Macintosh.

## Usage

You can turn SleepyMac on and off using the Control Panel. Your choice will take effect immediately, and will remain in effect even after a restart. (Bear in mind that leaving the Control Panel open wastes a shocking amount of CPU power.)

## Benefits

Macs using the original 68000 processor will not receive any benefit. Macs with *any* other 680x0 processor, including the 68HC000, 68020, 68030, and 68040, should see a slight reduction in power consumption depending on what applications are running. Emulated Macintoshes running in QEMU will see an enormous reduction in host CPU usage during idle time.

## Performance

System 7 and later: SleepyMac should have no noticeable performance impact while active.
System 6 and earlier: You may find that you get poor performance in some games. Temporarily disabling SleepyMac while playing should resolve this.

## System Software

SleepyMac is designed to work with Macintosh System 7.0 and later. It should also function on System 6, with or without MultiFinder, but this is untested.

## Memory

SleepyMac consumes only 58 *bytes* of memory in normal operation.

## Compatibility

SleepyMac should be compatible with all Macintosh software.

SleepyMac is *not* compatible with PowerPC processors, whether shipped with your Macintosh or installed via an upgrade card. SleepyMac is also *not* compatible with A/UX. In either case, you would have received no benefit from SleepyMac anyway.

## Acknowledgements

Thanks to the denizens of the #mac68k IRC channel on irc.libera.chat, who were enormously helpful during the creation of SleepyMac. And of couse, thanks to Mark Cave-Ayland, whose drunken New Year's Resolution brought working Macintosh Quadra 800 emulation in QEMU.

## Version History

- 1.0.0: Original version.
- 2.0.0: Switched to being a control panel instead of an application.
- 2.0.1: Support 68000 processors at least well enough to run on.
- 2.0.2: Fix crash on systems without `HWPriv` trap.
- 2.0.3: Better System 6 support.

## Legalese

SleepyMac is copyright ©2023-2024 Solra Bizna.
