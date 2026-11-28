# 0x008082C0 std::__adjust_heap

Retail Fable.exe 0x008082C0, 317 bytes (the UNMATCHED estimate was 432; the body ends at the first int3 pair). `__fastcall`, `ret 40h`.

PDB spelling (Dinkumware), both tables: `??$_Adjust_heap@PAU?$pair@VC3DVector@@J@std@@HU12@VCCompareTargetedPositionsForFirstPersonTargeting@?A0x58ed6966@@@std@@YIX...` Ego_r 0x00801CA0, FableWin 0x024E24A0. The bytes are STLport 4.6 `__adjust_heap`. The catalog reconstruction is spelled `std::__adjust_heap`. Manifest label already names the Dinkumware `_Adjust_heap` with `_anon_58ED6966`. See NAME_FIXES.tsv.

The matching spelling is the landed sibling model (0x00807620 / 0x00808AA0): scoring bodies visible via `FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES`, and `operator()` calling `GetCompareValueForPoint` with unparenthesised named values. The October draft without the bodies was DIFFER(317v317) on the comparator-copy schedule.
