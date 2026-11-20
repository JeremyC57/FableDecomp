# 2026-11-20 landing nf137: PRIORITY slice 9 fourth pass (10 rows)

Records: `naming/2026-11-20_agentNF137/` (selfcheck 0 problems). 5 entries: q5021, q4993, q5030 done; q4994 / q5140 open (binders
0x00913F30 / 0x00964CA0 left, then the self row 0x008FD000): NF141 has them.

## Rows (10): 10 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- Onto shared headers: 0x00956FE0, 0x009551A0, 0x009863E0, 0x00978FD0, 0x00964360, 0x0093DC10.
- In place on the row's existing local model (access / const / virtual only; the defined-symbol diff shows only each row's own leaf):
  0x009109B0, 0x0093D3A0, 0x0093D800, 0x0093DB10.
- Leaf-only self renames, both PDBs: 0x00956FE0, 0x009551A0, 0x00978FD0, 0x009109B0, 0x0093D800 (UAE -> MBE), 0x0093D3A0 (QAE -> ABE),
  0x0093DB10 (QAE -> MBE; no landed referrer). Every landed referrer of each renamed address moves here (RENAMES section 2).

## Headers (add-only opt-ins; default path token-identical apart from pin_asserts' `_AT` forms)
CAIStateGroup_BriarRoseWillUse.h _BRIARROSEWILLUSE, CCreatureAction_BriarRoseShotFarInto.h _CTOR, CCreatureAction_ThrowShotObject.h _CLASS,
CAIStateGroup_MazeRangedCombat.h _MAZERANGEDCOMBAT, CAIStateGroup_OpinionReactionRoot.h _COMMENTABOUTHERO,
CAIStateGroup_CreatureAdvanceInCover.h _GETCOVERTHINGLIST. No landed row defines any of them.

## Checks
- Split gate (GOTCHAS nf120 line): five universally referenced targets dropped from the recompile (above), each already bound under its new
  spelling. 272 rows rechecked, 272 good; gate 0; bindings_multi_check 1.
- Table: no set grew. Shrank: 0x0097DB70 3 -> 1, 0x008FD000 6 -> 3, 0x012944D4 2 -> 1, 0x0093D800 2 -> 1, 0x008E7790 2 -> 1,
  0x008FCF40 11 -> 9, 0x00A55A30, 0x00857F30, 0x008B7FF0, 0x0062E900, 0x004C73D0, 0x00A01B50. 0x009572A0 1 -> 0: 0x00956FE0 now binds the PDB
  `??0CCreatureAction_BriarRoseForcePush@@QAE@AAVCThingCreatureBase@@0@Z` there, a name the landed 0x008E8830 wrongly binds at 0x008E8C50
  (the InvulnerableTeleport constructor), so the table keeps one address per name; 0x008E8830 is queued.
- One-definition (`logs/symbol_diff.txt`): own leaves, shared-header inlines and COMDATs. 0x009551A0 now emits
  `??_7CCreatureAction_ThrowShotObject@@6B@` (and its ctor / dtors) from the shared _CLASS opt-in as COMDATs; the landed 0x009566C0 emits a
  different COMDAT copy from its local model with a placeholder slot: queued to move onto the opt-in. Link test over the 10 objects: no
  LNK2005.
