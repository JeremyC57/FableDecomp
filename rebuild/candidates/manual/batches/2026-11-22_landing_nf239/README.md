# Landing nf239 (Account A, 2026-11-22): CODEXP27 name fixes, second part (18 of 20 rows), the -1.0f literal, ~_List_base<CThing*>

Agent NF239. Hand-back: `pending/2026-11-22_nf239_handback/` (of its 63 lines: 11 done, 8 done-already, 2 partial, 3 deferred, 39 open).
- **Recheck** (`logs/recheck.log`, landed tree): 18 rows, 3 MATCH + 15 RELOCATION_MATCH, every test PASS, 0 RELOC_CONFLICT.
- **Held (ruling 2):** 0x0042ABAF / 0x0042ABCA (`_List_base<NUISystem::CComponent*>` ctor / dtor self rows): renaming them alone would
  leave 5 of their 41 `list` binders naming a function nobody defines; they land with the list<CComponent*> family pass (NF241).
- **Access fixes in the rows' local models:** 0x0090FDB0 / 0x009387C0 SitInSeat protected (0x00938380 2 -> 1); 0x0065C781
  UpdateTorchTasks private (0x00659625 2 -> 1); self row 0x00726BE0 private (2 -> 1).
- **NUISystem list<CActionParam*>:** 0x00429FBE _M_create_node, 0x0042AADC insert, 0x0042A141 clear, 0x0042AB1C ~_List_base as explicit
  instantiations on stlport/fable_containers.h (2 -> 1, 2 -> 1, 3 -> 1, 3 -> 2). STLport 4.6 shape (retail's size-optimised wrappers jump
  into each); the PDBs carry Dinkumware and cannot decide it (descriptive-by-library, EXTRA_RULES 2026-11-20). Catalog-only renames.
- **CPlayerGui:** 0x00437640 IsDrawingTargetingCentre, 0x00437650 SetFlourishEffect on engine/CPlayerGui.h; two new shared headers from the
  Ego_d field lists: engine/NPlayerGui_CDrawProjectileTargetingCentre.h, engine/NPlayerGui_CDrawMeleeActionButtonEffects.h (no other
  includers). Both 2 -> 1; files and catalog renamed from placeholders.
- **0x007B2990** CTCGuard::GetWaitForRebribeInSeconds on engine/CTCGuard.h (2 -> 1; renamed from `ReturnConst_15`); 0x005F807C / 0x005F88C8
  list<EHeroAbility> _M_create_node / insert (2 -> 1, 3 -> 2).
- **0x0122DEE0 (`-1.0f`):** 0x00744500 / 0x00744540 on the literal, the g_fMinusOne line gone. NF239's compiled census of all 118 binders
  found a real bug: **0x0041735A InitWorld passed 0.5f where retail pushes -1.0f** (it graded only because the relocated operand is masked);
  fixed.
- **0x004AB490** self row `~_List_base<CThing*>` (3 -> 2; 0x004FE030's CThingList stand-in left); 0x004DE6ED onto the shared list header
  (its HEAD test no longer linked).
- **reloc_bindings:** 7 deleted, 4 added; every add replaces a delete at the same target.

## Coordinator rulings on NF239's questions
1. The two new NPlayerGui headers: accepted (PDB field lists; `PMeleeAbilityDef` under FABLE_RETAIL_DEF_CLASS_BASE is fine).
2. 0x0042ABAF / 0x0042ABCA: held for the list<CComponent*> family pass (NF241, entry 1).
3. The `FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST` comment: corrected in that same pass.
4. L38 / L39: released to NF240 with 0x006A1620.
