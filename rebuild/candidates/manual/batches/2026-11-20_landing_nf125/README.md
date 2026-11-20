# 2026-11-20 landing nf125: PRIORITY slice 6, second remainder (22 rows)

Records: `naming/2026-11-20_agentNF125/` (selfcheck 0 problems). 8 entries (NF118 / NF122 leftovers): 7 done (the CTCHeroExperience family
q4486 q4762 q4764 q4765 q4766, and q5107 / q5151), 1 blocked: port (q5137: 8 of 10 wrong binders ported onto std::vector<CThing*>, spelling set at
0x006A2220 7 -> 3; left: self row 0x006A2220 + 0x006A2370, which carries a same-TU copy of it and must move with it, and 0x006A23E0, a
whole-row port out of namespace NRemap006a23e0; a naming follow-up). 31 rows not-a-binder.

## Rows (22): 3 MATCH + 19 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- CTCHeroExperience family, handed back whole (13 rows): the self rows 0x006D64A0 / 0x006D6510 / 0x006D6520 all defined one free
  `GetAbilityLevel` stand-in and are re-authored as the real members (MATCH); the SExpSpendUndo leaves 0x0056E93D / 0x0056F4BA / 0x0056F597
  move to the Ego_r spelling (all four referencing rows already bind it); side callees 0x0056FF3D, 0x006D6470, 0x006D6830, 0x006D6F60,
  0x006D8980 and (outside the entries) 0x006D6770 GetTrainableStatMaxLevel move onto PDB spellings another landed row already binds.
- 0x0070D1A0 (q5107 / q5151) re-authored on engine/CAnimSet.h + CAnimInit.h with STLport vectors.
- Coordinator decision (advisor precedent nf115 / owner delegated): 0x0070D1A0 rebinds 0x00710980 / 0x00710F70 to their STLport
  self-row spellings; 0x0070D2F0 AddAnim keeps its local `vector<T>` stand-ins, so each address stays at 2 spellings (the old `K` size-type
  stand-in drops out). Not created; 0x0070D2F0 queued.

## Headers (add-only opt-ins, default path token-identical; pin_asserts pinned 0; assert_names_check 0 mismatches)
CTCHeroExperience.h: FABLE_RETAIL_CTCHEROEXPERIENCE_STATVECTORS (layout branch), _SPENTONSTAT, _FORCEEXPERIENCE.

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0. 89 rows rechecked: 89 good / 0 bad. No-new-split gate 0. Table spelling sets
shrank at 8 addresses (0x006A2220 6 -> 2, 0x006D6510 / 0x006D6520 4 -> 1, ...). Three grew 1 -> 2 as new witness rows only:
0x0042B687 (0x0056F597 binds CLongList::push_back), 0x0077E580 and 0x007CE1F0 (0x007CF110 binds CRetailVector reserve / __copy); each
row bound the same stand-in before and after (HEAD vs landing pairs), so these are existing splits, queued.
