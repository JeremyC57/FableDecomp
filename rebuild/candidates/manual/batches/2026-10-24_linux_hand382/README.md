# Hand chunk 382 (CTestQuestNameCompare .. NPlayerGui::CStatBar): 36 of 40 byte-exact

## What matched (36)
- **STLport sort over CTestQuest (6: 0x623C10 __median, 0x623E90 __unguarded_linear_insert, 0x623F80 __push_heap,
  0x624760 __linear_insert, 0x624880 __make_heap core, 0x624C90 __insertion_sort).** These are the test-quest name-sort
  helpers. Shape: the STLport algorithm TU is /Gr. The empty comparator is passed BY VALUE, so every compare runs with
  `ecx = &comp`, the argument slot. CTestQuest's IMPLICIT memberwise operator= and copy ctor are inlined inside loops
  (six CCharString calls, with the ColourIndex dword moved between the second and third) and called out of line for the
  last store. The signed `/2` is `cdq ; sub ; sar`. Divide-by-28 is the `0x92492493` reciprocal. The copy_backward tag
  temporary needs the real `__copy_backward` body out of line in the TU (the 0xADBE70 lesson).
- **NUISystem::CUIStateDef::Transfer (0x625630).** Vtable slot 18; persists the graphic index, position, zoom, colour,
  update time and change flags. Shape: speed TU, members re-based on retail (def base 0x3c, 12-byte vector). Retail
  transfers StateChangeType, LinearChange and StateChangeFlag after UpdateTime, not in declaration order.
- **STLport containers for the combat action (0x62F590 vector<const CAnimationEntry*> copy ctor, 0x62F610
  list<CCountedPointer<CStrikeEvent>> copy ctor, 0x62F690 list insert).** Shape: STLport 4.x verbatim.
  - The vector ctor homes the allocator temporary in the dead `x` slot.
  - The list ctor has _List_base INLINED, and the iterator with a user copy ctor is built in its argument slot.
  - The insert has _M_create_node inlined (malloc(16), no null test), with `_Construct` taking ecx/edx under /Gr.
- **CRGBFloatColour::GetTRGBALong (0x62F9C0).** Packs a float colour into A8R8G8B8. Shape: one
  `((((A*255)<<8 | R*255)<<8 | G*255)<<8) | B*255` expression; the accumulated word lives in edi.
- **Rb-tree / map persistence for the UI def maps (8 rows):**
  - 0x631020 and 0x6310E0: `_M_copy` for map<ETableSprites,long> and map<unsigned long,CCharString>.
  - 0x632990 and 0x632C90: `insert_unique`.
  - 0x6327A0 and 0x632AF0: `CPersistContext::TransferMapBinary`.
  - 0x6327E0 and 0x632B30: `GFSerialiseMapBinaryOut`.

  Shape: STLport 4.x verbatim, with _M_clone_node/_M_create_node inlined at both sites and `this` parked in the
  `push ecx` word. insert_unique's first `_M_insert` passes (y, y, v), and both iterator temporaries are homed in the
  dead `&v` slot. The two-case Mode switch lowers to `sub eax,2 ; je ; dec eax ; jne`, with the mode-3 arm first.
  CPersistTraits<CCharString> carries a CCharString DefaultVal, so the value-traits local is the `push ecx` frame word,
  built first and destroyed last.
- **NThingMap::CThingMapManager::IsToBePlacedInMapwho (0x637F40).** Tests whether a mapwho component's radius/layer puts
  the thing in the map. Shape: the layer is a LONG local assigned from the unsigned char result.
  `if (a||b||c||d) return true; return false;` gives retail's 8-bit `xor al,al` / `mov al,1` exits.
- **set<long>::equal_range (0x639250, plain MATCH).** Shape: both bound walks are inlined. VC evaluates the pair
  arguments right to left, so the upper_bound walk runs first.
- **CWorldMap cell queries (0x63B750 FindMaxGroundZInCell, 0x63B7F0 FindMaxThemeZInCell).** Return the highest height
  over the four in-map corner blocks of a cell. Shape: `unsigned int i` over the 4-entry corner table gives the indexed
  walk and `jb`; `int i` gives pointer induction and `jl`. Two named longs plus a fresh C2DCoordI temporary per query
  produce the `and esp,-8` frame.
- **CWorldMap::InitPassabilityFromThemesIfNecessary (0x63E2E0).** Clips the area to the world map, collects its maps
  and lets each init passability on the local box. Shape: C2DBoxI needs a memberwise USER copy ctor (the implicit
  one loads in another order). A named `CMap& map = GetMap(*it)` orders the slot-18 call before the argument pushes.
  The STLport vector dtor is `if (_M_start) free`.
- **Thing filters (0x63F150 blocker/bounds-contain-point, 0x63F1E0 IsNot + that test, 0x63F2C0
  CFilter_Bind2<OverlappingCircle, CWorldMap::CThingFilterBase>::operator()<CThing>).**
  - The `&&` with a 0x40-byte CPhysicsMeshObject temporary gets a conditional-destruction flag. Fix: a `__forceinline`
    helper with a NAMED mesh local. VC7.1 keeps both identical CanBlockNavigatingCreature arms of the ternary.
  - Inner filters used inside another operator() must be `__forceinline`; plain inline stays a call.
  - The circle test copies the landed 0x63EF50 form (`d2 = dx*dx; d2 += dy*dy; if (d2 < r*r) return true; return false;`).
- **CEngineManager::AddChangedArea (0x640560).** `ChangedAreas.push_back(area)`. Shape: STLport push_back inline with a
  placement-new null test; `_M_insert_overflow` (0x640670) is a same-TU noinline body. ChangedAreas is at +8 (the
  retail list is 4 bytes).
- **CTCLight::AttachParticles (0x641260).** If the parent thing has interface 0x82 and the attacher's particle list is
  empty, attach. Shape: reuses the landed 0x83CA90 inline HasTC/GetTC model (LowerBound 0x40F020).
- **NPlayerGui GF layout helpers (0x643BFC GFToScreenScaled, 0x643CA1 GFAnchorToTopRight, 0x643CDA
  GFAnchorToBottomLeft, 0x643D0C GFAnchorToBottomRight).** Map 640x480 GUI coordinates to the real render target.
  Shape: `#pragma optimize("s", on)` (ebp frame, `leave`). The __fastcall class result comes through a hidden pointer in
  ecx with the argument in edx. The (x, y) constructor arguments evaluate right to left, so pos.Y is read first.
- **CPlayerGui::AddHitByMelee (0x643D8C).** Spawns the def's SmackEffect emitter at the hit position, oriented
  up (0,0,1) / forward (0,1,0). Shape: optimize("s"), with the def fetched first into a named local. SmackEffect is at
  retail +0x48C (PDB +0x4BC).
- **NPlayerGui::CStatBar::DrawStatBoxWithText (0x646A46).** Draws the stat box, then white text at
  MoneyTextOffset + pos, right-aligned to the screen edge when asked. Shape: optimize("s"). The screen position is a
  NAMED local built with the (x, y) ctor; an inline operator+ computes X first and shares a temporary slot.
  MoneyTextOffset is at retail +0x4AC.
- **NPlayerGui::CStatBar::CStatBar (0x65431D).** Builds the CStatBase part, six zeroed indices, the stat-bar font from the
  def (counted-pointer temporary), an empty change-text list, Resize(5) handles and Resize(1) effects. Shape:
  optimize("s"). Retail keeps `def` in edx across the base ctor call, which needs the landed CStatBase ctor body
  (0x64CC11) as a same-TU `auto_inline(off)` + noinline definition. StatBarsFontName is at retail +0x4E0.

## Parked for Fable 5.1 (4)
- 006379d0 NThingMap::InitLayerInfo DIFFER(52v53): retail keeps the running max in ecx and the table pointer in eax (`mov [max],ecx` 6 bytes), the lane swaps them (`mov [max],eax` 5-byte a3 form); every other instruction identical | tried: optimize("s") int index loop, an explicit pointer walk, the `i < g_NoMapwhoLayers` bound, the QFE compiler (the speed build unrolls: 189v53) | next: lane gap: register allocation (same under RTM and QFE)
- 006384b0 NThingMap::CThingMapManager::GetAllActiveThingMapsInArea DIFFER(133v132): ebx/ebp swapped (retail maps=ebx, it=ebp; ours maps=ebp, it=ebx), so `mov esi,[ebp+0]` is 1 byte longer; with `// Flags: /Oa` everything else matches (127v132 without /Oa) | tried: plain, /Oa, a vector member with begin()/end(), `(*it)->` twice (129v132) | next: declare `it` before `maps` is first used, or a named `CThingMap* map = *it` local; else lane gap: register allocation
- 00643b9f GFGetGraphicFrame (file-local, NPlayerGui TU) DIFFER(66v67): retail rounds with an inline-asm `fstp [ebp+0Ch] ; fld [ebp+0Ch] ; fistp dword [ebp+8]` in an ebp frame, C++ gives `call __ftol2` and no frame; the wrap-around arithmetic and unsigned->float conversion match | tried: the C++ `(long)` conversion under optimize("s") (always `__ftol2`; GOTCHAS: dword fistp ROUND helpers are asm-only) | next: lane gap: retail inline asm (bare `fistp dword`): a `retail_asm` grade candidate under the 2026-10-24 owner exception
- 006445ba NPlayerGui::CDrawBase::ClearEffects DIFFER(108v109): the second element fetch for Kill's GetPItem is `mov eax,[esi] ; lea ecx,[edi+eax]` in retail, `mov ecx,[esi] ; add ecx,edi` in ours | tried: cast to GetPItem, inline operator->, long index, QFE compiler | next: lane gap: register allocation (try a named element-pointer local once more at landing)

## Skipped
None.

## Label corrections
| Address | Catalog label | Real identity |
|---|---|---|
| 0x625630 | InitialiseDefaultCameraTags | NUISystem::CUIStateDef::Transfer (vtable slot 18) |
| 0x62F590 | lua_stackspace | std::vector<const CAnimationEntry*> copy ctor |
| 0x62F610 | std_list_Initialize_Window | std::list<CCountedPointer<CStrikeEvent> > copy ctor |
| 0x62F690 | std_list_PushBack_MorphParameter | std::list<CCountedPointer<CStrikeEvent> >::insert |
| 0x62F9C0 | PackARGBColor | CRGBFloatColour::GetTRGBALong |
| 0x6327A0 | CPersistContext_TransferList_CrimeOpinion | CPersistContext::TransferMapBinary<ETableSprites,long> |
| 0x6327E0 | CDataOutputStream_WriteMemoryAreas | GFSerialiseMapBinaryOut<ETableSprites,long> |
| 0x632990 | RBTree_find_insertion_bounds | _Rb_tree<ETableSprites,...>::insert_unique |
| 0x632AF0 | CPersistContext_TransferMap_CharStringPair | CPersistContext::TransferMapBinary<unsigned long,CCharString> |
| 0x632B30 | CDataOutputStream_WriteMap_CharStringPair | GFSerialiseMapBinaryOut<unsigned long,CCharString> |
| 0x632C90 | RBTree_LookupOrInsert | _Rb_tree<unsigned long,...>::insert_unique |
| 0x637F40 | CThingMapManager_IsOverlappingCells | NThingMap::CThingMapManager::IsToBePlacedInMapwho |
| 0x6384B0 | CThingMapManager_GetOverlappingThings | NThingMap::CThingMapManager::GetAllActiveThingMapsInArea |
| 0x639250 | RBTree_find_bounds | set<long>::equal_range |
| 0x63B7F0 | CWorldMap_FindMaxThemeMinCameraZ | CWorldMap::FindMaxThemeZInCell |
| 0x63E2E0 | CWorldMap_PostLoadTopology_Helper | CWorldMap::InitPassabilityFromThemesIfNecessary |
| 0x63F150 | CTCPhysicsBase_LoadPhysicsMesh | thing filter: blocker whose mesh bounds contain a point (no PDB name) |
| 0x63F1E0 | CThingFilter_IsNot_LoadPhysicsMesh | thing filter: IsNot && the 0x63F150 test (no PDB name) |
| 0x63F2C0 | CThingFilter_Distance_Test | CFilter_Bind2<CThingFilter_OverlappingCircle,CWorldMap::CThingFilterBase>::operator()<CThing> |
| 0x640560 | std_vector_push_back | CEngineManager::AddChangedArea |
| 0x641260 | CTCUserAction_AttachParticlesIfNeeded | CTCLight::AttachParticles |
| 0x643D8C | InitParticleEmitter | CPlayerGui::AddHitByMelee |
| 0x65431D | NPlayerGui_CStatBase_CStatBase | NPlayerGui::CStatBar::CStatBar |
| 0x437990 (callee) | _Cons_val | CGameUI AddHitByMelee forwarder (`mov ecx,[ecx+150h] ; jmp`) |
| 0xA12890 (callee) | BeginInputLoading | CPhysicsMeshObject::IsPointContainedInBoundingBox |
| 0x4294E2 (callee) | RBTree::GetMaxNode | _Rb_global::_M_decrement |
| 0x632830 (callee) | -- | GFSerialiseMapBinaryIn |
| 0x639730 (callee) | -- | vector<CThingMap*>::resize |
| 0x431020 / 0x431061 (callees) | UpdateShadowScene etc. | typed CPersistContext::Transfer overloads (unsigned long / float) |
