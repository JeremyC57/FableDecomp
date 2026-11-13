
## bwf1 author session (2026-11-12) -- running notes
- BIG LEVER (probe p1): a VISIBLE, non-capturing body of CTCInventoryMap::CreateMapMarkers (0x005FF190, same TU, defined earlier) under
  `#pragma auto_inline(off)` takes hunks 427 -> 234 (frame-masked 2778 -> 2842/2900): VC7.1 then knows marker_regions does not escape,
  keeps its _M_finish in ebx in the display loop and the whole B half (manager split, frame in edi, display-set zero) becomes retail's.
  The probe used a DUMMY body (not shippable). The real body exists as a DIFFER draft:
  rebuild/candidates/manual/pending/2026-11-03_linux_hand729/differ/005ff190.cpp (never passes `regions` itself on, so non-capturing).
- Head: with p1, order `PanMode=false; MapRegions.clear(); std::vector<CRegion> regions; marker_name` (o3) gives retail's head
  ALLOCATION (edi=0/finish, ebx=start, PanMode immediate) but the regions init lands after the clear (retail: before). 209 hunks.
  `*has_regions = regions.size() > 0` (j4) also flips the head allocation but adds the imul (10274 B). Allocation is on a knife edge.
- Remaining after o3: [eax+esi] v [esi+eax] in the three expanded Compare loops; 0x0061042C `mov esi,ecx/push esi` v reload [esp+8Ch];
  fxch/fadd 0x0061136A (FITTED origin+Position); A-loop `mov ecx,ebp` / push order; alignment pads follow.
- No effect (do not repeat): set it++/const_iterator/outside decl; decl order of frame/manager; has_regions spellings (begin!=end,
  ==false, named bool) on the old base; Compare body = landed 0x411570 text; foldable num_regions!=0 guards in B; visible dummies of
  GetRegionsToDisplay / CreateMiniMapMarkers / ConstructRegionDescription (no change beyond p1).

## bwf1 author hand-back (2026-11-12): DIFFER 10242v10254, 427 -> 70 hunks
- differ/0060eeb0.cpp = c/r2.cpp. What moved it:
  1. **Real body of CTCInventoryMap::CreateMapMarkers (0x005FF190, same TU, earlier) compiled in the row's TU under
     `#pragma auto_inline(off)`** (text = the hand729 DIFFER draft rebuild/candidates/manual/pending/2026-11-03_linux_hand729/differ/005ff190.cpp,
     its 9 extra defines + 4 includes, minus fable_game.h; plus CMAINGAMECOMPONENT_PLAYERMANAGER/_GETPLAYERMANAGER and two new
     overlay opt-ins CTCQUESTCARD_ISVIGNETTE / CREGION_WORLDMAPNAMEGRAPHIC). VC7.1 then knows marker_regions is not captured and the whole
     B half allocates like retail (427 -> 234). A dummy non-capturing body gives the same result, so only the escape fact matters.
     CAVEAT for landing: CreateMapMarkers is NOT landed (DIFFER draft); the rule wants a landed real body -- this row waits for 0x005FF190
     or the coordinator accepts the reconstructed body.
  2. Head order `PanMode=false; MapRegions.clear(); std::vector<CRegion> regions; marker_name("")` (234 -> 209): gives retail's
     ebx=start / edi=finish allocation, but places the regions init after the clear (retail: before). Retail order (Pan; regions; clear)
     with this TU = 95 hunks (zero constant in ebx again). Knife edge: `*has_regions = regions.size() > 0` also flips it (but adds imul).
  3. Named `CText* subtitle_text = static_cast<CText*>(subtitle.Get());` used for SetValue and AddPositionChild (209 -> 70):
     retail keeps it in esi across AddChild (0x0061042C `mov esi,ecx` ... `push esi`).
- Residue at 70 hunks (r2): (a) head order above; (b) frame: regions at S28 v retail S24 -- a 4-byte shared temp slot (LowerBound key
  0x6C / display insert temp / "HeroGuildComplexInside" temp; retail S30) sits below regions; (c) A-loop scheduling: retail hoists
  `mov ecx,ebp` above the `||` jne (0x0060EFBE) and sets ecx before the last push (0x0060F001, 0x0060F01C); (d) `[eax+esi]` v `[esi+eax]`
  in the three expanded CCharString==/Compare loops (0x0060FE86, 0x00610510, 0x006105A6); (e) x87 at 0x0061136A: retail
  `fxch st(1); fadd origin.Y` (Position.Y*scale first), ours `fld origin.Y; fadd st(2)` ... `fstp st(0)`.
- Tried without effect on (e): Y += origin.Y, Y = Y + origin.Y, Position += origin, statement orders T,F,Y,X / X,T,F,Y / Y,T,F,X.
  On (c): reversed `==`, ParentThing->, named const CRegion& / long region_number. On (a): u1/u2/u3/t1-t3/c1-c3/o1-o3 orders.
  Visible dummy bodies of GetRegionsToDisplay / CreateMiniMapMarkers / ConstructRegionDescription: no further change.
- Best next ideas: (1) frame slot order -- find which temp merges into the S24 slot before regions (try CTHING_GETTC_EXPANDED or a
  named key for the quest-card PeekTC; the display_regions.insert temp as a named long); fixing the frame may also move the head
  allocation tie; (2) for (d) try CCharString::operator== bodies (pointer-equal / length / Compare order) in the overlay; (3) for (e)
  try the scaling as one expression per axis or GetUIScale() into a named C2DVector per axis.
- Also tried on (d) with r2: Compare body `++b; ++a`, landed for(;;) text, both -- no change (the SIB order is not from the Compare
  text; look at CCharString::operator== / CBasicString::operator== in fable_string.h next).
- Grades this session: ~78 (ATTEMPTS.log). Best = c/r2.cpp = differ/0060eeb0.cpp.
