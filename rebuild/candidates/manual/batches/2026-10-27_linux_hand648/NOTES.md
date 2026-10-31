# Hand chunk 648 (CAtmosProcess .. CM_UndeadHere): 1 of 10 byte-exact

All 10 rows are long (811-1144 bytes) parked rows from the "fixable" bucket. One now matches; the other nine were
retested with at least two new levers each. Each is parked with a narrower residue, recorded on line 2 of differ/<addr>.cpp.

## What matched (1)
- **CAtmosProcess** -- 0x006B1F30 `UpdateInsideBuildings` (RELOCATION_MATCH 811v811, test PASS g++ + VC7.1). It takes the
  reverb environment from the current region's def. Every fourth frame it searches a +-3 box around the render camera for
  buildings the camera is inside, and that building's reverb settings win. When the inside flag changes, it cross-fades four
  sound groups and touches every SOUND_THEME def. When the reverb changes, it stores it and calls ApplyReverbSettings.
  Shape levers:
  1. `GetRenderCamera(1.0f).WorldPosition`. The virtual returns a whole 0x38-byte CCamera by value, which gives retail's
     0x7c frame. A C3DVector-returning model packs the frame to 0x44.
  2. A same-TU `GetAllThingsInRoughArea<..., CThingFilter_None>` stand-in that never reads the empty filter. VC7.1 then homes
     the filter on the existing `checked` byte (+0x13), as retail does.
  3. A same-TU `GetDef<CSoundThemeDef>` stand-in that only fills its out-parameter. This keeps the SOUND_THEME loop's
     CCharString and CDefPointer in retail's slots.
  4. The region def as a raw `PDef.Object` pointer with AddRef/Release written out. Going through CDefPointer copy or
     `.Get()` leaves a dead `add eax,24h`.
  5. A local CDefPointer template over the shared engine/CDefPointeeBase.h, because the shared CDefPointer.h has the wrong
     release shape.
  The stand-ins are not reconstructions. Both real instances are separate retail functions (0x006B2720 landed, 0x006B25E0).

## Parked for Fable 5.1 (9)
- 00912840 CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath DIFFER(860v860): the deque push_back CBezierCurve copy (12 dwords): retail software-pipelines (next load before previous store), ours pairs load/store | tried: C3DVector without user copy ctor (rep movsd, 788); copy ctor as body assignments (same); byte-packed C3DVector as landed 0x00913D60 (information probe, same) | next: a CBezierCurve with a user copy ctor copying four C3DVector members via member-init list, or push_back through an inline STLport _Copy_construct
- 009552c0 CAIStateGroup_MazeShieldedChase::Update DIFFER(868v861): GetCreature()->PDef loaded straight into ecx (retail eax then mov ecx,eax); the two def fields read in the other order; the jittered proximity not homed in the def word (+7 bytes) | tried: CDefPointer object with dtor replacing raw pointer + manual releases (866); plus same-TU GetSubDef stand-in (868, 248 v 315 differing lines -- early def shape now as retail); def through operator-> (same) | next: GetSubDef returning CDefPointer by value into a named local; read the two fields into locals in retail order before the jitter
- 00881c90 NScreenEffect::CScreenEffectPriorityStack::InsertScreenFilterEntry DIFFER(874v874): per-thing loop registers (&things[i] in ebp, operator[] result in ebx), default CScreenFilterParams zero-store order, three back() re-fetch registers | tried: *(things.begin()+i) (862, worse); end()-begin() bound (same) | next: real STLport _Vector_base/_STLP_alloc_proxy modelling of the reset-map vector (wave-25 "24i" lesson); CScreenFilterParams default ctor member-init order from the PDB
- 0079c880 CTCBulletTime::FrameUpdate DIFFER(933v933): frame slots only: retail parent-thing local B+0x20, CIntelligentPointer<CParticleEmitter> temporary B+0x24; ours parent B+0x24, temporary over the effect id B+0x1c | tried: opaque CalcAndLookup (182 lines, parent/effect swap); opaque GetSubDef too (942); effect id declared before the def pointer; parent inside the not-paused block; named emitter local | next: same-TU stand-in for the screen-particle create call that takes the emitter by reference (escape lever as in 006b1f30); CIntelligentPointer with a user dtor
- 00b17030 NParticleEngine::CPSCOrbit::DatabaseSerialise DIFFER(991v991): the loading-branch CCharString name takes a low frame word where retail homes it in the dead context argument slot (esp+78h); GetCRC data pointer into edx (retail eax then mov edx,eax); default/loop store order | tried: name in its own {} block (same); GetCRC locals length-first (same) | next: write the context argument into a by-value copy that dies before the name (frees the slot); GetCRC as a same-TU inline over CCharString::GetData()
- 00bf0740 CEnginePrimitiveRenderUtil::SetupOutlineStencilStates DIFFER(992v992): prologue only: retail `push edi` before `mov ecx,[g_pShaderRenderManager]`; ours loads ecx between push esi and push edi | tried: C4DVector(1,1,1,1) ctor + const-ref overload; literal 0 off states; PeekOutlineGlowScaleFactor inline accessor (0x01436E3C +0x1bc); glow factor as named local (988); PDB const CEngineMeshEffectOutline& parameter | next: lane gap: register scheduling (prologue) -- retest with g_pShaderRenderManager read via a static inline Get() and the outline widths computed before the first SetVertexShaderConstant call
- 007d00f0 CTCTavernGameShoveHaPenny::FrameUpdate DIFFER(1016v1016): frame 0x28 v 0x2c: retail aim C3DVector temporary B+0x14, wrapped angle B+0x20 (ours angle B+0x14, vector B+0x18); guide/new-coin temporaries 4 higher | tried: PositionMesh respelled to the PDB (..., float, float) (same); angle in an inline AngleOf() (945) | next: GetText/SetStateAfterOK with the PDB CWideString by-value signatures (temporaries change the frame); angle as a block-scoped local inside the aiming branch
- 00ea5230 CLostTraderBanditSpawn::Main DIFFER(1117v1117): in both attacker wait loops retail hoists `lea ecx,[attacker]` above the terminate `jne` (the return enters the shared cleanup one instruction later) | tried: IsThingAlive by pointer; explicit if + do/while rotation; for-loop over a named alive bool | next: a CScriptThing temporary copy of attacker per iteration (the lea is the copy ctor's this); lane gap: branch scheduling if that fails
- 00e95770 CM_UndeadHere::Main DIFFER(1144v1144): only the AXE arm (twice) picks edx/eax differently for the Game vtable loads around SetCreatureCreationDelayFrames + CreateCreature; the SWORD arm matches | tried: __forceinline CreateUndead helper (1340); !(rand()%2) condition (same) | next: swap arm order (AXE first in the source, else-branch SWORD); give the AXE arm's def name a named CCharString local

## Skipped
None.

## Label corrections
Every row below is also in NAME_FIXES.tsv, with the evidence.
- 0x006B1600 is the PDB `CAtmosProcess::ApplyReverbSettings` (private). The landed binding `LoadMainTheme_006b1600` is
  descriptive and wrong.
- 0x006B2720 GetAllThingsInRoughArea<const CThingBuilding, vector<const CThingBuilding*>, CThingFilter_None> is a const
  member (QBE). The landed row spells it non-const QAEX.
- 0x004FB150 is `CWorldMap::GetLoadedRegion() const`. 0x004FB490 is `CWorldMap::GetRegion(long)`. 0x0049D870 is the static
  __fastcall `CWorld::GetFrame`. 0x0044C6B0 is `CGameDefinitionManager::Get` (static __fastcall). 0x0082D000 is
  `CThingBuilding::IsPointInside` (const). 0x0082D8C0 is `CThingBuilding::GetReverbSettings` (const). All of these carry
  several wrong propagated labels.
- 0x013B8394 `g_pSoundManager`: many wrong bindings (g_pSoundSystem, g_pGameState, g_soundMgr).
- 0x01436E3C is the CEngineScreenEffectOutlineGlow global (its OutlineGlowScaleFactor is at +0x1bc). It is bound as
  g_pWaterRenderer/CEngineWaterRenderer in 0x00B33010. The correct global name is not yet proven (no PDB symbol found), so
  it is not in NAME_FIXES.tsv.

## Header debt
- engine/CDefPointer.h: the shared `Release()` holds the pointee in a local and stores `Object = 0`, which compiles to
  `mov edx,[eax+4]; dec edx; mov [eax],edx`. Retail is `dec dword ptr [ecx+4]; jne`. 0x006B1F30 uses a local CDefPointer
  template over the shared CDefPointeeBase.h.
- engine/CWorld.h and engine/CAIGameCameraBase.h flatten the vptr into `void* __vftable`, so the local models carry the
  retail virtual slots.
- CWorldMap, CThingSearchTools, CDefinitionManager and CRegion are included under the layout-define pattern. CRegion.h
  carries Ego_r offsets: PDef is at +0x2c there, +0x24 in retail.
- No engine headers exist for CAtmosProcess, CRegionDef, CGlobalDef, CGameDefinitionManager, CMainGameComponent or
  CThingBuilding.

## Names: proven vs descriptive
- PDB-proven: UpdateInsideBuildings, ApplyReverbSettings, GetRenderCamera, GetWorldMap, GetLoadedRegion, GetRegion,
  GetFrame, GetAllThingsInRoughArea, IsPointInside, GetReverbSettings, CGameDefinitionManager::Get, GetNumberOfDefsOfType,
  GetDef, and every member name used.
- Descriptive: the CASoundManager slot 46 `SetGroupVolume` (no PDB symbol; it follows the landed InitialiseSounds model).
