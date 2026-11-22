# Landing nf253 (Account A, 2026-11-22): ??_GCIEngine off its strong scaffolding, UnsheatheMeleeWeapon + 0x00890C80, the GetLocalSeed binder (4 rows)

Agent NF253 (q12710, q12712 done; q12711 in part; q12713 blocked with a slot map; q12659 open). Hand-back:
`pending/2026-11-22_nf253_handback/` (evidence/e4 = the per-class vtable slot map for the four blocked ctor rows).
- **q12710 0x00B23770** (`??_GCIEngine@@UAEPAXI@Z`, all three PDBs): now 0x00B23750's scaffolding verbatim; it no longer strongly defines
  `??0CIEngine@@QAE@XZ` (the last QAE definer), `??0CInitBaseClass@@QAE@XZ` or `?MakeOne@@YAPAXXZ`. Its `??0CIEngine@@IAE@XZ` copy EQ
  owner 0x00B23750; vtable = the same 85 slots. Catalog `ScalarDeletingDestructor` -> `` `scalar deleting destructor' `` (0x00B2AB10's
  spelling), files renamed. Link: per-row duplicates 3 -> 1 (the accepted nf251 fold; 0 counting one object per TU).
- **q12711:** new add-only `FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHEMELEEWEAPON` in engine/CThingCreatureBase.h
  (`?UnsheatheMeleeWeapon@CThingCreatureBase@@QAEXXZ`, three PDBs; 0 asserts moved; equiv_check over 57,125 rows: 0 differ).
  0x00890C80 (EntityUnsheatheMeleeWeapon) on the 0x00890D80 recipe (self leaf + four callees onto PDB spellings; it keeps its /GS);
  0x006649C0 on the 0x00664A10 recipe. 0x006649C0 3 -> 2 (0x0096B600 left, queued), 0x006A15A0 6 -> 4. Five stand-in table lines deleted.
- **q12712 0x008EE720:** fully on CMaterialDef.h / CWorld.h / CMainGameComponent.h / GFRandom.h; 0x0049D800 now one spelling; its GFRandom
  and vector<long> copies EQ their owners; the CEngineRandomOwner table line deleted.
- **Recheck** (`logs/recheck.log`, landed tree): the 4 changed rows + 0x00B23750, 0x0049D800, 0x006648D0 / 0x00664A50 and the table witnesses
  0x008E4100 / 0x00979F70 / 0x0049F180 / 0x008EE5A0 (12 rows): 1 MATCH + 11 RELOCATION_MATCH, all PASS; 0 RELOC_CONFLICT.

## Coordinator rulings on NF253's questions
1. 0x006648D0 / 0x00664A50 as not-a-binder (every callee already binds its PDB spelling): accepted; their local-model ports queued as debt.
2. 0x00B23770's catalog rename to `` `scalar deleting destructor' ``: applied.
3. The two new LNK4006 between an equal pick-any copy and a strong owner (0x00890C80's `??0CGameEvent` v 0x004159B0, as sibling 0x00890D80;
   0x008EE720's GFRandom v 0x004CDAE0): accepted (nf250 ruling 3, the equal-copy ruling); the owners' scaffolding queued.
4. Entry 4 (L72 / L74 / L92 / L100) runs as its own unit, one class per pass, L92 first, per the NOTES slot map, with the four Clone rows and
   0x008C9E40 moving with their ctors: next agent (NF255).
Rule slips (0x00890C80 compiled before its lock; one grade without /GS from a script bug, fixed; two compiles before the clang
precheck): noted.
