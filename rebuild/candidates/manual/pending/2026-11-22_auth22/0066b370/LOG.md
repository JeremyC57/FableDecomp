# 0x0066B370 LOG (AUTH22)
0. r0 (recorded draft): SRC_FAIL on today's headers (not a grade).
1. r1 = r0 + re-ported CTCSoundPlayer opt-in, CSymbolMap macro renamed: DIFFER(1805v1808), 278 lines. Reproduces the record.
2. r2 = Ego_d order of index_to_use / best_strength: identical bytes.
3. r3 = Ego_d locals in the anim loop (panim_set, anim_indeces, c1, strength): DIFFER(1836v1808) but loop now retail's shape.
4. r4 = unsigned loop bound: jae as retail. 1836.
5. r5 = whole function on Ego_d names and blocks: identical to r4.
6. r6a FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE: no change. 7. r6b ..._RELEASE_SELFDELETING_POINTEE: 1785 (drops the Delete; wrong).
8. r7 SELFDELETING + CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22: no change (1836). Release text is not the lever.
9. r8 = r5 + visible GetDef / GetSubDef bodies: DIFFER(1798v1808), masked 19 lines.
10. r9 = GetParentThing() + implicit CDefString conversion: DIFFER(1806v1808), 42 lines, registers = retail.
11. r10 = inline StopAllCriteriaContainingString forwarder: RELOCATION_MATCH.
12. r11 = CThing virtuals GetThingStandingOn / GetFloorMaterial instead of the local view: RELOCATION_MATCH.
13. r12 = shared headers + PDB accessors + bitfields: DIFFER(1844v1808) (frame 0x7C: two stored float temporaries at the event query).
14. r13 = r12 with the two percentages as member reads, CSmallVector global: RELOCATION_MATCH. (1 SRC_FAIL compile before it.)
15. final-a = + operator delete[] declaration (call target fix found by cc.py): RELOCATION_MATCH.
16. final = new shared headers CSmallVector / NAnimationEvents_CEvent / NAnimationEvents_CManager: RELOCATION_MATCH; strict vland dry-run WINS 1, test PASS.
Not grades: mini TUs (mini/), VC10 /Od line sizes (r1 / r11 / r12), cc.py, vland dry-runs.
17. final + FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD (for the GetDef pick-any copies): RELOCATION_MATCH; strict vland dry-run WINS 1; gobj on the five pick-any copies: 4 RELOCATION_MATCH, GetSubDef<CEntitySoundDef> 182v162 (owner in a size TU).
