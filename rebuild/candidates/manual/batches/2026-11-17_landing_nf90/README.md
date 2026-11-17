# 2026-11-17 landing nf90: name-fix round 7, NF90 (25 rows: the nf88 follow-ups, q11178..q11189)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes".

NF90 took the 12 entries queued at the nf88 landing (q11177, the 0x01436E9C label, was already applied). Brief, list and records:
`naming/2026-11-17_agentNF90/`.

## Rows (STATUS.tsv, 42 = 34 locked + 8 claimed through the lock): 25 ported, 17 not-a-binder, 0 blocked
- **0x004C9D60 CThing::AddTC** left its 504-byte all-local model for the shared headers and now calls `CTCBase::AllocateTC` by its PDB spelling
  (parameter / local names from Ego_d.pdb; the `Insert` + `Sort` calls follow FableWin.pdb). Retail expands CThing::HasTC at its head, which
  VC7.1 does from `/d2inlS60`: the row carries `// Flags: /d2inlS60` and an `// Inline-budget:` line (owner's 2026-11-08 inline-budget rule;
  the default and 20 / 30 / 45 give DIFFER 461 v 504).
- **One definition for CModeBase::OnDeactivate (q11189).** The body is one inline definition under a new opt-in in CTCCreatureModeManager.h;
  0x00739E60 emits it as a pick-any COMDAT through a scaffolding helper (a qualified call of the virtual under `#pragma inline_depth(0)`, marked
  `// Fidelity: scaffolding, not retail`; coordinator accepted this variant), and 0x008E74F0 (CModeIntangible::OnDeactivate) expands it. Link
  proof with `LINK=` cleared (`link/`): the old owner object plus the new 0x008E74F0 gave LNK2005 / LNK1169; the two new objects link (both
  selection 2). 0x008E74F0's opening code also matches CModeDragonBase::OnDeactivate 0x008E72B0: possibly a second case (queued).
- **CHeroExperienceDef** destructor 0x004EBB1C and `operator=` 0x004F914A, as compiler-generated members on the header's TRANSFER branch, each
  emitted by a scaffolding helper.
- **EDamageAttribute / EHeroExperienceStatCategory trees and maps:** `_M_erase`, `clear`, `~_Rb_tree`, `operator=`, `_M_copy`, `_M_create_node`,
  `insert_unique`, `_M_insert`, the map copy constructor 0x004F9960, and the pair scalar deleting destructor 0x004E3644 (several were
  `CRetailTree_*` / `_global::RbTree_*` / `LTextBinTree` stand-ins).
- **NInventory CCategory:** IsSpaceInCategoryForItem (was catalogued `CanAddItem`), UpdateItemStatsForThing, the two AddItem overloads,
  AddInventoryItem, GetFirstEmptySlot, GetSlotWithItemsOfType, and CTCInventoryBase::GetAllSelectedItems; `find_if` 0x005C1921 on NF88's recipe.

## Entries (APPLIED.tsv, 12): 12 done

## Rows and checks
- **Section 1: 25 self renames (38 file moves).** Modules match their siblings' catalog spellings.
- **Headers:** rebased on NF89's landing (NF90 dropped its copies of the three blocks NF89 had already landed). New opt-ins in engine/CThing.h
  (`_CTHING_INITTC`, `_CTHING_TCSTOBEREMOVED`), stlport/fable_containers.h (`_STLP_BITSET_SET_DECL`), engine/CCategory.h (5),
  engine/NInventory_CTCInventoryBase.h (5 CItem opt-ins and `_GETALLSELECTEDITEMS`), engine/CTCCreatureModeManager.h (`_CMODEBASE_ONDEACTIVATE_INLINE`),
  engine/CModeDragonBase.h (`_CMODEDRAGONBASE_MODEBASE`, 6 asserts pinned) and stlport/fable_algorithm.h (`_STLP_CONST_MEM_FUN_REF`).
  Token-identical without the opt-ins (`tokid.log`); `lane_merge/assert_names_check.py`: 0 mismatches against 49b943df65.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 25 rows graded from the repo after landing: 24 RELOCATION_MATCH and 1 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 38 binding lines and added 33; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 19)
NF90's NAME_FIXES.tsv (18, among them: the catalog name `CThing::$E_TCsToBeRemoved` on 0x0121A670 is wrong, the real initializer is 0x0121A440 for
the list at 0x013B8A24; the lighting global in 0x00B33010 is 0x01436EA0, not 0x01436E9C), plus the possible CModeDragonBase::OnDeactivate case.

## Rule slips
- PASS was written to STATUS before vland ran on some early rows (verified afterwards); q11189 was worked before q11188.
- 0x004EBB1C / 0x004F914A were first marked blocked after one probe (later ported); 0x005C1921 was first recorded ported while it still had local
  models (then properly ported).
- One grade went on a `@3@` / `@4@` leaf typo (0x006DC9B0); two Bash calls exited non-zero.
