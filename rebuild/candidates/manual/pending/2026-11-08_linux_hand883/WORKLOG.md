# WORKLOG chunk 883 -- 0x006096E0 CTCInventoryMap::ConstructLogbookListPC

## Identity
?ConstructLogbookListPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z (Ego_r donor order; landed GetMenuEntryFromCategoryPC
0x005FAC20 callers are here). Catalog "_global::ConstructLogbookScreen" is descriptive.

## Decoded phases (retail)
1. manager = CManager::GetInstance(); entries = copy of player's CHeroLogBook::LogEntries (map copy 0x00619C50);
   list = CreateComponent("PC_LOGBOOK_CATEGORIES"); frame = list->FindChild("PC_UI_FRAME_LOGBOOK").
2. Four scroll-action groups (parent/frame x LIST/DESCRIPTION): observe/ignore CActions; group 4 runs out of inline budget
   (CAction() 0x42BE50, CCountedPointer<CAction>(T*) 0x42AA29, ~ 0x429B72 out of line).
3. `child = list->FindChild(..); frame->AddPositionChild(child);` x25 (one named local: the nested call form loads frame's vptr
   before the FindChild call); viewport fake children x8.
4. CategoriesList.clear() (out of line tree clear 0x6166E0); count switch over entries (iter++ 0x47B3E0 out of line).
5. Books: GetTC inventory, GetAllItems, IsBook -> books; PC_SUB_LIST; per book: menu entry, PC_MODEL mesh (+ anim), PC_DESCRIPTION
   details, subtitle (+ icon when GetEngineGraphic.BankIndex).
6. Seven category sub-lists (BASICS, OBJECTS, TOWNS, HERO, COMBAT, QUESTS, STORY), CategoriesList[cat] = sub.
7. Tail loop over entries (it++ with _M_increment inline): SetDefaultChild(size-1), one menu entry per log entry.
8. Final loop category = 0..7 in esi (skip 2): find(key copy) in CategoriesList; AddChild sub-list or MakeUnavailable, the
   loop variable (not the address-taken key) passed to GetMenuEntryFromCategoryPC.

## What fixed what (in order)
- STLport `_Construct` in list _M_create_node + alloc_proxy rebind opt-ins: group-4 inline budget exact; /d2inlT71 (smallest; T70 = 591, T71 = T72 = 590): all 590
  calls in retail order.
- Final loop passes `category` (register) not the address-taken `key` to GetMenuEntryFromCategoryPC: size 12832v12832 (p03).
- Position phase as `child = list->FindChild(..); frame->AddPositionChild(child)`: list in edi, whole prologue + 4 action groups
  byte-identical (p04, 303 -> 35 differing segments).
- CEngineGraphic with its two FableWin.pdb ctors (non-aggregate): GetEngineGraphic's hidden result pointer in ecx, result read in
  place (p07).
- `CRightHandedSet orientation = GetStartingRightHandedSet(index); mesh->SetOrientation(orientation);` (p08, 23 segs).
- Visible template bodies of CDefinitionManager::GetDef<T> and CDefClassBase::GetSubDef<T> (header opt-ins, retail logic of the
  landed 0x0042B040 / 0x0042B0A2): def release without the null test / dead store, appearance release `dec [ecx+4]` (p11, p12).

## Current best
p12 + header cleanup = differ/006096e0.cpp, `/d2inlT71` (final2): DIFFER(12832v12832), call sequence identical, 17 differing segments (NOSTACK/NOREG), frame
0x124 vs 0x11C.

## Remaining residue (p12)
- Count prologue: retail `xor ebx,ebx` before the clear call and the 7 counter zero stores through ebx, entries header loaded after;
  ours zero in eax after the call, header (#71, prio -14) loaded first.
- Books loop icon branch: retail subtitleText.Info in ebx, manager from memory; ours manager piece in ebx, Info in memory.
- Books loop end: retail keeps a zero ebx from the description release across the loop exit to `push ebx` (title CreateComponent).
- COMBAT block ring release `cmp [eax],ecx` (retail) vs `cmp [eax],0`; one extra `mov r,r` in the tail loop; alignment pads follow.
- All are const-0 piece / callee-saved piece choices (c2 split pass), plus 8 bytes of frame.

## Probes after p12 (no gain)
p13 counters before clear; p14 separate `it` decl; p15 chained zero init; p16 unnamed sprite cast; p17 orientation in a block
scope; p18 counters before the viewport phase (worse). final/final2 = p12 with experimental opt-ins removed: unchanged.
