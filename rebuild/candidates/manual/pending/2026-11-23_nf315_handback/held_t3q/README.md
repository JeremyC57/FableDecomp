# NF315 held set: proven rows that T3_PORT_QUEUE.tsv names inside other ports' units (pending a ruling)

Each row below carries the same kind of edit as its landing-ready siblings and was proven the same way (HEAD/new RELOCATION_MATCH,
objsec ALL_EQ under one rename, COMDATs SAME, links clean: evidence/proof_B / proof_W / proof_Q, cmpfn_*, link_B1 / B5 / Q / W;
vland --recheck-landed RELOCATION_MATCH + PASS in recheck_final.log). They were moved here at hand-back because T3_PORT_QUEUE.tsv names
them (offl.py read only column 2, the target column, and missed them), and the prefix says to stay off every T3_PORT_QUEUE unit.
Locks released. orig/ = HEAD copies (cmp-equal at hand-back); rows/ = the changed sources.

| row | unit it belongs to here | where T3_PORT_QUEUE names it |
|---|---|---|
| 0x00934920 | B1 (IsOriginatedByThing; with it the PAVCThing delete line) | column 4 of the 0x008128F0, 0x00870BB0 and 0x00934E50 port lines (binder) |
| 0x009624D0 | B5 (CombatPerformActionAndWait; with it the QAEJPAVCCombatActionBase delete line) | column 3 of the 0x0073A8A0 line (NF296 unit 4) |
| 0x007EF600 | Q (ActivateQuest; with it the reference / reference delete line) | column 4 of the 0x007B5680 port line (same-TU stand-in body) |
| 0x006EBDD0 + 0x0092B3E0 + 0x006ED510 | W owner unit (CreatureUnsheathWeapon self -> PDB; with it the QAEXPAV...J delete line) | 0x006EBDD0: column 4 of the 0x0085F0C0 port line; 0x0092B3E0: column 3 of the 0x008D5110 line (NF293 entry 30) |

If the ruling is that a row named only as a binder of ANOTHER target's port may still take an unrelated respell (NF313 changed 0x0092B3E0
in batch M and that landed today), each unit lands with its table line:
- B1+: 0x00934920 + `delete ??0CMessageEventFilter_IsOriginatedByThing@@QAE@PAVCThing@@@Z 006af2b0 00919a70` (all exe callers checked).
- B5+: 0x009624D0 + `delete ?AIAction_CombatPerformActionAndWait@CAIStateGroupBase@@QAEJPAVCCombatActionBase@@_N@Z 0097da40 0091e050`.
- Q+: 0x007EF600 + `delete ??0CCreatureAction_ActivateQuest@@QAE@AAVCThingCreatureBase@@AAVCThing@@ABVCCharString@@_N@Z 00843f50 007ef600`.
- W: 0x006EBDD0 (self -> ?CreatureUnsheathWeapon@CCombatManager@@QAEXAAVCThingCreatureBase@@W4EUnsheatheWeaponType@@@Z, full Leaf on
  line 2; old self ?...@@QAEXPAVCThingCreatureBase@@J@Z; catalog Module CCombatManager, oracle name CreatureUnsheathWeapon) + 0x0092B3E0 +
  0x006ED510 + `delete ?CreatureUnsheathWeapon@CCombatManager@@QAEXPAVCThingCreatureBase@@J@Z 006ebdd0 0092b3e0` (the exact lines are in
  work/pre_restructure_TABLE_CHANGES.tsv / pre_restructure_APPLIED.tsv / pre_restructure_RENAMES.tsv).
Note: 0x006EBDD0's `// Leaf:` and description lines were swapped after its grade (description back on line 1, the row convention); the
line count is unchanged, so no `__LINE__`-based name moves; regrade it at landing.
