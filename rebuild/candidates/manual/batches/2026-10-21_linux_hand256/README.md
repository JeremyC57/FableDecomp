# Hand chunk 256 (CTrappedMan .. CWorldMap): 31 of 40 byte-exact

All 31 matched rows grade RELOCATION_MATCH under RTM cl 13.10.3077 (gall.sh). Each has a behaviour test that passes under
g++ (tall.sh) and VC7.1 (tall.py). Three rows build with `/GS`: 00407040, 004071c0 and 0049f180. No row needed `/EHsc`
or the QFE compiler, and no row has a jump table.

## What matched (31)

**CTrappedMan**
- 00ef1e10 `Main`: the trapped man's hit/health messages. It matched first try on the CHHUndead script model.

**CTriangleStripifier2**
- 00f3d830 `CalcSectionScore`: scores a strip section from its polygon mask and stripped vertices.
  - Shape: same-TU noinline stand-ins for CalcSectionPolyMask and StripUnusedVerts.
  - The stand-in must assign the vector's three pointers, or VC drops the free.

**CUIIconsDef**
- 00432c70 `operator=`: the def copy with the whole base chain inlined. It uses the CFrontEndDef operator= model.

**CUserProfileManager**
- 00407040 `DeleteProfile` (/GS): deletes every file in the save directory, then the directory. Matched first try.
- 004071c0 `GetDirectoryNameFromProfileName` (/GS): the profile's save directory with the `.\<dir>` prefix stripped.
  - Shape: the success path is nested so that the failure return comes last.

**CV_BeardyBaldyScript**
- 00e50650 `GoTalkToBeardyBaldy`: moves the minimap marker between BeardyBaldy and the nearest barber.
- 00e50e40 (shares the label with 00e50650): the mirror step, with the markers swapped.

**CV_BodyGuardScript**
- 00e5d9a0 `HenchmanDeductWages`: deducts the henchmen's wages while they are hired. Uses the Sunnyvale master data.

**CV_ChapelOfEvilScript** (all propagated labels; see Label corrections)
- 00d04770: the bandit-camp gate thread. The CScriptThing dtor is declared out of line (DTOR_OUTLINE).
- 00dd9900: a prison-race step.
- 00de1770: a Knothole house step. The dtor is declared out of line.
- 00e10bf0: the Main of the WaspChaseWoman follower entity.
- 00e6a7f0 `DoMission`: waits for DarkwoodChapel, spawns WatchForLeaving and hides things.
- 00edc450 `DoMission`: the Witchwood3 copy of 00e6a7f0.

**CV_ExposeMayorScript**
- 00e7a5c0 `PhaseTwo` and 00e7a9f0 `PhaseThree`: the quest stages.
  - Shape: the `!(!x)` WAIT_UNTIL form.
  - Shape: a const-reference local for virtual dispatch.

**CV_FisticuffsClubScript**
- 00e82650 `InitialiseVariables`: resets the fight state and the timers.

**CV_GhostGrannyNecklaceScript**
- 00e8cf40 `CheckBanditDead`: waits for the kill, then sets the flag.

**CV_LostTraderScript**
- 00e0c350 (the label is wrong): a secret-passage quest ender. An implicit CWideString temporary converts the literal.

**CV_TravellingHeroesScript**
- 00ee07e0 `GossipSetup`: CCharString gossip registration. `operator==` is inline.

**CVectorBase**
- 0067cc50: `vector<CBlurredAttitudesDef>::_M_fill_insert`. Uses same-TU stand-in helpers.

**CVertexStreamConverter**
- 00ac4d70 `ConvertStream`: matched first try.

**CVillageTaskCarryCrate**
- 00861670 `IsBetterThan`: priority comparison, then the crate distance (Pickup inverts the sense), then IsTaskAvailable, then IsCreatureMoreAppropriateForTask.
  - Shape: labelled `fail:`/`avail:` gotos reproduce retail's block order and its tail-duplicated `return false` epilogues.

**CWorld** (the engine header `engine/CWorld.h` is used for `this`)
- 0049f180 `ActivateWorld` (/GS): the staged start-up. The world-activated CGameEvent posts a bool payload.
  - Shape: the bool payload goes through an inline template `Add(const T&)`, so its temporary sits in the dead argument slot.
  - Shape: the loading-stage marker's second argument is a `long` (`xor edx,edx`).
- 004a0370 `DisplayCollectableRentInformation`: totals the collectable and maximum rent of the rented buyable houses, then formats TEXT_GUI_COLLECTABLE_RENT into the living hero's region display.
  - Shape: a same-TU noinline stand-in for the 0x4AD780 house search. With it the house vector does not escape, so VC stops re-zeroing ebp before the vector free.
  - Shape: the out-pointer `GetInterface(&house, 0xd5)` is used without a null test.

**CWorldMap** (local model; retail STLport containers are 12 bytes, see Review at landing)
- 004fbde0 `SaveWorldToBinaryFile`: writes the map count and each map's SaveBinary through a CDiskFileWin32. Matched first try.
- 00502170 `CreateWorldLevelWad`: when both WAD switches are on, adds every level (from index 1) and its thing file to the WAD bank.
  - Shape: a same-TU noinline stand-in for GetLevelFilesFromWorldFile 0x00501D30. VC then keeps the local vector's begin and end in ebp/ebx through the loop and the inlined destructor.
- 00502850 `CacheMaps`: loads or unloads by player proximity.
  - Shape: write `Maps[i]` twice. A reference local moves the address add.
  - Shape: retail indexes the unload test with the *load type* (`Maps[loadType].Loading`). This is retail's behaviour, kept as is.
- 0063a420 `GetWaterSizeZAt`: uses the model of the landed sibling 0063a540.
  - Shape: a named `const CMap&` local, so that PeekMap runs before `&local` is pushed.
- 0063e480 `GetHeightZBelowPos`: the highest mesh hit between the position and the ground.
  - Shape: a same-TU noinline stand-in for the 0x49C490 rough-area search lets VC keep the vector start in ebp across the loop's calls.
  - Shape: the physics pointer is named before the segment end, and the search box is a call temporary.
- 0063eb20 `DestroyThemeAt`: paints the destruction theme, records the block, clears layers, sets the blend and resets the map's engine blend.
  - Shape: a same-TU GetDef stand-in (the landed 0x63F3A0 body) folds the null test of the def release.
  - Shape: inline STLport push_back with the frame-byte tag. The 8-aligned frame comes out by itself.

## Parked for Fable 5.1 (9)

- 00e2eac0 CV_AmbushScamScript::TricksterApproach DIFFER(304v363): retail expands the inline ~CScriptThing at three sites (out of line 0x004AA840 only on the distance path), zero-stores a dead dword at the frame base and keeps edi as the zero register; the lane expands at the first site only | tried: for(;;) loop, `const CScriptThing&` binding, killed-flag variant (262v363) | next: lane gap: inline-destructor expansion budget per function
- 00e32080 CV_AmbushScamScript::WatchForExpressions DIFFER(275v371): retail inlines ~CScriptThing at the loop end and calls it out of line at the terminating returns; the lane tail-merges three out-of-line calls and keeps `mov bl,1` where retail stores immediates | tried: base form, --qfe, two loop variants | next: lane gap: inline-destructor expansion sites
- 00e504c0 CV_BeardyBaldyScript::WatchForBarberLadyDeath DIFFER(354v394): retail keeps the zero register edi after the GetNearest call and expands ~CScriptThing inline at the second wait-loop exit; the lane calls 0x004AA840 there | tried: while-alive / nested-terminate forms, MsgIsKilledBy early-false form, e50_b/e50_c variants | next: a `__forceinline` release helper at the loop-2 exit with the out-of-line dtor elsewhere (the push_back first-site trick)
- 00ead9f0 CV_MurderTwistScript::GemsFoundProcess DIFFER(356v356): retail reloads the counted-pointer Info into eax for operator delete; the lane uses ecx | tried: release shapes, operator delete / char* delete, local copy, `--x == 0`, QFE | next: lane gap: register choice
- 00beab40 CWaterPatchMesh::GetWaterType DIFFER(325v345): retail keeps this/x/y/theme0 in edi/ebx/esi/ebp and spills theme1/theme2; the lane spills x and this | tried: seven source variants (probe/bea_a..g: loop nesting, named theme locals, call forms) | next: lane gap: register allocation
- 0062bd80 CWorld::EAControlledCreatureStrafe DIFFER(339v354): retail keeps the inlined has-weapon bool materialised (`mov al,1 / xor al,al ; test al,al`) and overlaps the dead UID slot with the action; the lane jump-threads the bool | tried: six source variants of the weapon test (probe/62b_a..f: named bool, inline helper, GetInterface forms), plus same-TU stand-ins for LowerBound 0x4365B0 and CMemoryBuffer::Read (no better) | next: lane gap: bool materialisation / frame overlap
- 004fec80 CWorldMap::UpdateDestroyedThemeAreas DIFFER(337v337): only the count/index register pair is swapped (retail count=ebx, index=ebp) | tried: 14 source variants (probe/4fe_a..n), including one reused `pos` and a C2DCoordI user copy ctor (fixed the box copy); same-TU C2DBoxI stand-ins (worse, 321v337) | next: lane gap: register-pair swap
- 005022f0 CWorldMap::DeleteAllLevels DIFFER(351v359): retail keeps the dead finish store after the erase copy-down before zeroing LoadedMaps (byte count in esi, zero in ebx); the lane removes that dead store | tried: named `__false_type` tag, `capacity() > 0`, inline `clear()` member, swap with an empty vector (fixes the zero-store order; this is the form kept in differ/) | next: lane gap: dead-store elimination
- 0063e590 CWorldMap::GetNavigationLayerAt DIFFER(340v340): one store moved; retail stores Bind2.P1 (&hasNavigation) after pushing &finder and loading World, the lane right after the lea (the Bind3 branch is byte-exact) | tried: 24 declaration orders, named/temporary filters, ctor forms, search-tools local/getter, non-const refs, FilterBind helper, a same-TU ForEach stand-in | next: lane gap: instruction scheduling

## Skipped

None. Every row is a single function.

## Label corrections

| Address | Correct identity | Label it carries now |
|---|---|---|
| 0x0041A4F0 | NResourceDirectoryNames::GetSaveDirWithoutTrailingBlackslash | "GetSaveDir" (that is 0x0041A540) |
| 0x00F3CF50 | CTriangleStripifier2::CalcSectionPolyMask | "Mesh_CalcSectionScore" |
| 0x00406350 | GetValidProfileName(const CWideString&) `__fastcall` | |
| 0x00A87090 | CArray<unsigned short> copy ctor | "Vector_Copy" |
| 0x00E50E40 | the mirrored marker step, not the twin of 0x00E50650 | GoTalkToBeardyBaldy |
| 0x00D04770 | bandit-camp gate thread | CV_ChapelOfEvilScript |
| 0x00DE1770 | Knothole house step | CV_ChapelOfEvilScript |
| 0x00E10BF0 | WaspChaseWoman follower entity Main | CV_ChapelOfEvilScript |
| 0x00DD9900 | CQ_PrisonEscapeRescueMotherScript | ChapelOfEvil EndMission |
| 0x00EDC450 | the Witchwood3 DoMission copy | suspect label |
| 0x00E0C350 | a secret-passage ender, not CV_LostTraderScript | CV_LostTraderScript (no CScriptThing parameter) |
| 0x0067CC50 | vector<CBlurredAttitudesDef>::_M_fill_insert | "CVectorBase::resize_with_copy" |
| 0x00E827A0 | unknown | CScriptBase_InitialiseVariables_Check |
| 0x0099B6B0 | CWideString(const char*) | AssignFromWide |
| 0x0099B510 | ~CWideString | ~CCharString |
| 0x00CD23B9 | scripted-handle IsScripting | HasPhysicsMesh |
| 0x00CD2770 | scripted-handle Release | Clear |
| 0x00B43720 | CEngineMap::PeekThemeId(x, y, layer) | PeekLandscapeHeight |
| 0x009D8240 | empty `__fastcall` loading-stage marker | $E2 |
| 0x009D8250 | empty `__fastcall` loading-stage marker | $E2 |
| 0x009F1760 | CGameEvent::AddData | CMemoryBuffer::Copy |
| 0x00997B30 | returns 0x800, the default bank block size | AddChildPrimitive |
| 0x004AD780 | the buyable-house GetAllLeafThings instantiation | |
| 0x006B8410 | the spell-effect interpolator's screen-effect refresh | |
| 0x0051BE20 | resize | GetAnimsWithNameStartingWith |
| 0x00A3A410 | the WAD bank add-file update | |
| 0x006C20A0 | the level-loader busy test | IsActionPointFree |
| 0x013B89BC | the world-frame global | |
| 0x0049C490 | CThingSearchTools "all things in rough area" query, `(const C2DBoxI&, filter, vector<CThing*>&)`, `ret 0Ch` | GetPBestThingInRoughArea<HasTrackName, Nearest, CThingTrackNode> |
| 0x004FB310 | CWorldMap::PeekDefinitionManager | _Cons_val<...CParticleMorphs...> |
| 0x0081E920 | CMap::GetNavigationLayerAt(const C3DVector&) const | _Cons_val<...CParticleMorphs...> |
| 0x0063A9E0 | CWorldMap::SetThemeBlendAtBlock(const C2DCoordI&, unsigned char, unsigned long, EThemePaintType) | SetThemeAtBlock (the 0x0063A970 name) |
| 0x00618550 | vector<C2DCoordI>::_M_insert_overflow | FUN_00618550 / CVector_InsertRange |
| 0x0063F3A0 | the generic CDefinitionManager::GetDef body | GetDef<CThingPhysicalSwitchDef> (here the def is a CEngineThemeDef) |
