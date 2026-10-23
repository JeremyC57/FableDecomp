# Hand chunk 311 (CVillageDef .. CVertexShaderInput): 19 of 40 byte-exact

Lane: RTM cl 13.10.3077 (`$S/wine/vc71rtm`). All 19 matched rows grade RELOCATION_MATCH and their behaviour tests
print `<addr>_TEST PASS` under g++ and VC7.1. Every row was attempted; none was skipped.

## What matched (19)
- 004e82c7 CVillageDef::Transfer -- typed CPersistContext transfers (the catalog labels on those shared bodies are wrong).
- 00d86680 CWaitingGuardInWoods::Main -- entity script.
- 00be91e0 CWaterSeaGenerator::LoadSea -- index/vertex buffer fill from a memory stream.
- 008eddd0 CWillButtonHelperTimer::FrameUpdate -- 0x005EDCEF is a bare `ret` (ClearQuickAccessState).
- 00d68810 CWillWhisper::Main -- entity script.
- 00d823a0 NScript::CQ_HeroSoulsBriarScript::PlayCutscene -- the catalog's CWomanToAttract::DoLeavingCutscene label is wrong.
- 00ed0b10 CWomanToAttract::DoLeavingCutscene -- entity script.
- 0049e370 CWorld::UpdateEnvironmentCameraPos -- bilinear blend of the environment themes.
- 004a3200 CWorld::LoadGameState -- needs `/GS`.
- 004a67d0 CWorld::CWorld -- constructor.
- 004fe2a0 CWorldMap::LoadAllLoadableGlobalThingsFromSingleFile.
- 00506940 CWorldMap::~CWorldMap -- the three helper shapes are listed in LESSONS.md.
- 0063b0f0 CWorldMap::GetHighestThemeSizeZInArea -- renamed from the manifest's GetHighestThemeMinCameraSizeZOffsetInArea.
  - That name belongs to 0x0063BF20.
  - Ego_r.pdb lists both symbols.
- 0063bbc0 CWorldMap::ClipLinePointsToMapBox -- uses CPlane::Initialise and GetIntersectionWithLine.
- 0063bf20 CWorldMap::GetHighestThemeMinCameraSizeZOffsetInArea.
- 0063c230 CWorldMap::GetAverageThemeMinCameraSizeZOffsetInArea.
- 0063dfd0 CWorldMap::UpdateAllEdges -- needs the C2DBoxI user copy ctor.
- 00665260 CThingCreatureBase::IsPosInFOV -- the catalog label CVertexBufferWin32::ComputeAngleBetweenPositions is wrong.
  - GetPosition 0x006A4D00 is declared on CThingCreatureBase, as the landing rule requires.
- 0098d490 CVertexShaderConstantLayout::CVertexShaderConstantLayout.

## Parked for Fable 5.1 (21)
- 00aad340 CWaveFile::ConvertSamplingRate DIFFER(613v584): retail inlines an inline-assembler fistp floor helper three times | tried: plain C++ floor/_ftol | next: lane gap (fistp helpers), no C++ route.
- 0049ef50 CWorld::GetPathNameForFileName DIFFER(553v553): instructions are identical; only the frame slot order of the six CWideString locals differs (/GS) | tried: scope blocks, declaration orders | next: advisor -- what drives VC7.1's slot order for /GS string locals.
- 004a34f0 CWorld::AutoLoadRegionLeaving DIFFER(564v564): instructions identical, two frame slots swap (/GS) | tried: scopes, declaration order | next: same as 0049ef50.
- 004a6550 CWorld::PostInit DIFFER(625v625): one scratch pair swapped at the SetWorld call | tried: inline accessor, named locals | next: chained-call split variants.
- 0062b070 CWorld::EAOpenHeroInformationScreen DIFFER(898v891): retail keeps three interface keys in the dead event-argument slot (frame 0x34 v 0x38) (/GS) | tried: block order fixed, key locals | next: lane gap (locals in dead argument slots).
- 004fe030 CWorldMap::CompileLoadableGlobalThingsToSingleFile DIFFER(611v611): frame packing (int->float temp shares the list-header slot) | tried: scopes, named temps | next: advisor.
- 00500eb0 CWorldMap::LoadThingsWithSectionNames DIFFER(684v670): the loader wait loop stays rotated in ours; scratch rotation around ToWideString | tried: while / for(;;) break / continue-break, DestroyRange helper, CLevelFiles member-ctor order (frame now matches) | next: find what keeps VC7.1 from rotating the IsLoading loop (same shape in 0x00501990).
- 00501990 CWorldMap::UpdateNavMaps DIFFER(936v920): first pass; retail keeps each new-expression result in a frame slot around the flag-guarded ToWideString temporary | tried: model from 0x00500EB0 | next: name the new result, then the loop fix from 0x00500EB0.
- 00502620 CWorldMap::LoadLevel DIFFER(561v559): esi/edi role swap plus ebx/ebp caching across the by-value counted-pointer copies | tried: several local/temporary shapes | next: family gap (whole-program caching).
- 005064c0 CWorldMap::PostRegionLoad DIFFER(536v542): retail caches the list head in ebp across GetAllThings(&list); three hoisted push_back constant temps | tried: named constants, const list head, node-local ctor | next: family gap (whole-program caching).
- 005066e0 CWorldMap::CWorldMap DIFFER(605v601): retail keeps 1<<shift in ebp across the MapPlacement resize call | tried: STLport vector-base ctors (forceinline), CPlayerPositionInfo/C3DVector copy ctors, box operator=, local granularity | next: family gap (whole-program caching).
- 00507650 CWorldMap::LoadFromFile DIFFER(908v875): first pass, frame 0x18 larger, ebp/ebx role swap | tried: full model with the inline singleton Get and the STLport char-vector fill | next: slot sharing for the resize CRegion temp and the stream.
- 0063ab90 CWorldMap::GetThemeFaceNormalAt DIFFER(586v582): retail computes the cell fractions after the parity test from the spilled ix/iy | tried: cross product and normal shapes (those now match) | next: move the fraction computation into each parity arm.
- 0063aee0 CWorldMap::IsLineOfSight(C3DVector, C3DVector) DIFFER(516v516): one x87 operand pair (fld step.Z; fadd pos.Z) | tried: +=, b+a, a+b, raw pointer, operator+=, declaration order | next: advisor.
- 0063b3f0 CWorldMap::CreateLineSegment DIFFER(836v856): dy/floorX spilled into dead argument slots, flag-byte slots, inv*t re-association | tried: scoped temps, a goto tail (made the flags memory-resident) | next: lane gap (locals in dead argument slots).
- 0063d0a0 CWorldMap::FindGroundCollisionPointWithinCell DIFFER(1017v1017): scheduling only (fld dir.X before push esi; pointer loads after the first normal copy) | tried: normalise spellings, pointer locals | next: advisor.
- 0063d4a0 CWorldMap::FindGroundCollisionPointWithinCell (interpolated twin) DIFFER(1017v1017): same residue as 0063d0a0 | tried: same | next: same fix as 0063d0a0.
- 0063e6f0 CWorldMap::IsLineOfSight(CThing, CThing, bool) DIFFER(599v599): retail keeps the new list head in esi across FindThingsInRoughArea and stores it late, so the manager lands in esi, not ebx | tried: Bind2 of IsNot temporaries (fixed the arg-slot write-backs), node-local ctor, filter declaration orders, list placement | next: family gap (whole-program caching).
- 00872af0 NCameraModes::CZTargetNew::GetCameraPosition DIFFER(935v971): heavy x87 -- sin and its YAxis products before fcos, camera.Y kept on the x87 stack | tried: named s/c, inline sin/cos | next: per-term spelling pass with the landed Update/UpdateCoordSystem models.
- 009fb750 CTexture::LoadFromDataStreamToPreallocatedSurface DIFFER(789v789): registers only -- stream ebp/ebx swap and a src/dst pair swap in the row loop | tried: local stream model, signed level, size<=remaining compare, product order, src/dst declaration and increment order | next: advisor.
- 00ab3870 CVertexShaderInput::GenerateD3DDeclaration DIFFER(668v668): register allocation only (jump table) | tried: source case order (now matches the table) | next: advisor.

## Skipped
None. Every row has a src/ or differ/ file.

## Label corrections
- 0x0063B0F0 is CWorldMap::GetHighestThemeSizeZInArea; the manifest names it after its 0x0063BF20 sibling.
- 0x00665260 is CThingCreatureBase::IsPosInFOV(const C3DVector&, bool) const; the manifest says CVertexBufferWin32::ComputeAngleBetweenPositions.
- 0x007445D0 is the TC-0x34 field-of-view query; the manifest says GetAirResistance.
- 0x00751A80 is a CTCLook float query; the manifest says DrawGetSeaLevel.
- 0x0099E480 is CCharString::ToWideString; the manifest says GetDefaultVal<CAnimationSet>.
- 0x0063A390 is the interpolated ground height (landed as GetGroundSizeZAt).
- 0x00D823A0 is NScript::CQ_HeroSoulsBriarScript::PlayCutscene; the manifest says CWomanToAttract::DoLeavingCutscene.
- 0x005EDCEF is a bare `ret` (ClearQuickAccessState).
- 0x006B1960 is the CAtmosProcess ctor, and 0x004AB300 is CCountedPointer<CAtmosProcess>::Reset. Their manifest labels are propagated.
- 0x00407FD0 and 0x00406730 are the autosave region-leaving filename getters. Their "GetAutoSaveCheckPoint*" labels are propagated.
- 0x004AB490 is the thing-list dtor; its catalog label is wrong.
- 0x00AC1EE0 is CIndexBuffer::Uninitialise.
- 0x00625930 is the interface query used by EAOpenHeroInformationScreen.
- These callee labels are wrong:

  | Address | Manifest label | Actual role |
  |---|---|---|
  | 0x0051E480 | "CountReferences" | thing manager -> search tools (+0x24 -> +0x20) |
  | 0x0063F360 | "SortTreeRecursively" | list<CThing*> dtor |
  | 0x0063FAE0 | "GetPBestThingInRoughArea<...>" | ICF-shared FindThingsInRoughArea instance |
  | 0x006C27A0 | "AddChild" | CLevelLoader::CObject::SetFiles |
  | 0x006C20A0 | "IsActionPointFree" | CLevelLoader::IsLoading |
  | 0x006C2120 | "LevelLoader_CreateObjectEntry" | CLevelLoader::AddObject |
  | 0x00658603 | "$E2" | CTCVillage member called on re-region-load |
  | 0x0051D0A0 | "?FinishAction@CCreatureAction_OpenChest" | vector<CRegion>::_M_fill_insert |
  | 0x0051BE20 | "GetAnimsWithNameStartingWith" | CArray<CMapInfo>::resize |
  | 0x00993B10 | "~CChunkedFileChunk" | CMemoryDataInputStream dtor |
  | 0x009E3820 | "?RegisterTheme@CEngineStaticMapEdgeHeights" | CPixelFormat::GetColourDepth |
