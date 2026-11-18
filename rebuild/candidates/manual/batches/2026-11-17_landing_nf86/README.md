# 2026-11-17 landing nf86: name-fix round 7, NF86 (36 rows: the nf84 follow-ups, q11098..q11120)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes".

NF86 took the 23 entries queued at the nf84 landing. Brief, list and records: `naming/2026-11-17_agentNF86/`.

## Rows (STATUS.tsv, 83 = 80 locked + 3 claimed through the lock): 36 ported, 27 not-a-binder, 20 blocked (13 authoring, 7 port)
- **Self renames (21), among them:** the CHitParameters `scalar_deleting_destructor` 0x00812110 and `CCountedPointer<CHitParameters>`
  DeleteData / Reset (were `_global::...` stand-ins); the EObjectAugmentationType tree `~_Rb_tree` / `clear`; the CPersistTraits ctor / dtor
  pairs 0x006DBFE0 (CComboMultiplierData) and 0x006DC1B0 / 0x006DC1F0 (CHeroStatIncreaseData) that NF84's 0x006DC020 placement implies (were
  `CUnprovenContainer_...` / `CRetailTree_...` stand-ins); the NUISystem deque `_M_reserve_map_at_back` / `_M_reallocate_map` and its
  `__uninitialized_copy` / `__copy_aux` / `__copy` helpers (two were catalogued as CStaticMeshPlacement / CNavigatorAStar members); 0x004DA252
  the EHeroExperienceStatCategory tree `_M_erase` (was `LTextBinTree<...>`); NCreatureMode::CModeBriarRoseConcentrate::OnDeactivate 0x008E7E70.
- **Compiler-emitted COMDATs through a scaffolding reference (coordinator accepted):** 0x00860150 (the CCreatureAction_GetUpFromSitting
  constructor) and the `??_G` rows 0x00B9C110 / 0x00B9C130 / 0x00BCA060 reference the header's inline constructor from an anonymous-namespace
  function, so VC7.1 emits the constructor (or the vtable and its scalar deleting destructor) as a pick-any COMDAT. The helper is marked
  `// Fidelity: scaffolding, not retail` (the coordinator added the `Fidelity:` tag); its name is hashed per TU and never collides. GOTCHAS
  line added; the same pattern is proposed (queued) for the open one-definition cases.
- **Blocked (20):** 0x004585A1 and 12 more `??_G` rows (each one's constructor is a separate strong row), the CTCInventoryBase wrappers
  q11109..q11111 (names proven, in NAME_FIXES; the PASS22 CCategory / NInventory_CTCInventoryBase.h header chain must be fixed first),
  0x0074ACE0 (an all-local 612-byte model; C2259 on the shared header), 0x005959AB (DIFFER 87 v 88: retail keeps `_M_cur` in esi across
  `back()`).

## Entries (APPLIED.tsv, 23): 15 done, 1 done-already (q11098, a Ghidra label fix), 7 blocked

## Rows and checks
- **Section 1: 21 self renames (28 file moves).** Modules match their siblings' catalog spellings.
- **Headers (5 opt-ins in 4 files):** engine/CCreatureAction_GetUpFromSitting.h (`_CTOR_INLINE`), stlport/fable_deque.h (`_STLP_DEQUE_UNINIT_COPY_BODY`,
  `_STLP_STACK_EMPTY`), engine/CAIStateGroup_OpinionReactionRoot.h (`_THINGREACTINGTO`, 3 asserts pinned), engine/CEngineSubPrimitiveSpriteTrail.h
  (`_IMPLICIT_DTOR`, 1 pinned). Token-identical without the opt-ins (`tokid.log`); `lane_merge/assert_names_check.py`: 0 mismatches against
  49b943df65.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 36 rows graded from the repo after landing: 36 RELOCATION_MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 40 binding lines and added 29; bindings_multi_check is at its baseline (`NullCoord`).
- `standin_leaf.py` warns on the three `??_G` rows because it only knows the spaced catalog spelling `scalar deleting destructor`; the
  underscore spelling is the catalog's other established form (1,922 rows).

## Name fixes queued (`land1117fu`, 13)
NF86's NAME_FIXES.tsv (12: the Ghidra label of 0x01436E9C, the CTCInventoryBase wrapper names, 0x00664670 `SetCurrentAction(const
CCountedPointer<CCreatureActionBase>&)` whose catalog name `_Cons_val` is wrong, and others), plus the proposal to re-author the
one-definition `_DEFINE` rows with the scaffolding pattern.

## Rule slips
- About four Bash calls exited non-zero; one vl.sh run used a relative log path (the redirect failed; nothing was written in the repo).
- 0x008E7E70 was probed after its new form was graded (the final file was regraded).
- The final vland ran in the background, so the harness kept its task output outside NF86's directory.
