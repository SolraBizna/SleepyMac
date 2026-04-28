#!/bin/sh
# Run this script from MacRelix before committing, to regenerate
# the MacBinary files.
set -ev
macbin encode "SleepyMac.rsed" "SleepyMac.rsed.bin"
macbin encode "SleepyMac Read Me" "SleepyMac Read Me.bin"
