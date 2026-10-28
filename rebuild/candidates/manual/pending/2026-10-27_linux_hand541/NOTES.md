# Hand chunk 541 retry (CTCInventoryQuests .. CThingCreatureBase): 1 of 8 byte-exact

All grades are VC7.1 RTM 13.10.3077 (lane default). The QFE-4035 toolset (`FABLE_VC71=$S/wine/vc71`) and `/Oa` were
tried on the STL-container rows and changed nothing.

## What matched (1)
- **CThingCreatureBase** (the manifest says CThingPhysical)
  - 00663c40 `DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&)`, RELOCATION_MATCH 577v577, test PASS
    under g++ and VC.
  - What it does: the fly-on-spline TC prepares first, then the CThingPhysical pass runs. If an appearance is present, the
    LOD-centre distance test picks between two paths:
    - in range: Draw, the weapon trails and hit notification (only when trails are shown), the emote icon prepare, and
      the firefly sprite;
    - out of range: clear the appearance and the emote icon primitives.
  - Shape: the two non-virtual TC calls use the PDB inline out-pointer accessors `CThing::GetTC<T>(T**)` and
    `PeekTC<const T>`, which put the component in eax. The two virtual `DrawPreparePrimitivesForRendering` calls use the
    pick form of `CThing::GetTC(ETCInterfaceType)`, which puts it in ecx.
  - The leaf is the PDB `MAE` protected virtual on CThingCreatureBase. It sits in vtable 0x0125AC0C slot 36.

## Parked for Fable 5.1 (7)
- 00622a00 CTCInventoryQuests::OpenInventory DIFFER(880v877): the frame homes of the two local vectors are swapped, and retail keeps `quests._M_finish` in ebp across the gather loop (`xor edi,edi` / stores of edi,ebp,edi), where ours stores ebp three times and reloads | tried: PDB-exact model (CManager slots SetPlayerThing 26 / SetQuestInventory 17, CTCInventoryBase 0x130 + CBase, real STLport vector with `__forceinline push_back`, `__true_type` user ctor, nested IsDead if); local declaration orders; RTM and QFE; `/Oa`; `<stdlib.h>` malloc | next: lane gap: local STL container fields enregistered (retail caches `_M_finish` in a register across the loop)
- 00627400 CTCQuickAccessMenu::ConstructExpressionCategoryList DIFFER(750v742): ours stores the null `_List_base` head of the CDeletion temporary (`mov [esp+0x2c],0`). Retail drops that dead store and builds the by-value argument in place (`push ecx; lea edx; mov ecx,esp`). Everything else matches | tried: inline CCPPointerInfo ctor (gets the 1-inline + 2-out-of-line 0x429C15 CCountedPointer split exactly); `_List_base` with a local `__n`; CDeletion default ctor forms; QFE | next: lane gap: dead STLport `_STLP_alloc_proxy` null-init store kept
- 0063ab90 CWorldMap::GetThemeFaceNormalAt DIFFER(586v582): retail computes the cell fractions after the parity test, from the spilled ix/iy (faddp/fcompp consume both). Ours computes them before the test (`fadd st(1)` / `fcomp` + `fstp`) | tried: CrossProduct with const-left zero products; the normal kept in the frame; class keys fixed; invented CMap / CMapInfo models removed | next: compute the fractions inside each parity branch (duplicated expression) or as `float` temporaries declared after the parity `if`
- 0063d0a0 CWorldMap::FindGroundCollisionPointWithinCell DIFFER(1017v1017): scheduling only. Retail issues `fld dir.X` before `push esi` in the Normalise prologue, and loads the start/end/hit pointers after the first plane-normal copy (ours loads them before) | tried: class keys (C3DVector, CPlane = class); PointIsWithinCell TR/BL/TL/BR order; inline vs member Normalise | next: read the args into locals after the CPlane construction; try the C3DVector `operator/=` Normalise form from landed relatives
- 0063d4a0 CWorldMap::FindThemeCollisionPointWithinCell DIFFER(1017v1017): same residue as its twin 0063d0a0 | tried: same as 0063d0a0; renamed (it samples GetThemeSizeZAt 0x63A390) | next: same as 0063d0a0 (one fix should match both)
- 0063e6f0 CWorldMap::IsLineOfSight DIFFER(607v599): two differences, both from the list:
  - ours stores the null list head and then the new node at construction; retail keeps the node in esi across GetAllThingsInRoughArea and stores it late, with no null store;
  - as a knock-on, the CWorld manager lands in esi instead of ebx, and the two IsNot filter temporaries swap slots.

  | tried: real STLport list/_List_base (607); the earlier hand-rolled CThingList (599v599, now removed as not a real class); dropping the alloc-proxy null init by hand gives ndiff 21→17 (probe only, not real STLport) | next: lane gap: dead STLport `_STLP_alloc_proxy` null-init store kept + register caching of the local list head
- 0065cb6b CTCVillage::ReportCrime DIFFER(748v747): the frame is 0x1c where retail has 0x18. Retail shares one slot (ebp-0x14) between the two key temporaries, and keeps the deed log in ebp-0xc. Ours gives each key temporary its own slot, so ebp-8 and ebp-0xc swap and the deed log is re-read from ebp+8 once | tried: size-TU pragmas `optimize("y",off)` + `("s",on)`; PDB-exact callee rename pass (parked 750v747 -> 748v747) | next: declare the key temporary once at function scope and reuse it for both lookups through a reference; else lane gap: frame slot sharing

## Skipped
None.

## Names
### Proven (PDB spelling + a second source; every spelling checked with `objdump -t`)
- Row leaves:
  - `?OpenInventory@CTCInventoryQuests@@UAEXXZ`
  - `?ConstructExpressionCategoryList@CTCQuickAccessMenu@@AAE...` (`/Zc:wchar_t`)
  - `?GetThemeFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z`
  - `?FindGroundCollisionPointWithinCell@CWorldMap@@IBE_NPBVC3DVector@@00PAV2@1@Z`
  - `?FindThemeCollisionPointWithinCell@CWorldMap@@IBE_NPBVC3DVector@@00PAV2@1@Z`
  - `?IsLineOfSight@CWorldMap@@QBE_NABVCThing@@0_N@Z`
  - `?ReportCrime@CTCVillage@@QAEXJ@Z`
  - `?DrawPreparePrimitivesForRendering@CThingCreatureBase@@MAE_NABVCEnginePrimitivePrepareInfo@@@Z`
- Callees: every row in NAME_FIXES.tsv that is not marked PROBABLE. The evidence for each is in that file.
- `?CreateMenuEntryComponent@...MV5@MV5@@Z`: this PDB spelling only comes out when one of the two by-value CCharString
  parameters is declared `const`, because VC7.1 then skips the argument back-reference. The parked source declares the
  last one const. The bound spelling `...MV5@M2@Z` is not the PDB name.
- `?OnOpenMenus@CManager@NUISystem@@SIXXZ` (0x41D122): it was bound as a free function, `?OnOpenMenus@@YIXXZ`.

### Probable
- 0x0063A7E0 `?GetThemeSizeZAtBlock@CWorldMap@@QBEMABVC2DCoordI@@@Z`.
  - Its CMap callee 0x820100 returns ground (0x81FA20) plus the first per-cell offset.
  - The FableWin alternative is `GetThemeMinCameraSizeZOffsetAtBlock`.
- 0x004C1060 `?GetFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@M@Z`: it returns C3DVector by hidden
  return, takes the float 1.0f, and is the PDB method that fits.

### Descriptive (no PDB name exists)
- `FormatBracketedCount` (0x627400): an inline by-value Sprintf `L"(%d)"` helper with no out-of-line body.
- `CSoundManagerLH::SetGroupVolume` (slot 46, 0x00A3E9D0).
- The globals `g_pMainGameComponent`, `g_pSoundSystem` and `g_pPlayerGui`: these are the bound descriptive spellings,
  since the PDB has no data symbols for them.
- `CMapInfoArray` in 0063e6f0: a stand-in for the retail `CArray<CMapInfo>` (CWorldMap::Maps +0x24, 12 bytes).

## Label corrections
- 0x00663C40: the manifest says CThingPhysical "PrepareForRendering". It is
  `CThingCreatureBase::DrawPreparePrimitivesForRendering`, from vtable 0x0125AC0C slot 36.
- 0x0063D4A0: the manifest says "FindGroundCollisionPointWithinCell". It is `FindThemeCollisionPointWithinCell`: it
  samples GetThemeSizeZAt 0x63A390, while its twin 0x63D0A0 samples GetGroundSizeZAt 0x63A300.
- 0x00623560: the manifest says "BuildTreeArray". It is STLport `_Rb_tree::_M_erase` of `map<CThing*, CComponent*>`.
- 0x00428494: the bound spelling `...MV5@M2@Z` should be the PDB `...MV5@MV5@@Z`.
- 0x0063A7E0 / 0x0063A390: these were bound as `GetThemeMinCameraSizeZOffsetAt`.
- 0x00487DC0: the parked spelling `GetHeroCreature` should be `CPlayer::GetPControlledCreature`.
- vland RELOC_NAME labels that are wrong (ours are the PDB names): 0x004C7AA0 `GetGlobal_013b8a1c_deref0x24_004c7aa0` (is CThing::GetWorld); 0x00864FF0 `CThingPhysical_RenderWeaponTrails` (is CTCCreatureHitNotification::DrawHitNotification); 0x0049D150 `CTCPhysicsBase::GetInterpolatedPosition` (is GetPosition(float)); 0x00669390 `CViewport::ClearHandle` (is CTCEmoteIcon::ClearPrimitives).
- The full list is in NAME_FIXES.tsv.

## Header debt
- Vtable filler slots (`SlotNN`) remain in these local models:
  - CIEngine
  - CSoundManagerLH
  - NUISystem::CComponent and CList
  - NUISystem::CManager
  - NInventory::CTCInventoryBase
  - NHeroInformationScreens::CBase

  The generated `engine/<Class>.h` headers flatten the vptr, so the virtual calls cannot use them yet.
- Retail-layout offsets that differ from Ego_r:
  - CTCInventoryBase is 0x130.
  - CThingCreatureDef::Property is at +0xF0 (Ego_r +0xF8).
  - CGlobalDef::E3BodgeFireflyGraphic is at +0xD4 (Ego_r +0xD8).
  - NUISystem::CManager setters are shifted −0x14 from Ego_r.

  Each is noted in a `// Layout:` comment.

## Review at landing
- vland dry run (00663c40): RELOCATION_MATCH + test PASS, no RELOC_CONFLICT / DUPLICATE / RELOC_IDENTITY; 4 WARN SHARED_HEADER_AVAILABLE (CIEngine, CTCBase, CWorld, CTCPhysicsBase) -- the generated headers flatten the vptr / lack the called methods, so the local models stay (header debt above).
- 0x0063A7E0 is only probable: decide between GetThemeSizeZAtBlock and GetThemeMinCameraSizeZOffsetAtBlock (see above).
- 0x00428494: decide which CCharString parameter is const. Both choices give the same mangled name.
- The q.py / manifest label for 0063d4a0 is still FindGroundCollisionPointWithinCell. Grade with the Leaf line.
- Three rows hit the same lane gap (00622a00, 00627400, 0063e6f0). Retail removes the dead `_STLP_alloc_proxy` null
  store and keeps the fields of local STL containers in registers; RTM, QFE and `/Oa` all keep them.
