# 2026-11-20 landing nf113: PRIORITY table slice 4 (32 rows)

Records: `naming/2026-11-20_agentNF113/` (selfcheck 0 problems). 32 queue lines over 30 addresses: 18 done, 3 done-already (q4775 q6172 q6392),
1 rejected (q5230: the 44-byte body releases, not destroys; NF75's ReleaseData<CScrollableList> stands), 3 blocked: family (q4272 self staged,
waits for 0x007D62D0; q4716 / q4950: binders 0x006D0020 Died / 0x006CFF10 Unmarry land on the claim, the self rename of 0x0057BC6E waits for
0x006D0180 Divorce), 4 deferred (q4187 q4188 q4189 q4192: the basic_string family, now NF116's), 3 open (q4468: 17 binders reach 0x005D8E50
through row-local stand-ins and need engine/CTCInventoryItem.h; q4719: waits for 0x0091DB50, whose CTCTargeting stand-in is really
CTCPerceiveThing; q5201: a fold is plausible, a third unlanded caller 0x00677F05).
- Claims wrong as posed (REJECTED.tsv): q4773 list::clear is FableWin's Dinkumware member; STLport `_List_base<CSliderAnimator>::clear` applied
  (what both header-built callers bind); q6172's `@4@` back-reference is a typo (the `@5@` spelling is landed everywhere); q6390
  GetAnimationTransitionInTime is in no name source.

## Rows (32): 7 MATCH + 25 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
20 self renames (3 Leaf-line only) and 25 callee respellings. 0x006556D3 was mislabelled Destroy_range: it is STLport
`fill<CIntelligentPointer<CParticleEmitter>*>`, re-authored as that on `/Gr` (as the landed 0x00BBD380 fill; carried into CompilerFlags).
0x005792DA ported to `_List_base<CSliderAnimator>::~_List_base`; all 3 refs of 0x005B4EBA CalculateArmorValues bind the PDB spelling.
- Header: engine/CTCWeapon.h `FABLE_RETAIL_CTCWEAPON_ISPROJECTILEWEAPONLOADED` (declaration only, pin_asserts 0, assert_names_check 0). It sits in
  the generated default branch: a header regeneration must keep it.

## Checks
bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0. Includers + renamed + exe-scan referencing rows: 212 good / 0 bad. No-new-split
gate (before set compiled from HEAD at HEAD flags): 0 created.
