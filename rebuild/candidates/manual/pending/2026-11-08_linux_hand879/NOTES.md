# Hand chunk 879 (CWorldMap .. CTCInventory): 0 of 2 byte-exact

## What matched (0)
None.

## Parked for Fable 5.1 (1)
- 0x00502E90 CWorldMap::PopulateRegionWithVillagers DIFFER(13821v13622): every DisplayProgress segment calls the same
  callees in the same order as retail except two inline-budget sites. P9: ours expands CreateRelevantPersonAndUpdateStats
  (0x004FF750); retail calls it. P11: ours calls the female `vector<CThingAICreature*>::push_back` (0x00517A10); retail
  expands it. The rest is register colouring (esi=0 / edi=peopleDef swapped against retail, scratch rotation in P19, World
  loaded before the inline PeekDefinitionManager in P15) and the frame (0x19C v 0x194). Best v21: md 2902 / mdf 1434,
  54 grades.
  | tried: body nested in the GetSubDef success branch; retail branch order in phase 1 (`GetProperty() != 3` first, plus the
  discarded GetHomeBuilding evaluations); `++it` at the end of the loop body; named `float totalWeight`; school declared before
  phase 2; scoped CDefPointer in phase 13; `long remaining = homeSpaces; while (remaining--)`; the erase(key) forceinline
  overlay; scoped `#pragma inline_depth(0)` on the phase-1 list push_backs (kept, v16) and on P9/P10/P11 (v13/v17/v18/v20,
  which cascade); /d2inlS (no effect) and a /d2inlT sweep (non-monotonic, never retail's pattern); declaration-order sweeps
  (b01-b10, c01-c04, d1-d6, e1-e4).
  | next: find a spelling that outlines P9 without freeing budget for P12 (vector copy) and P14 (SetHomeBuilding). For example,
  try a /d2inlT bisect with the P9 pragma present, or move CreateRelevantPersonAndUpdateStats's definition after the row in
  the TU. Then re-read c2prio for the esi/edi swap (peopleDef CDefPointer lifetime / declaration point) and the 8-byte frame
  excess.

## Skipped (1)
- 0x0058AF10 CTCInventory::OpenPCOptionsMenu (13048 bytes incl. the 17-entry jump table 0x0058E1C4..0x0058E208; extents
  verified with xdis): not drafted (0 grades). This session's budget all went to 0x00502E90.
  Resume notes:
  - Retail skeleton (strings decoded): tmp/skel_58af10.txt, 1907 lines.
  - TU: frame-pointer size TU (`#pragma optimize("y", off)` + `#pragma optimize("s", on)`, `leave; ret`), as the landed
    CTCInventory_OpenPCScoreboard_00587b93.cpp.
  - Shape:
    - DeleteMenuComponents 0x00587A41.
    - CFrontEndManager::GetInstance; a find loop over the components for "PC_OPTIONS_LIST", else CreateComponent.
    - About 40 `GetChild(name)` (vtbl +0xC) frame children with AddFrameChild (+0x1F0) / AddViewportChild (+0x238) for the
      descriptions.
    - The clickables: listener AddListener (+8), then SetListenState(0x19) (+0xC).
    - About 40 CActions: `new(0x10)` CAction, a CCountedPointer, AddParam 0x0053FDB0 / 0x0053FDE0, and a `new(0xC)` param
      (vtable 0x0122F844, value 0x19), pushed with push_back 0x0042B646.
    - OpenInGameMenuPC 0x004804DE; the 17-case switch on [esi] (cmp 0x10 at 0x0058D8DE).
  - Idiom reference: the parked sibling rebuild/candidates/manual/pending/2026-10-27_linux_hand649/differ/00587bff.cpp
    (OpenPCSkillsMenu) uses local stand-ins; use the shared engine/CTCInventory.h, NUISystem_CAction.h, NUISystem_CComponent.h
    and CFrontEndManager.h headers instead.

## Label corrections
See NAME_FIXES.tsv:
- 0x00512210..0x00512350 are CThingAICreature::SetHomeBuilding / SetWorkBuilding / SetFatherCreature / SetMotherCreature /
  SetSpouseCreature. They were landed as unattributed AssignTarget*.
- 0x00662010 is GetProperty and 0x00662000 is GetCreatureType. Landed as CPixelShader::CountReferences and GetMovementMode.
- 0x0049DBD0 is CWorld::PopDontPopulateNextLoadedRegion, landed under CRegionManager.
- 0x005143E0 is list<CThingAICreature*>::size, landed as CGuiControlTreePane::SortTreeRecursively (ICF caveat).

Unproven, so not in NAME_FIXES. The ICF may have folded byte-identical bodies:
- 0x0051B6E0, landed as map<long,long>::map, and 0x0051B720, landed as CRetailTree_erase00519970.
- In this row they are the map<CThingBuilding*, list<CThingAICreature*> > default constructor and destructor (Ego_r
  0x004EA060 / FableWin 0x01C9F800).
