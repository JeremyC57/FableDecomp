# Chunk Q8 (parked near misses, Opus 5.5)

3 of 40 rows matched, and all three landed in landing cm: 0x006A4FB0, 0x0095D0E0 and 0x009F5690. The other 37 are in differ/, each with
its residue on line 2. The closest are:
- **0x00989830 InitialiseLight (98v99).** VC7.1 reuses one `lea [ebx-1]` where retail computes it twice.
- **0x00ADD690 FastBuildFromPoints (646v648).** The remainder loop counts down in the wrong register.
- **0x006D8980 UpgradeTrainableStat (180v183).** Retail keeps the Abilities start pointer in ebx.
- **0x00918A40 GazeAtWork::IsRunnable (114v116).** One mov / pop ordering differs.
- **0x0093D270 (305v295).** Waits on a decision about the private ProximityForNavigation member of CCombatManager.

For 8 rows, Q8's picks pointed at the old .test.cpp instead of the draft. The picker is fixed for Q9
(scratch regrade/pickQ.py: source drafts only).

NOTES.md lists the header debt behind the 30 rows that were only regraded.
