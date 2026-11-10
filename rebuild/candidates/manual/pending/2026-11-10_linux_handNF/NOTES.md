# agentNF hand-back notes (2026-11-10)

## What is here

- **`rows/<addr>.cpp`**: 75 full row sources.
  - All were re-graded in one final pass with `FABLE_HDR_OVERLAY=$S/w22/agentNF/hdr`: 74 RELOCATION_MATCH and 1 MATCH (005ba9be).
  - Grades and bindings come from `work/bind.py` (it runs `reloc_identity.check`). Trailing 0x90/0xCC are no longer stripped, because
    00846100 ends in `jmp rel8` = EB 90.
- **`rows/<addr>_test.cpp`**: 9 new or changed tests, each passing under g++ -std=c++11: 0041d207, 00429c15, 00485524, 005bb04f,
  005bb0a7, 005bb3ef, 005bac52, 005babb8 and 005ba78b. Every other row keeps its landed test unchanged.
- **`hdr/`**: overlay of 3 shared headers with opt-in additions only. `hdr/README.md` gives the opt-ins, the evidence, and the rows
  that need each one.
- **`STATUS.tsv`**: one line per row (75). Columns: old bindings, new bindings, grade, two-source evidence, rename, notes.
- **`NAME_FIXES.tsv`**: landed spellings proven wrong (24 entries). One entry, 0x005BB3CC, is marked as a one-source candidate only.
- **`differ/`**: the three job-3 rows, parked (`differ/README.md`).

## Job 1: missed-binders sweep (67 rows)

### Targets and rows

**Target 0x0042A168 `clear<long>`, plus 0x0042AB58/0x0042AB73 `_List_base<long>`:**
- 0041d207 is re-authored as the vtable slot-29 method:
  - Real name `NUISystem::CManager::ClearActionParams`, `?ClearActionParams@CManager@NUISystem@@UAEXXZ` (Ego_r + FableWin).
  - Module/name change: `NUISystem::CManager::ClearActionParams`, was `vfunc_29` / `CObject_0041d207::DestroyMembers`.
  - It has a new test.
- 005a30a1 and 0053e880 have binding-only fixes.
- 00846410 and 00846100:
  - Both now use list<long> from `stlport/fable_containers.h`.
  - Real callee names: `GetAppearanceModifierList`, `GetAppearanceModifierListWithCoversBodyAreaFlags`,
    `RemoveAppearanceModifier`, and the enum `EHeroAttachableAppearanceModifierType`.
  - 00846100 needs the overlay (`FABLE_RETAIL_STLP_LIST_REMOVE_DECL`).
- 005b43a2 and 005b425b use `FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE`, with the constructor body as a same-TU specialisation.
- 005127a0 binds none of the targets; it was checked and left alone.

**Targets 0x00429C15 / 0x00428B10 (CCountedPointer<CComponent> constructor and DeleteData):**
- 00429c15 is re-authored as an explicit constructor instantiation.
  - Module/name change: `CCountedPointer<NUISystem::CComponent>::CCountedPointer`, was
    `CCountedPointer_00428b10::CCountedPointer_00428b10`.
  - It has a new test.
- 005ee99d, 00555eb0, 0053f810, 005fc2d0, 005fc8a0, 005518e0 and 00551bc0 get binding fixes. Some of these leave row debt (see below).

**Targets 0x004291DE / 0x00429C6F (ReleaseData and Reset):** 51 binders re-graded under the ReleaseData proposal below.

**Coordinator-added binders:**
- 005a731f: CComponentBase is replaced by CComponent.
- 00485524 is re-authored as the scalar deleting destructor
  `??_G?$CActionParamCCountedPointer@VCComponent@NUISystem@@@NUISystem@@UAEPAXI@Z`.
  - It is built by explicitly instantiating the constructor only. An explicit class instantiation does not inline the destructor and
    gives DIFFER(28v43).
  - Module/name change: `NUISystem::CActionParamCCountedPointer<NUISystem::CComponent>`, was CLandscapeBackgroundSubNodeSplitBranch.
  - It has a new test.

**Targets 0x004196B2 / 0x005EDFD9 / 0x005F900A:** none of the scanned rows still binds a wrong spelling (scan in `work/hits.txt`).

### PROPOSAL (needs a coordinator decision): 0x004291DE is ReleaseData, not the destructor

This supersedes fix [1].

The names:
- 0x004291DE = `?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ`.
- 0x00429C53 = `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ`, a 5-byte `jmp 0x004291DE`.

Evidence:
- Ego_r.pdb's `??1<CComponent>` is 5 bytes (0x00438DDC..0x00438DE1).
- 0x00429C53 is called at the scope ends of locals: 0x0054522E and 0x0056BBB9.
- 0x004291DE is called on live pointers: the ClearBastardChild loop, Reset 0x00429C72 and ShareData 0x00429217.
- FableWin.pdb has 420 ReleaseData instantiations for 419 destructors.

The same body+jmp pairs exist for CAction (0x00429160 / 0x00429B72), CMeshDataBank (0x0042918C / 0x00429BF9) and CList
(0x00429288 / 0x00429D8E).

The landed rows 00544d20 and 0056ad40 already bind `??1` at 0x00429C53. Until this is decided, the catalog has `??1<CComponent>` on two
addresses.

If the proposal is adopted:
- **Overlay:** take `hdr/engine/CCountedPointer.h` with `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA`. The coordinator opt-ins
  `_RESET_CALLS_DTOR` and `_DTOR_DECL_ONLY` then have no users in this hand-back.
- **Self rows:**
  - 004291de: `template ...::ReleaseData();`.
  - 00429c53: explicit destructor instantiation, which produces the jmp.
  - 0042920a: ShareData.
  - 00429c6f: Reset.
  - 00429e16: re-authored as `std::pair<const CCharString, CCountedPointer<CComponent> >::~pair`. Leaf
    `??1?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@QAE@XZ`. It uses `_RELEASEDATA_DECL_ONLY`.
    Module/name change: was `_global::Dtor_CCharStringBase_00429e16`.
- **Not yet done:** about 332 call sites elsewhere bind `??1<CAction>` at 0x00429160. They need the same respelling, which this sweep
  did not do (it was out of the target list).

If the proposal is rejected, every overlay-dependent row in STATUS.tsv (grade column `FABLE_HDR_OVERLAY (...)`) must be reverted to its
landed source.

## Job 2: Codex fixes [5]-[10] (8 rows)

**005b4a95 `CTCInventoryClothing::GetCurrentTotalArmorValue`:**
- It is moved onto 005b4eba's define/include set: `stlport/fable_map.h` / `fable_containers.h`, real CDefPointer / GetSubDef /
  CDefinitionManager types.
- It needs the overlay `engine/CTCInventoryClothing.h` with `FABLE_RETAIL_CTCINVENTORYCLOTHING_TOTALARMOUR`, which declares the method.
- Every callee now binds the 005b4eba spelling (STATUS.tsv). Its test is unchanged.

**005ba9be:**
- `_M_find` is now a member template. The row explicitly instantiates `_M_find<_TreeKey>` and grades MATCH.
- Rename: leaf only. Module/name stays `std::_Rb_tree<EDamageAttribute,...>::_M_find<EDamageAttribute>`.

**Self rows re-authored on shared headers, with new module/names for the rename tool:**

| Row | New module/name | Was | How it is built |
|---|---|---|---|
| 005bb04f | `std::_Rb_tree<EClothingSuitPart,EClothingSuitPart,std::_Identity<EClothingSuitPart>,std::less<EClothingSuitPart>,std::allocator<EClothingSuitPart> >::~_Rb_tree` | `_global::StdList_Dtor_005bb04f` (leaf OnReadFinished; manifest CActiveFile) | `stlport/fable_map.h` + `FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE` |
| 005bb0a7 | `std::list<CDefPointer<const CAppearanceModifierDef> >::insert` | `_global::StdList_Insert_005bb0a7` (free ListNode_InsertBefore) | `_M_create_node` kept out of line by an explicit-specialisation declaration |
| 005bb3ef | `std::list<CDefPointer<const CAppearanceModifierDef> >::push_back` | `ListWrapper_005bb3ef::ListPushFrontWrapper_005bb3ef` (catalog label `CCreatureAction_SheatheItemToInventory_ForDivineWrath_ctor` is wrong) | Frame-pointer size TU; unreferenced in retail |

**Extra rows, the out-of-line callees of the above, which carried propagated wrong names:**

| Row | New module/name | Was | Evidence |
|---|---|---|---|
| 005bac52 | `std::list<CDefPointer<const CAppearanceModifierDef> >::_M_create_node` | `std::list<CDefPointer<const CCreatureGenerationFamilyDef> >::_M_create_node` | Its only caller is 0x005BB0A7 |
| 005babb8 | `std::_Rb_tree<EClothingSuitPart,...>::clear` | `_global::RbTree_Clear_005babb8` (leaf OnKill) | Callers 0x005BB04F and 0x005BB102; 005bb102 already binds this spelling |
| 005ba78b | `std::_Rb_tree<EClothingSuitPart,...>::_M_erase` | `LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray` | Only callers are itself and 0x005BABB8 |

**Not changed:** 005baf94, 005bb3b3 and 005bbb34 already carry the right names; only their binder 005b4a95 was wrong.

**Candidate, not proven:** the unreferenced 5-byte `jmp 0x005BB04F` at 0x005BB3CC is probably `~set<EClothingSuitPart>`. There is one
source only (bytes), so it is in NAME_FIXES.tsv as a candidate.

## Job 3: parked

0064fdb4, 00651273 and 00e5e350 are parked in `differ/`; see `differ/README.md`.
- 00e5e350 was re-measured here at DIFFER(584v585).
- The two DrawAt rows have no catalog oracle. They depend on Codex's header integration and, for 0064fdb4, the flag `/d2inlS200`.

## Job 4: debt notes (no changes made)

- **005518e0 / 00551bc0:**
  - The whole row lives in the invented namespace `RetailUI`, including its own symbol. `RetailUI::Pointer` is
    `CCountedPointer<NUISystem::CComponent>`.
  - The constructor 0x00429C15 is still bound as `??0Pointer@RetailUI@@`.
  - These are inline_depth rows; a full re-author is needed.
  - 005fc2d0 and 005fc8a0 are the same kind of debt (invented `NScreen005fc2d0` / `NScreen005fc8a0` namespaces). Only DeleteData
    binding was fixed there.
- **00443d70:** spells `CQuestInfoElement` as a top-level type. Ego_r.pdb has `NPlayerGui::CDrawQuestInfo::CQuestInfoElement`, e.g.
  `?Clear@CQuestInfoElement@CDrawQuestInfo@NPlayerGui@@UAEXXZ` at Ego_r 0x00688DCB and
  `?GetElementByIndex@CDrawQuestInfo@NPlayerGui@@QBEPAVCQuestInfoElement@12@J@Z`. Respell it with the nested class from
  `engine/NPlayerGui_CDrawQuestInfo.h` (Codex's hdr has one).
- **0x0042ABCA:** reloc_bindings.tsv has 9 spellings for this target, from these rows:

  | Row | Spelling |
  |---|---|
  | 0047d6f2 | `CComponentList` |
  | 005836d2 | `CEntryList` |
  | 00555390 | `CDeletionParents` |
  | 00595356 | `list<CComponent*,allocator>` |
  | 0053b321 | `list<CActionParam*,allocator>` |
  | 00531e90 | `NUISystem::CComponentList` |
  | 0059de04 | `list<CComponent*>` (no allocator argument) |
  | 00545b10 | `_List_base<CComponent*>` |
  | 0059db82 | `CActionParamList` |

  - There are more than 10 callers across NUISystem (0x0041E958...).
  - Bodies of `~list<T*>` for different pointer types are byte-identical after clear() is inlined, so this may be an /OPT:ICF fold.
    "One address, one name" still needs one choice. `??1?$list@PAVCComponent@NUISystem@@V?$allocator@...@std@@@std@@QAE@XZ` is the
    best-evidenced: FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE cites 0x005A8A33 in the CTCInventoryTrade TU, whose list is
    list<CComponent*>.
  - The proof should come from the caller's member types, per caller, before respelling.

## Other conflicts found (not fixed)

- 0x0059A994 is a second CList-typed release in the CFrontEndManager TU. This hand-back binds it as `ReleaseData<CList>`, which clashes
  with 0x00429288. Its T is unproven (maybe CScrollableList or CMenuEntry).
- `SetStaticCast<CList>` is on both 0x0042A2DB and 0x0042A2CA (pre-existing).
- Pre-existing multi-spellings across rows:
  - CInterfaceMap and CLongMap.
  - The NScreen* namespaces.
  - CAction 0x0042BED4 is spelled CDeletion.
- The ShareData name for 0x0042920A rests on three things: the catalog name, callee identity, and the PDB member set (188 ShareData
  instantiations, none for CComponent).
