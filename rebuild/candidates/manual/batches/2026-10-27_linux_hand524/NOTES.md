# Chunk 524 (re-queue) -- notes

13 of 20 rows RELOCATION_MATCH (src/ + test/, every test PASS under g++ and VC7.1), 7 parked in differ/, 0 retail_asm,
0 SKIP. vland dry run: 13 wins, no RELOC_CONFLICT / RELOC_DUPLICATE / RELOC_IDENTITY; the SHARED_HEADER warnings were
fixed with the `#define X XPdb / #include "engine/X.h" / #undef X` pattern (regraded RELOCATION_MATCH).

## Proven (two sources each)
Row functions:
- 0x007ED780 `??0CCreatureAction_ShowItemToThing@@QAE@AAVCThingCreatureBase@@AAVCThing@@1@Z` -- vtable 0x012714C4 RTTI
  .?AVCCreatureAction_ShowItemToThing@@ + literal 0x012715DC "STANDARD_SHOW_OBJECT" + both PDBs.
- 0x007FA320 `??$__adjust_heap@PAPAVCThing@@HPAV1@VCThingCompare_BestForTargeting2@@@std@@YIX...@Z` -- STLport 4.6
  _heap.c body + PDB class key; landed popheap 0x007FA680 binds this spelling.
- 0x008082C0 (parked) STLport `__adjust_heap<pair<C3DVector,long>*, int, pair<..>, (anon)::CCompareTargetedPositionsFor
  FirstPersonTargeting>` -- the landed make_heap 0x00808CF0 calls it; PDB class keys V.
- 0x008100D0 (parked) `?Create@CTCDExplosiveTrail@@SIPAVCThing@@JABVC3DVector@@0PBV2@@Z` -- Ego_r 0x0080AA40 / FableWin
  0x027B55B0; follows the landed CTCDExplosiveTrail::OnInitialActivate 0x008100A0; writes PTarget through a
  CIntelligentPointer<const CThing> temporary (vtable 0x01237F1C RTTI); caller 0x008C71B3 then calls AddThingToIgnore.
- 0x00816AC0 (parked) `?OnInitialActivate@CTCHitLocations@@UAEXXZ` -- slot 6 of vtable 0x012434E4 (RTTI CTCHitLocations;
  slot 6 = OnInitialActivate, as the landed 0x008100A0 in 0x01242B4C) + both PDBs + body (GetSubDef<CHitLocationsDef>).
- 0x00825E00 (parked) / 0x008265C0 / 0x00826C90 (parked) / 0x00826F10 / 0x008271E0
  `?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@V<New>@@V<Old>@@@@SI?AV?$CCountedPointer@V<New>@@@@PAX@Z` for
  <Cell3,Cell2> / <Cell4,Cell3> / <Cell5,Cell4> / <Cell6,Cell5> / <CFileFormatHeightMapCell,Cell6> (COldFileFormatHeightMap*)
  -- both PDBs; allocation sizes 0xF / 0x11 / 0x19 / 0x15 / 0x15 = Ego_r sizeofs (pack 1); version tests 2..6; each calls
  the previous link (chain from the landed <Cell2,Cell1> 0x00825700); deleters below.
- 0x0083D460 (parked) `?Create@CThingMarker@@SIPAV1@JABVC3DVector@@ABVCCharString@@@Z` -- both PDBs + final vtable
  0x01245524 RTTI CThingMarker (intermediate 0x01244F7C CThingGameObject).
- 0x0083EBD0 `?toLowerCase@LHNetworkLib@@YI?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ABV23@@Z` -- both
  PDBs (followed by toUpperCase, StartsWith); retail twin 0x0083ECF0 (toupper) and landed StartsWith 0x0083EE10 caller.
- 0x008427E0 `??0CCreatureAction_PlayIntoLoopOutOfAnimation@@QAE@AAVCThingCreatureBase@@ABVCCharString@@11M_NJJ@Z` and
  0x008428B0 `...@@AAE@AAVCThingCreatureBase@@ABVCCharString@@1JM_NJJ@Z` -- both PDBs, donor order of the four ctors =
  retail 0x00842740 / 0x008427E0 / 0x008428B0 / 0x00842960.
- 0x00842FD0 `?PerformAction@CCreatureAction_MagicBlast@@UAEXXZ`, 0x00843940 `?FinishAction@CCreatureAction_PlayAnimationWith
  LookTurning@@UAEXXZ`, 0x00843B40 `?FinishAction@CCreatureAction_PlayAnimation@@UAEXXZ`, 0x00843A80
  `??0CCreatureAction_PlayAnimation@@QAE@AAVCThingCreatureBase@@ABVCCharString@@MABVCSoundPair@@_N33JJ@Z`, 0x00844600
  (parked) `?PerformAction@CCreatureAction_Read@@UAEXXZ`, 0x00845FE0 `?PerformAction@CCreatureAction_DropWeapon@@UAEXXZ` --
  vtable slots (3 = PerformAction, 4 = FinishAction) + PDB names.

Callees (spelling in NAME_FIXES.tsv where a landed binding is wrong):
- 0x006924B0 CCreatureActionBase::GetRandomAnimFromName (KI); 0x00693B30 CCreatureActionBase ctor (AAV owner);
  0x00693EF0 ~CCreatureActionBase; 0x00694430 CCreatureActionBase::FinishAction; 0x00692910/20/30/40
  PeekDefinitionManager / PeekPlayer / GetPlayer / GetWorld (I-access, donor order).
- 0x0049D870 CWorld::GetFrame (SIJXZ; donor order GetFrame / GetFrameAsString = 0x0049D870 / 0x0049D880).
- 0x00661E70 CThingCreatureBase::IsAbortingCurrentAction (donor order with IsInMelee 0x00661E80); 0x006644F0
  CThingCreatureBase::SetCurrentAction (bool); 0x008425E0 / 0x00842DF0 the 11-argument PlayAnimation /
  PlayAnimationWithLookTurning ctors; 0x005DEC70 CAnimationEntry(const CAnimationEntry&, long).
- 0x004C7990 / 0x004C7A10 / 0x004C7A30 / 0x004C7A70 / 0x004C7A80 / 0x004C7A90 / 0x004C7AA0 CThing PeekDefinitionManager /
  PeekPlayer / GetPlayer / PeekComponent / GetSampleBank / PeekSampleBank / GetWorld (donor order).
- 0x009AD410 CDefinitionManager::GetDefGlobalIndexFromName (const); 0x00663B10 GetShotAccuracyPercentage (const);
  0x007C0AD0 NThingShot::CreateShot; 0x006A1440 CTCCarrying::GetThingInPrimarySlot (const); 0x0073ABA0
  CTCCreatureModeManager::RemoveMode(unsigned long); 0x00845CE0 CCreatureAction_DropWeapon::DropWeapon (static SI).
- 0x007F9E70 CThingCompare_BestForTargeting2::GetThingCompareMultiplier (private const); 0x007FA160 __push_heap.
- 0x0099B7D0 CWideString::operator= (ABV0 return); 0x0099B2C0 CWideString::ToCharString; 0x009C95E0
  NGameText::CDataBank::GetTextBySymbol; 0x00492E4B CSubtitleRenderer::SetText; 0x00403C60
  __char_traits_base<unsigned short,unsigned short>::compare (STLport, static).
- 0x00822BF0 / 0x00822CA0 / 0x00822D10 / 0x00822D90 / 0x00822DF0 `?DeleteData@?$CCountedPointer@V<cell>@@@@SIXPAX@Z` for
  CFileFormatHeightMapCell / Cell6 / Cell5 / Cell4 / Cell3 -- FableWin donor order (0x02245FA0.. descending cell number,
  same order as retail) + each ConvertPreviousUnknownVersionUp stores its own deleter.
- 0x005296B0 `??0CThingPhysical@@QAE@K@Z` (both PDBs; landed body stores the CThingPhysical vtable); 0x004C7380
  `??2CThing@@KIPAXI_N@Z`; 0x008388D0 CThingGameObject::ConstructFromParams (IAE, position by reference).
- 0x00810970 `?AddThingToIgnore@CTCDExplosiveTrail@@QAEXAAVCThing@@@Z` (both PDBs + caller 0x008C71D9 on the explosive
  trail's driver; appends to ThingsToIgnore).
- 0x00817BB0 GetSubDef<CHitLocationsDef>, 0x00817C70 GetDef<CHitLocationDef>, 0x00817B40 GetDef<CArmourDef> (call order
  = hit-location def then its Armour), 0x006869D0 CTCBase::GetDefinitionManager, 0x00818DC0 vector<pair<CHitLocation,
  long>>::push_back, 0x00777DB0 basic_string<char>::reserve, 0x00565A30 _String_base<char>::_M_throw_length_error,
  0x00BFED10 tolower, 0x00A14440 C3DVector::AccurateNormalise, 0x006A06E0 CTCParamsPhysicsBase ctor.

## Probable
- none beyond the above (every row name has two sources).

## Descriptive (no PDB data symbol)
- 0x01375550 `g_framesPerSecond` (long; CMainGameComponent::GetConstantFPS / ConvertSecondsToWF exist in FableWin).
- 0x013B86A0 `g_pMainGameComponent` (its +0x14 PTextBank, +0x1614C PCutSceneMainFont, Ego_r offsets).
- 0x013B8790 `g_pPlayerGui` (CPlayerGui*, +0x18 SubtitleRenderer).
- 0x013B8A1C `g_pThingManager` (landed spelling kept).
- Model-only helpers: `CTCPhysicsMethods` (method adapter over CTCPhysicsBase, landed pattern), `GetTCIfPresent`,
  `STCEntry` / `CTCMap` (landed SetEnabled model). None of these reach a symbol.

## Label corrections
- 0x007ED780 "CCreatureAction_GiveItemToThing::CCreatureAction_GiveItemToThing" -> CCreatureAction_ShowItemToThing ctor.
- 0x007FA320 / 0x008082C0 "_Adjust_heap" (Dinkumware) -> STLport std::__adjust_heap.
- 0x008100D0 "Create" -> CTCDExplosiveTrail::Create; 0x0083D460 "Create" -> CThingMarker::Create.
- 0x00816AC0 "CParticleMorphs_PopulateFromDefinitions" -> CTCHitLocations::OnInitialActivate.
- 0x00825E00 "CAFile_ReadPrimitive", 0x008265C0 "CAFile_ConstructSphereOBB", 0x00826C90 "CAFile_ConstructNavConnectorVertex",
  0x00826F10 "CAFile_ReadNavConnectorVertex", 0x008271E0 "CAFile_ConvertNavConnectorNode" -> the CReplaceSerialise<..>::
  ConvertPreviousUnknownVersionUp chain.
- 0x0083EBD0 "CString_ToLower" -> LHNetworkLib::toLowerCase.
- 0x00822BF0 / 0x00822CA0 / 0x00822D90 "OperatorDelete_thunk_*" -> CCountedPointer<cell>::DeleteData.
- 0x00661E70 "CTCPhysicsStandard::CanCollideWithLandscape" -> CThingCreatureBase::IsAbortingCurrentAction.
- 0x00692910 "CCreatureActionBase::GetPDefinitionManager" -> PeekDefinitionManager.
- 0x005DEC70 "CIVCountedPointer_Assign_005dec70" -> CAnimationEntry::CAnimationEntry(const CAnimationEntry&, long).
- 0x00810970 landed as "CUnprovenDriverOwner_00810970::AppendTrackedThing" -> CTCDExplosiveTrail::AddThingToIgnore.

## Header debt
- CCreatureActionBase: retail 0xA8 bytes vs PDB 0xB4 (the 0xC shrink after +0x84: three STLport containers); the
  rows keep a local polymorphic model next to the renamed engine header.
- CThing / CThingPhysical / CThingGameObject (0x0083D460, 0x008100D0, 0x00816AC0): retail CThing is 0x10 shorter than
  Ego_r (PhysicsTC +0x60, PTCDriver +0x5C, PDef +0x70); local models next to the renamed engine headers.
- CTCHitLocations, CTCDExplosiveTrail, NHitLocations::CHitLocation, CHitLocationDef, the COldFileFormatHeightMapCell*
  family, CReplaceSerialise / CCountedPointer / CCPPointerInfo: no engine headers; local models with Ego_r member names.
- CHitLocationDef / CHitLocationsDef: retail offsets are Ego_r less 4 (Armour +0x44, Priority +0x70, Default +0x74,
  EnableOnCreate +0x75; HitLocations +0x28).

## Parked (differ/)
- 0x00825E00 DIFFER(256v263) register allocation (retail shrink-wraps ebp).
- 0x00826C90 DIFFER(288v288) one load scheduled two instructions early.
- 0x0083D460 DIFFER(228v247) inline-constructor `new` select jump-threaded in ours.
- 0x008100D0 DIFFER(280v280) ebx/edi swap.
- 0x00816AC0 DIFFER(520v421) def-pointer lifetimes / null checks.
- 0x00844600 DIFFER(222v222) this / text-bank esi/edi swap.
- 0x008082C0 DIFFER(317v317) comparator-copy scheduling before `sub esp`.
