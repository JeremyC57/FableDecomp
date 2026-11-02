# Hand chunk 711 (CTCFamilyMember .. CCharString): 10 of 10 byte-exact

All ten rows grade RELOCATION_MATCH (gall.sh, and the read-only vland check: 10 wins, all behaviour tests PASS). Tests pass
under g++ (tall.sh) and VC7.1 (tall.py). Every extent was checked against the int3 padding or the next function: no extent
fixes. Each catalog label in this chunk except CCharString::Insert and LSocketServer::CloseSocket (which lacked its namespace)
was a stand-in; every row's identity is proven from two sources (callers / vtable + PDB name sets).

## What matched (10)
- **CTCFamilyMember** 0x00778D20 `??1CTCFamilyMember@@UAE@XZ` (catalog "CTCInventoryItem::OnDie"): the compiler-generated
  destructor -- the only caller is ??_G 0x00778D00, slot 0 of vtable 0x0126AADC (RTTI CTCFamilyMember); neither PDB has a user
  ??1. Body = STLport ~vector<const CThing*> of the Ego_r member FamilyMembers (+0x0C), then tail ~CTCBase. Shape: define the
  class's constructor in the TU so the vtable, ??_G and the implicit ??1 are emitted; real STLport 4.6 _Vector_base text.
- **CTCFlashOnHit** 0x007E3420 `??1CTCFlashOnHit@@UAE@XZ`: `if (ParentThing && !ParentThing->IsDead())
  ParentThing->GetTCGraphicAppearance()->RemoveTintColour(this);`. Shape: the plain member test; the PDB template
  IS_THING_ALIVE (by reference) keeps the vtable store ahead of the ParentThing load (42v42).
- **std (CRandomAppearanceMorph texture-morph maps)** 0x0086D090 `__uninitialized_fill_n(..., const __false_type&)` (ret 8,
  caller _M_insert_overflow 0x0086E1C0) and 0x0086D570 the three-argument `uninitialized_fill_n` front end (ret 4, caller
  _M_fill_insert 0x0086E4B0): byte near-twins named each from its own caller. STLport 4.6 text, /Gr like the TU's landed rows.
- **CEngineLandscapeRenderer** 0x00B671A0 `?SetupBackgroundStates@CEngineLandscapeRenderer@@AAEXXZ` (catalog
  "UpdateLighting"): `g_pLightingManager->SetEnableFadedShadowFactor(true); g_pLightingManager->UpdateGlobalLightingStates();`.
  Identity from the Ego_r donor order (CreateMap / DeleteMap / SetupBackgroundStates / .. / CalcForegroundFadeFactors =
  0xB67140 / 0xB67180 / 0xB671A0 / 0xB671E0) and the FableWin inline setter. Shape lesson: 28 caller-side forms failed
  (53..92 bytes); the key was a disp32 0x4694 byte scan that found the setter's out-of-line copy 0x00B66EB0 (0 callers),
  whose instructions fixed the setter's own text (reassign the by-value `enable` when the mode is not 1, then compare /
  store / `StateUpdateFlags |= 1`); with that setter inlined the caller is just `(true)`.
- **NScript::CV_FisticuffsClubScript** 0x00E827A0 `?SetCompletedThisClub@CV_FisticuffsClubScript@NScript@@QAEXXZ` (FableWin;
  catalog "CScriptBase_InitialiseVariables_Check"): clear CompletedThisPunchClub, switch on RelevantRegion (1, 4, 2, 3, default
  return), set it when the club flag is set and the thread is not terminating. Jump table verified with objdump -r and by vland.
  Shape: all Ego_r members restated at Ego_r - 0x14; class-typed members as `unsigned long X_[n]` (unsigned char arrays
  misaligned the later members).
- **CFileInstaller** 0x009C4420 `?BeginDecompressionThread@CFileInstaller@@KAXPAX@Z` (Ego_r; catalog
  "CFileInstaller_ThreadEntry"): the _beginthread start routine (0x009C5A00) -- normal priority, then
  DecompressionThreadMain 0x009C4230. Full Ego_r layout, sizeof 0xA0 asserted.
- **CCodeSectionManager** 0x0122A940 `_$E2` (catalog "DynamicInit_013caa54_58"): the "`dynamic atexit destructor for 'desc''"
  of GetSectionDesc's function-local `static CSectionDesc desc;` (both PDBs). The TU is GetSectionDesc itself; the leaf is the
  compiler's thunk.
- **LHNetworkLib::LSocketServer** 0x0090AA10 `?CloseSocket@LSocketServer@LHNetworkLib@@QAEXPAVLSocket@2@@Z` (Ego_r):
  `it = std::find(ListOfConnectedSockets.begin(), .end(), socket); ListOfConnectedSockets.remove(*it);` (STLport __find).
- **CCharString** 0x0099F3D0 `?Insert@CCharString@@QAEXJD@Z` (both PDBs): MakeOwnCopyOfDataOrEmptyString(), then a
  block-scope `char text[2] = c, 0` passed to CBasicString<char>::Insert. Shape: the block scope lets VC7.1 home the buffer
  in c's dead argument slot (function scope: 44v42).

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x00778D20 "CTCInventoryItem::OnDie" -> CTCFamilyMember::~CTCFamilyMember (implicit).
- 0x0086D090 / 0x0086D570 "_Fill_n<unsigned long*,...>" -> std::__uninitialized_fill_n / std::uninitialized_fill_n over
  CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph>.
- 0x00B671A0 "UpdateLighting" (and binding ?UpdateLighting@CEngineLandscapeRenderer@@QAEXXZ) -> SetupBackgroundStates (private).
- 0x00E827A0 "CScriptBase_InitialiseVariables_Check" -> NScript::CV_FisticuffsClubScript::SetCompletedThisClub; the landed
  InitialiseVariables 0x00E82650 calls it as "InitialiseVariablesCheck".
- 0x009C4420 "CFileInstaller_ThreadEntry" -> CFileInstaller::BeginDecompressionThread; 0x009C4230 = DecompressionThreadMain.
- 0x0122A940 "DynamicInit_013caa54_58" -> _$E2 (atexit destructor for 'desc'); static 0x013CAA54 is `desc`, not `s_empty`;
  GetSectionDesc 0x009F0240 is private AAE returning a non-const reference; CSectionDesc +0x0C is `Size`, not `Handle`.
- 0x0090AA10 / 0x0090B8E0 / 0x008F8C70: the LSocket family is in namespace LHNetworkLib (Ego_r); 0x0090B8E0's catalog
  "list<CTCBase*>::remove" is list<LHNetworkLib::LSocket*>::remove.
- 0x004BFAE0 "NParticleEngine::CParticleSystem::Kill" -> NGraphicAppearance::CTCGraphicAppearance::RemoveTintColour(void*).
- 0x0099EB40 MakeOwnCopyOfDataOrEmptyString is protected (IAE); 0x009A06B0 CBasicString<char>::Insert takes `char*` (KPAD).
- 0x00CB7940 is CScriptBase::IsActiveThreadTerminating (global CScriptBase); several derived-class / _Retail spellings bound.
All with exact spellings in NAME_FIXES.tsv.

## Decisions for the coordinator
- 0x01436E9C (the engine lighting manager pointer): no data symbol in either PDB name set, so the name is not proven. The
  source uses the bound `?g_pLightingManager@@3PAVCEngineLightingManager@@A`; eight variants are bound to this address
  (g_pLightingManager / g_pEngineLightingManager / g_pMistParams / ..._01436e9c). Pick one spelling fleet-wide.
- 0x00B66EB0 is an uncatalogued out-of-line copy of CEngineLightingManager::SetEnableFadedShadowFactor (0 callers, the
  FableWin inline QAEX_N): a free authorable row (`?SetEnableFadedShadowFactor@CEngineLightingManager@@QAEX_N@Z`).
- 0x009C5870 is the uncatalogued sibling CFileInstaller::BeginThread (Ego_r ?BeginThread@CFileInstaller@@KAXPAX@Z: priority
  1, tail ThreadMain 0x009C5690).

## Header debt
- engine/CEngineLightingManager.h and engine/CEngineLandscapeRenderer.h are flattened (vptr as data); used through the
  layout-rename pattern. Both want a real polymorphic retail branch.
- fable_string.h: CCharString / CCharStringData carry no PDB member names (text / unknown04 / owners vs PData / StringLength /
  DataLength:31 + UseFastExtend:1 / NoRefs) and no protected MakeOwnCopyOfDataOrEmptyString; 0x0099F3D0 restates them locally.
- fable_file_installer.h: CFileInstaller is an opaque 0xA0 block without its CTBaseSingleton vptr; its
  `basicInstallStartupFlag` +0x81 is the Ego_r member `IgnoreTimeStampWhenCopying`. 0x009C4420 restates the full Ego_r layout.
- No engine/CThing.h: 0x007E3420 restates CThing over the CClass retail branch with every Ego_r member (sizeof 0x98).
- No shared header for LSocketServer, CCodeSectionManager, CV_FisticuffsClubScript, CTCFamilyMember, CVectorMap.
