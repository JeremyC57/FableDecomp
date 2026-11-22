# Landing nf255 (Account A, 2026-11-22): two CCreatureAction classes on their 81-slot vtables (L92 DragonFireBeamInto, L74 NymphSummonOutOf; 7 rows)

Agent NF255 (q12720 / q12721 done; q12722 / q12723 / q12724 / q12659 open). Hand-back: `pending/2026-11-22_nf255_handback/` (evidence/ =
the per-slot retail owners of each emitted vtable).
- **L92 CCreatureAction_DragonFireBeamInto:** ctor 0x009535B0 self leaf PAV -> AAV (both PDBs; binder 0x00951FB0 already binds AAV);
  Clone 0x00953CF0 stand-in QAE `CBase_0062f3b0` -> the PDB `UBE` spelling (its base copy ctor onto `??0CCombatActionBase@@QAE@ABV0@@Z`);
  `??_G` 0x009539C0 free `__fastcall` stand-in -> the real member. New add-only `FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBEAMINTO_INITIALISE`
  (Initialise, both PDBs, slot 12 = landed 0x008F41D0; one assert pinned).
- **L74 CCreatureAction_NymphSummonOutOf:** ctor 0x008CA600 PAV -> AAV; Clone 0x008CAA70 -> `UBE`; FinishAction 0x008C9E40 free stand-in ->
  `?FinishAction@CCreatureAction_NymphSummonOutOf@@UAEXXZ` on the shared headers; `??_G` 0x008CA9A0. New add-only
  `FABLE_RETAIL_CCREATUREACTION_NYMPHSUMMONOUTOF_OVERRIDES` (Clone + FinishAction, both PDBs). Table: the stand-in
  `?base_FinishAction_859a20@@...` line (only witness 0x008C9E40) deleted.
- Every row matched on the first form. Each class's rows share one prelude (CCombatActionBase.h's MELEE branch with 0x00955620's opt-in
  set), so the vtable / `??_G` copies in their objects are identical; every one of the 81 slot symbols names the retail owner of its
  target (0 errors), equal to 0x00955620's HEAD vtable on every shared target (77 / 77, 76 / 76). Pick-any: each `??_G` copy EQ its row,
  placement `??2` EQ 0x00405C10. Link: 0 duplicates. equiv_check: 0 of 57,125 rows differ.
- **Recheck** (`logs/recheck.log`, landed tree): the 7 changed rows + the includers / binders 0x00951FB0, 0x009539A0, 0x00953A00, 0x0091C6A0,
  0x008F41D0, 0x00955620, 0x00859A20 (14 rows): 14 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.

## Coordinator rulings on NF255's questions
1. The MELEE-branch prelude for these classes' rows: accepted (only the class rows emit these vtables, and only that branch declares the copy
   ctor the Clone rows call; the callers' own branches are unchanged).
2. The placement-copy helper to emit `??_G` when the class ctor is out of line (anonymous namespace, `// Fidelity: scaffolding`): accepted.
3. Header-based tests for the 7 rows: queued.  4. NAME_FIXES items: queued (five slot self rows on free stand-ins, 0x008F41D0 port debt,
   the pick-any implicit `??1` copies in 0x00951FB0 / 0x0091C6A0).
5. q12722 (L72), q12723 (L100; equiv_check over 0x00955620, which includes CCombatAction_WillHealInto.h), q12724 (0x0096B600), q12659: to
   the next agent (NF257) on this recipe. q12723's listing of 0x008C9E40 was wrong (it is L74's FinishAction): REJECTED.tsv.
