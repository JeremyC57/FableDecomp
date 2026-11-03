# Chunk 729: CTCInventoryMap map markers (nothing matched; both rows parked)

| row | result | attempts | best |
|---|---|---|---|
| 0x005FF190 CreateMapMarkers (4176 B) | DIFFER(4604v4176), 808 masked diff lines | a1-a7, a9-a11 (10), plus regrades final1-3 | differ/005ff190.cpp (a10) |
| 0x00603000 CreateMiniMapMarkers (9571 B) | DIFFER(9852v9571), 2215 masked diff lines (skeleton) | b1-b10 (10), plus regrades b11/parked | differ/00603000.cpp (b10) |

No payload.json: nothing graded MATCH / RELOCATION_MATCH, so vland was not run. src/ and test/ are empty.

## 0x005FF190 residue
- Same call sequence as retail except CCountedPointer<NUISystem::CComponent>::Reset: retail expands it at 2 of 9 sites
  (loop-3 core marker, teleport marker) and calls 0x00429C6F at the other 7. Ours expands 7. The extra expansions add the
  zero register, grow the frame (0x84 v 0x7C) and shift every loop's register allocation.
- A declaration-only Reset gives 4060 B, so retail's 4176 = 4060 + two expansions. Probe TUs are in inl/: an if-else chain
  expands the first two sites and calls the rest, and removing loop 1 changes loop 3's pattern. The choice follows VC7.1's
  block-frequency guess, not a linear budget.
- Levers that helped: `const CCharString& regionName = regions[i].GetName();` (retail's cached begin), FABLE_RETAIL_STLP_MAP
  (visible map::operator[]), and a named hero-position local.
- No effect: make_pair, hoisting the card declaration, marker declaration order, IsAlive.

## 0x00603000 skeleton, residue and attempts
The full body is written, and the call sequence equals retail's 363 calls except for three differences:
1. Reset is expanded at the UI_INTERESTING_MARKER site. Retail calls 0x00429C6F at the first three sites (cached loop, hero,
   interesting) and expands the ten later ones.
2. Retail emits IsThingAsPosBeingDrawn's four tests inline in the script-marker loop. They are written out by hand there; b8
   calls the function instead (closer dl but one extra call).
3. Return epilogues: retail duplicates the marker destructor at the cached-path return and jumps to the shared tail from the
   maps-not-loaded return. Ours does the reverse. The cause is register choice (cross-jumping), not source.

Other systematic residue:
- Retail stores the loop-invariant CFilter_Bind2 references (&hasTC, &inMap) once before every per-map loop. Ours re-stores
  them each iteration, about +16..31 B per loop. Probably stack-slot sharing in the bigger loop body. GFFilter_Bind2 (b7) and
  named filter locals (b6) do not change it.
- Ours stores 0 to the per-loop list's node proxy before malloc; retail's dead-store elimination removes it.
- Frame 0xAC v 0xB4.

Attempts:
- b1 (first skeleton): 9599 B, dl 4231. const LowerBound called out of line; `UIDefName = "..."` used operator=(const char*).
- b2: const CVectorMap::LowerBound specialisation plus `UIDefName = CCharString("...")`. All calls aligned; dl 2656.
- b3: named find iterator in the prologue (retail reads end() after _M_find).
- b4: the interesting test written directly in the `if`. No change.
- b5: compare operands swapped to `FromName != MapIsIn`, matching retail's PeekWorld use order. dl 2262.
- b6: named filter locals. No change.
- b7: GFFilter_Bind2 (Ego_r.pdb has the HasTC/InMap and HasInformation/InMap instances). Same bytes.
- b8: IsThingAsPosBeingDrawn call in the script loop. 9766 B, dl 2245, one extra call.
- b9: maps direct-init plus the bool form. No change.
- b10: no `regionName` cache. dl 2215, best.
- b11 / parked: b10 regraded after the repo rebase. Unchanged.

## Overlay (hdr/), all hdrcheck2 default=same
Carried from chunk 726: NUISystem_CManager / CList / CViewport / CScrollingViewport / CScrollBar / CKeyRedefiner, C2DBoxF,
fable_containers.h (STLP_LIST).

Rebased on the repo at 2026-11-03 12:04: CRegion.h, CWorldMap.h and stlport/fable_map.h were re-copied because Codex pass 13
typed CRegion and added GetMaps / GetMiniMapGraphic / IsOnWorldMap / offset getters and GetRegionNumberMapIsIn. The repo's
fable_map.h has insert_unique under STLP_MAP, and the chunk-726 operator[] body is re-applied in the same branch.

| header | change |
|---|---|
| CRegion.h | + GetWorldMapNameGraphic 0x006BC380 |
| CWorldMap.h | + const GetRegion 0x004FC180 |
| CTCInventoryMap.h (FABLE_RETAIL_CTCINVENTORYMAP) | GetRegionTeleporter 0x005FBEA0, ConstructRegionExitMarkers 0x005FE840, CreateMapMarkers, CreateMiniMapMarkers; protected GetRelativePosOnMiniMap 0x005FBAA0, IsThingAsPosBeingDrawn 0x005FABA0 |
| NInventory_CTCInventoryBase.h (new FABLE_RETAIL_CTCINVENTORYBASE) | GetMiniMapGraphic(CRegion&) const 0x005BEB57 |
| CQuestManager.h (new FABLE_RETAIL_CQUESTMANAGER) | active / completed quest-card lists at +0x5C / +0x60, size 0xB4 |
| CTCQuestCard.h | StatGetInterfaceType, GetPDef, IsCore / IsOptional / IsVignette, GetTeleporterRegionName, GetRegionName; includes ETCInterfaceType.h |
| CQuestCardDef.h (new) | retail layout, IsCore +0x58, IsVignette +0x59 |
| NUISystem_CSprite.h (new) | slots 142-148, members to 0x184 |
| CThing.h (FABLE_RETAIL_CTHING) | GetDefName 0x004C7CC0 (forward CDefString inside the macro) |
| CThingCreatureBase.h | inline IsDemonDoor (DemonDoor bit) |
| CThingSearchTools.h | GetAllThings template declaration |
| CDefString.h | opt-in FABLE_RETAIL_CDEFSTRING_INLINE_EQ: inline operator==(const char*) |
| fable_definition_table.h | GetStringAsCharArray 0x009D49F0 |
| CPlayerGui.h | static __fastcall PeekGuiDef 0x00438570, GetMiniMap 0x00437CE0 |
| CPlayerGuiDef.h (new, partial) | MiniMapGraphics at +0x94C |
| CMiniMapDisplay.h (new FABLE_RETAIL_CMINIMAPDISPLAY) | CMarkerInfo, GetNonAutoMarkers 0x0082A390, opaque 0xC0 |
| CTCBuyableHouse.h (new FABLE_RETAIL_CTCBUYABLEHOUSE) | CTCBase-derived, GetOwnedByPlayer inline, IsForSale 0x006BF960 |
| CTCWife.h | FABLE_RETAIL_CTCWIFE is now the whole retail class (CTCBase-derived, opaque 0xE0, the CourtingAdviceGroupNames static, StatGetInterfaceType, IsMarriedToThePlayer 0x006CEC80, IsAttractedToHero 0x006CFE80); 0x0121E210 still RELOCATION_MATCH |
| CTCShop.h (FABLE_RETAIL_CTCSHOP) | IsShopKeeperAvailable 0x00773AA0 |
| CTCFollowed.h, CTCShopKeeper.h (new) | StatGetInterfaceType, GetFollowers 0x006D5850, GetShopTC 0x00798F50 |
| CThingFilter_InMap.h, CThingFilter_HasInformation.h, CThingFilter_EnemyCreature.h (new) | PDB layouts and ctors |
| stlport/fable_containers.h | opt-in FABLE_RETAIL_STLP_FIND (`__fastcall __find` + find) and FABLE_RETAIL_STLP_MEM_FUN (const_mem_fun_ref_t, mem_fun_ref) |

The source includes stlport/fable_containers.h first so the overlay copy wins its include guard (see GOTCHAS.txt).

Header debt:
- CPlayerGuiDef, CTCFollowed and CTCShopKeeper are partial (no size assert).
- CTCWife is opaque past CTCBase, and chunk 728 is editing CTCWife in parallel, so merge by hand.
- The CTCInventoryBase GetMiniMapGraphic branch adds a forward `class CRegion;` inside the macro.
