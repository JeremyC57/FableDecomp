# 2026-11-11 landing ch -- family FAM04: 279 landed rows respelled to the virtual names FAM03 proved

Source: `naming/2026-11-11_agentFAM04/` (STATUS, NOTES, LESSONS, RENAMES, NAME_FIXES, LEFT, hdr README).

**Verification**
- vland --recheck-landed ran in three parts: all 279 grade MATCH (217) or RELOCATION_MATCH (62).
- Running every test exe found its catalog PassPattern in 279 of 279.
- Bindings were re-derived (-17 / +47 lines). bindings_multi_check is at its NullCoord baseline.
- Before applying anything, the agent re-verified each proven name (slot, name set, `ret N`): 443 of 445 hold.

**What was renamed**
- By class: NUISystem 74, CTCPhysics* 52, the CThing family 50, NCameraModes 34, NCreatureMode with CTCCreatureModeManager 33, other CTC components 31.
- 3 tests changed. 0x0052E890's catalog test used inline asm and is replaced by a self-contained test with the same PassPattern. 0x0041D1C8 and 0x004C7460 gain a portable prelude.

**Headers**
- 35 headers carry `#ifdef` opt-ins (the _VSLOTS / _VSLOTS2 families and others listed in hdr/README).
- hdrcheck2 reports default=same, and every header is a line-superset of the repo copy.

**Known debt and warnings**
- 8 rows keep local class models that were already in their landed sources: 0x0041D1C8, 0x0041D8A8, 0x00541020, 0x00644FE5, 0x00815070, 0x008DD6E0, 0x008DF9C0, 0x008E8830. vland warns SHARED_HEADER_AVAILABLE on them. No new local models were introduced.
- vland's RELOC_NAME warnings are catalog labels lagging the STLport spellings (list find/erase), plus the settled DELTA_NOTIONAL_ZERO case.
- Unproven details: the access level of DummyRHSet (assumed protected static). 0x00739E50 reaches the static object through the landed name s_NoGroupAnimFilter_01382438.

**Left over and queued**
- 166 rows are left (LEFT.tsv), 140 of them not attempted for time.
- Name fixes are queued as land1011ch: 11 entries, plus the rejection of the land1011cf claim for 0x00AF1200 (its body ends `ret 18h`, so it cannot be a destructor).
- Labels: ghidra_out/labels_landing_2026-11-11ch.tsv.
