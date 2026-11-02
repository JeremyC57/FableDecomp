# Hand chunk 704 (CAtmosProcess .. CBaseVideoRenderer): 10 of 10 byte-exact

All ten NA047 rows were authored from their own disassembly, callers, vtables and the PDB name sets. 10 RELOCATION_MATCH under
q.py, 10 PASS under g++ and VC7.1 (tall.sh / tall.py), vland (read-only) 10/10 RELOCATION_MATCH + behaviour PASS. Five of the ten
catalog labels were wrong (0x006B1550, 0x00747530 placeholder, 0x00CA7310, 0x007FC7A0 twin label; the PhysicsLeaf rows' module
column was a truncated template name). No extent problems: every row ends at the padding the oracle shows.

## What matched (10)
- CAtmosProcess::~CAtmosProcess 0x006B1550 (catalog "CCounter::operator="): user body `if (PSoundManager) { slot41(); slot42(); }`
  (the global re-read after the first virtual call), then the implicit destruction of PRainPlayer, a pack(1)
  CIntelligentPointer<CThingMarker> at +0x0A, which chains straight to ~CBaseIntelligentPointer (no vtable reset).
- CTCStealth::OnCreate 0x00747530 (catalog "vfunc_4", vtable 0x012689EC slot 4): `ResetStealthVariables();` with
  ResetStealthVariables a plain private member defined above in the file; VC7.1 inlines it (4 byte stores on one `xor al,al`,
  4 dword stores on one `mov eax,-1`) and tail-jumps to InitialiseSneakSpeed.
- CBaseVideoRenderer::~CBaseVideoRenderer 0x00CA7310 (catalog "CTCStealth::OnCreate"): the DX9 SDK renbase.cpp empty dtor; the real
  strmbase hierarchy (CUnknown / CBaseFilter AM_NOVTABLE, MIDL novtable interfaces, CBaseRenderer, IQualProp, IQualityControl)
  gives the five vtable stores in retail order and the tail jump to ~CBaseRenderer, plus the real vtable symbols.
- CTCKraken::StartLowAttackBeam 0x007FC7A0 / StartHighAttackBeam 0x007FC7D0 (byte twins, catalog label on both "StartHigh"):
  `DelayBeforeBeam = (long)(PKrakenDef->DelayBefore{Low,High}Beam * CMainGameComponent::GetConstantFPS()); SetMode(4 / 5);` with
  SetMode (and the two InitTentacles bodies) defined above in the same file: VC7.1 inlines SetMode and folds the switch to the mode
  store + StartBeamEffect tail. The direct `Mode = 4; StartBeamEffect();` form gives the same bytes (probe 1 of 0x007FC7A0); the
  SetMode form is kept because SetMode's case 4/5 is exactly that entry action and EndAttackBeam 0x007FC800 is SetMode(3) folded.
  `fild ConstantFPS ; fmul def+0x4C` came out of `def * GetConstantFPS()` directly (the inline static accessor).
- CMap::Update 0x00820B50 (vtable 0x0127252C slot 13): `HeightMap->Update(); NoChangedHeightAreas = 0; NoChangedGameAreas = 0;
  UpdateScriptedMapBrushes();` -- CHeightMap::Update is private in Ego_r, so CMap is a friend of CHeightMap (keeps AAE).
- CCreatureAction_ActivateZTarget::PerformAction 0x008424A0 (slot 3): `GetPlayer().AddMode((EPlayerMode)2, false);` with the PDB
  protected `CPlayer& GetPlayer()`.
- CCreatureAction_PlayAnimation::FrameUpdate 0x008426E0 (slot 56): `if (StayOnLastFrame) { if (FramesRunning + 1 >= TotalTime)
  FramesRunning--; } CCreatureActionBase::FrameUpdate();` -- retail's odd `push esi / pop esi` around TotalTime came out by itself.
- NCameraModes::CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelGotoOutsideSimple, CPhysicsCoordCartesian>::FrameUpdate
  0x00850640 and <..., CPhysicsForceModelRigid, CPhysicsCoordRigid>::FrameUpdate 0x0085CA10 (byte twins, two instantiations, each
  named from its own RTTI vtable): `OurForce->FrameUpdate(); OurCoords->FrameUpdate(); GetPhysicsState().Reset();` with the inline
  CPhysicsState::Reset (CullPhysics = false; Attractors.clear(); IncidentalAttractors.clear()) over real STLport 4.6 lists, whose
  list::clear forwards to the out-of-line _List_base::clear 0x0056CAD0. Each source explicitly instantiates only its FrameUpdate.

## Names and evidence (all proven, two sources each)
- 0x006B1550 ??1CAtmosProcess@@QAE@XZ: callers ??_G 0x004A8380 and DeleteData 0x004A9B50 (stored by CCountedPointer<CAtmosProcess>::
  Reset 0x004AB300, used by CWorld::PostInit on PAtmosProcessor +0x24 with `new (0x20)` + ctor 0x006B1960) + Ego_r/FableWin ??1.
- 0x00747530 ?OnCreate@CTCStealth@@UAEXXZ: RTTI slot 4 (CTCBase OnCreate slot, 13 landed rows) + Ego_r.
- 0x00CA7310 ??1CBaseVideoRenderer@@UAE@XZ: its five vtables (slot 0 = landed CBaseVideoRenderer::NonDelegatingQueryInterface,
  IQualProp vtable = landed get_* rows), its ??_G 0x00CA73D0, the ctor 0x00CA7360 bound as ??0CBaseVideoRenderer, the SDK source.
- 0x007FC7A0 ?StartLowAttackBeam / 0x007FC7D0 ?StartHighAttackBeam@CTCKraken@@QAEXXZ: Ego_r donor order = retail order, and the def
  field each reads (+0x4C DelayBeforeLowBeam / +0x50 DelayBeforeHighBeam) with the mode each enters (4 / 5).
- 0x00820B50 ?Update@CMap@@MAEXXZ: RTTI slot 13 + Ego_r (protected virtual, donor order after UpdateScriptedMapBrushes).
- 0x008424A0 ?PerformAction@CCreatureAction_ActivateZTarget@@UAEXXZ and 0x008426E0 ?FrameUpdate@CCreatureAction_PlayAnimation@@UAEXXZ:
  RTTI slots 3 / 56 + Ego_r and FableWin.
- 0x00850640 / 0x0085CA10 ?FrameUpdate@?$CPhysicsLeaf@...: the RTTI type descriptor of each vtable (0x01275974 / 0x01276AEC) slot 0
  + Ego_r/FableWin names for both instantiations; the base CSubComponentBase vtable 0x012758F4 (_purecall, Reset, _purecall, ??_G)
  fixes slot 0 = FrameUpdate.
- Callees: every call target was checked with calls.py against retail; spellings used are the PDB ones (GetPlayer IAE + CPlayer&,
  AddMode W4EPlayerMode, CCreatureActionBase::FrameUpdate MAE, CHeightMap::Update AAE, UpdateScriptedMapBrushes IAE,
  InitialiseSneakSpeed AAE, StartBeamEffect AAE, ConstantFPS / GetConstantFPS, PSoundManager, ??1CBaseIntelligentPointer UAE,
  ??1CBaseRenderer UAE, _List_base<pair<C3DVector,float>>::clear).

## Descriptive names
- CASoundManager virtual slots 41 / 42 (`ClearAtmosSounds`, `ClearAtmosMap`) and CASoundManager::CAtmosMap slots 0 / 1
  (`GetAtmosAt`, `GetAtmosLevelAt`): CASoundManager is a TLC class absent from both PDBs and these slots are called from nowhere
  else; names describe the CSoundManagerLH bodies (0x00A3F340 empties the two atmos-sound lists and stops their samples;
  0x00A3E920 hands the audio thread a fresh empty map object). Virtual names emit no symbol; nothing binds.
- CAtmosProcess member `AtmosMap` (+0x04, the TLC CMyAtmosMap) -- TLC member absent from Ego_r.
- EKrakenMode / EPlayerMode values written as casts (enumerator names are in no name set).

## Label corrections
- 0x006B1550 "CCounter::operator=" -> CAtmosProcess::~CAtmosProcess.
- 0x00747530 "CTCStealth::vfunc_4" -> CTCStealth::OnCreate.
- 0x00CA7310 "CTCStealth::OnCreate" -> CBaseVideoRenderer::~CBaseVideoRenderer (DirectShow strmbase, not CTCStealth).
- 0x007FC7A0 "CTCKraken::StartHighAttackBeam" -> CTCKraken::StartLowAttackBeam (the twin 0x007FC7D0 keeps the High name).
- Landed rows found wrong while settling callees (all in NAME_FIXES.tsv): the CAtmosProcess ??_G / DeleteData stand-ins, GetPlayer,
  AddMode, CCreatureActionBase::FrameUpdate, InitialiseSneakSpeed (QAE), OnSetInLimbo (non-virtual QAE + dead Leaf vfunc_16),
  UpdateScriptedMapBrushes (QAE), 0x0056CAD0 (?Clear@List@@ stand-in for the STLport _List_base::clear), ~CBaseIntelligentPointer
  binders, EndAttackBeam / KillBeamEffect / KillTentacles fastcall stand-ins, InitTentaclesOnly / InitTentaclesAndHead (one free
  function spelling on two addresses), CreateTentacles stand-ins, the whole CBaseRenderer / CBaseVideoRenderer family
  (~CBaseRenderer "CVolume", both ??_G, eight vtables).
- Not a NAME_FIX (no landed row): 0x00747500, the unreferenced byte twin of OnCreate's body just above it, is most likely the
  out-of-line ?ResetStealthVariables@CTCStealth@@AAEXXZ (Ego_r name; the body is exactly the stealth reset + InitialiseSneakSpeed).
- Not a NAME_FIX (unproven spelling): the CBaseRenderer ctor 0x00CA61D0 is bound ??0CVolume_00ca61d0@@QAE@PAX000@Z; by the SDK and the
  bound ??0CBaseVideoRenderer@@QAE@ABUGUID@@PBGPAUIUnknown@@PAJ@Z it is ??0CBaseRenderer with the same parameter list, but the
  GUID / TCHAR spelling there (UGUID vs U_GUID) should be settled when that row is re-authored.

## Header debt
- engine/CTCBase.h, engine/CCreatureActionBase.h, engine/CCreatureAction_PlayAnimation.h, engine/CIDrawMap.h, engine/CBaseClass.h,
  engine/CPlayer.h are flat generated headers (vptr as a data member, no member functions); the virtual overrides here need
  polymorphic bases, so those classes are restated locally with `// Layout:` lines (CCreatureAction_PlayAnimation uses the
  layout-rename include pattern of the landed 0x00843B40).
- No engine header exists for CAtmosProcess, CMap, CHeightMap, CTCStealth, CTCKraken, CKrakenDef or the NCameraModes classes.
  Ego_r CAtmosProcess (0x1C) lacks TLC's CMyAtmosMap member at +0x04 (retail 0x20).
- 0x00CA7310's CBaseObject is the DirectShow class; vland's shared-header check matches engine/CBaseObject.h by name only.

## Parked for Fable 5.1 (0)

## Skipped (0)
