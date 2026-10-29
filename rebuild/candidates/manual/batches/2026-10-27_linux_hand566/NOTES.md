# Hand chunk 566 (CThingDeadCreature .. CVertexShaderConstantLayout): 11 of 12 byte-exact

All 11 matched rows grade RELOCATION_MATCH (gall.sh, q.py) and pass their behaviour tests under g++ (tall.sh) and the
lane's VC7.1 cl (tall.py). vland dry run (stock, `$S/batch/chunk566_oracle.tsv`): 11 WINS, no RELOC_IDENTITY /
RELOC_CONFLICT / RELOC_DUPLICATE; only RELOC_NAME and SHARED_HEADER_AVAILABLE warnings (below). No retail_asm rows.

## What matched
- 00835b10 CThingDeadCreature::~CThingDeadCreature (120) -- vptr 0x01272B04 / RTTI CThingDeadCreature; scoped_ptr
  checked_delete of the body reorienter, PKiller, PAnimFilter CCountedPointer release, tail ~CThingGameObject 0x00838A00.
- 0085d0a0 NThingMap::CThingMap::~CThingMap (213) -- RTTI `.?AVCThingMap@NThingMap@@` (catalog lacked the namespace).
- 00878f50 NCameraModes::CProjectileWeapon::OnInit (276) -- vtable 0x01277804 slot 21.
- 0088bbe0 CAIBrain::~CAIBrain (199).
- 008b7ba0 CCreatureAction_UnsheatheWeapons::PerformAction (336, incl. the 5-entry jump table, checked entry by entry).
- 008c40f0 CCreatureAction_WaspQueenDiveAttack::FinishAction (119).
- 008cc7a0 CCreatureAction_BalverineCorneredJumpIntoSky::FrameUpdate (120).
- 008cc820 CCreatureAction_BalverineDropInFromSky::FrameUpdate (114).
- 00934870 CAIStateGroup_MinionKidGroupTag::Update (169).
- 009536d0 CCreatureAction_DragonFireBlastBase::~CCreatureAction_DragonFireBlastBase (119) -- implicit dtor (no vptr store).
- 0098d710 CVertexShaderConstantLayout::~CVertexShaderConstantLayout (270) -- implicit dtor (no vptr store).

## Parked
- 0097c5e0 CAIStateGroup_ScorpionKing::Update DIFFER(308v383): the row runs past the function's jump table into three
  other functions; the true extent 0x0097C5E0..0x0097C714 (code + `mov edi,edi` + 5-entry table 0x0097C700) is
  byte-exact with relocations masked (tools/pfx2.py: 308 bytes, 21 relocs, 0 mismatches; table entries
  0x0097C641 / 0x0097C6C3 / 0x0097C641 / 0x0097C6D0 / 0x0097C641 match) | tried: one q.py grade (DIFFER 308v383 only
  from the row extent); pfx2 masked prefix compare | next: re-split at 0x0097C720 (0x0097C720 is a 16-byte ctor storing
  vtable 0x012989D4 = RTTI `.?AVCEventCreatureCharging@NEntityEvents@@`, 0x0097C730 is that vtable's slot 1
  `mov eax,25h ; ret`, 0x0097C740 its slot 0 scalar deleting dtor storing the base vtable 0x0124F2C4
  `.?AVCEventBase@NEntityEvents@@`), then land differ/0097c5e0.cpp as is (test differ/0097c5e0.test.cpp passes g++ + VC).

## Names
### Proven (exact PDB spelling in both Ego_r.pdb and FableWin.pdb name sets; address from a byte-matched call/ref)
Leaves (all confirmed with objdump -t on the compiled object):
- 0x00835B10 `??1CThingDeadCreature@@UAE@XZ`
- 0x0085D0A0 `??1CThingMap@NThingMap@@UAE@XZ`
- 0x00878F50 `?OnInit@CProjectileWeapon@NCameraModes@@UAEXXZ`
- 0x0088BBE0 `??1CAIBrain@@UAE@XZ`
- 0x008B7BA0 `?PerformAction@CCreatureAction_UnsheatheWeapons@@UAEXXZ`
- 0x008C40F0 `?FinishAction@CCreatureAction_WaspQueenDiveAttack@@UAEXXZ`
- 0x008CC7A0 `?FrameUpdate@CCreatureAction_BalverineCorneredJumpIntoSky@@UAEXXZ`
- 0x008CC820 `?FrameUpdate@CCreatureAction_BalverineDropInFromSky@@UAEXXZ`
- 0x00934870 `?Update@CAIStateGroup_MinionKidGroupTag@@UAEXXZ`
- 0x009536D0 `??1CCreatureAction_DragonFireBlastBase@@UAE@XZ`
- 0x0097C5E0 `?Update@CAIStateGroup_ScorpionKing@@UAEXXZ` (parked row)
- 0x0098D710 `??1CVertexShaderConstantLayout@@UAE@XZ`
Callees (both PDB name sets; the ones with a wrong bound spelling are in NAME_FIXES.tsv):
- 0x0088B6F0 `?OnKill@CAIBrain@@QAEXXZ`; 0x00A44620 `??1CSuspendableProcessMicrothread@@MAE@XZ` (protected);
  0x0099A300 `??1CBaseClassNonCopyable@@MAE@XZ`; 0x0099A2E0 `??1CBaseClass@@UAE@XZ`; 0x00A01C10
  `??1CBaseIntelligentPointer@@UAE@XZ`; 0x00A01B50 `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`.
- 0x00838A00 `??1CThingGameObject@@UAE@XZ`; 0x00863790 `??1CThingBodyReorienter@@QAE@XZ`.
- 0x00697AC0 `?HasCameraMode@CGameCameraManager@@QBE_NW4EMode@NCameraModes@@@Z`; 0x00878CE0
  `?SetUpParams@CProjectileWeapon@NCameraModes@@AAEXXZ`.
- 0x006647F0 / 0x00666830 / 0x00664A50 / 0x006649C0 / 0x00664A10 `?SheatheAllWeapons` / `?UnsheatheAllWeapons` /
  `?UnsheatheDefaultWeapon` / `?UnsheatheMeleeWeapon` / `?UnsheatheRangedWeapon` `@CThingCreatureBase@@QAEXXZ`;
  0x00662110 `?MakeNoise@CThingCreatureBase@@QAEXABVCCharString@@@Z`; 0x006A1540
  `?IsThingCarryingWeapon@CTCCarrying@@SI_NABVCThing@@@Z`; 0x0073A840 `?RefreshCurrentMode@CTCCreatureModeManager@@QAEXXZ`
  (not RestartCurrentMode: no PDB has that name; donor order GetCurrentMode / RefreshCurrentMode / FrameUpdate /
  IsModeActive with gaps 0x40 / 0x20 = retail 0x0073A840 / 0x0073A880 / 0x0073A8A0); 0x00859A70
  `?SetupCombatStrikeEvents@CCombatActionBase@@IAEXXZ`; 0x0099EBF0 `??0CCharString@@QAE@PBDJ@Z`; 0x0099EAE0
  `??1CCharString@@QAE@XZ`.
- 0x004C9840 `?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z`; 0x00859A20 `?FinishAction@CCombatActionBase@@MAEXXZ`;
  0x00859D80 `?FrameUpdate@CCombatActionBase@@UAEXXZ`; 0x006925E0 `?GetPercentageComplete@CCreatureActionBase@@QBEMXZ`
  (catalog says CInputTypeMouseMovementEvent::GetMovementRatio: byte-twin propagation); 0x004BFAC0
  `?SetAlpha@CTCGraphicAppearance@NGraphicAppearance@@QAEXE@Z`.
- 0x008FCFA0 `?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ`; 0x0049D870 `?GetFrame@CWorld@@SIJXZ`; 0x008FCFD0
  `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ`; 0x00497890 `?GFROR13@@YIJK@Z`; 0x008138E0
  `?GetTagger@CTCCreatureGroupTag@@QBEPBVCThingAICreature@@XZ`; 0x009341C0 / 0x00934410
  `?TagTarget` / `?EvadeTarget@CAIStateGroup_MinionKidGroupTag@@AAEXXZ`; 0x0097E200
  `?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ`; 0x008FCF40 `?Terminate@CAIStateGroupBase@@QAEXXZ`.
- 0x0062E900 `??1CCombatActionBase@@UAE@XZ` (implicit too: no vptr store).
- Parked-row callees: 0x0097DB70 `?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z`;
  0x008E4770 `??0CCreatureAction_ScorpionRecover@@QAE@AAVCThingCreatureBase@@@Z` (catalog: CCreatureAction_GoUnderground);
  0x007AED10 `?GetBossPhase@CTCBoss@@QBEJXZ`; 0x007AEE90 `?IsReadyForPhaseChange@CTCBoss@@QBE_NXZ`; 0x0097B870
  `?IsStillRunnable@CAIStateGroup_ScorpionKing@@IAE_NXZ`; 0x0097BDA0 `?UpdateCombat@CAIStateGroup_ScorpionKing@@IAEXXZ`;
  0x0097C580 `?UpdatePhase2@CAIStateGroup_ScorpionKing@@IAEXXZ` (donor order, Update - UpdatePhase2 = 0x60 in both).

### Probable
- 0x0040F020 `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@...` -- not in the PDB
  name sets (template instance); the bound naming-pass consensus spelling, used unchanged.
- 0x013BA848 `?g_NoMapwhoLevels@NThingMap@@3JA` -- landed Clear 0x0085CEC0 / GetMemoryFootprint 0x0085CFE0 spelling; no
  PDB name set carries data globals. Second binding `?g_NumMapwhoLevels@@3JA` (from 0x006B2720) -- see Review at landing.
- Inlines from the PDB but with bodies reconstructed from the bytes: CGameCameraManager::SetSingleCut (OldCamera =
  Camera; SingleCut = true), GetCameraMode<T>(T**), CProjectileWeapon::StatGetModeType (0x0F from slot 2 0x0087B5F0),
  GFFloatRandom(float, unsigned long*) (`?GFFloatRandom@@YIMMPAK@Z`), CAIStateGroupBase::GetCreature,
  CThing::GetPhysicsTC, CThing::GetTCGraphicAppearance, CEnginePrimitiveHandle::Clear.
- CTCPhysicsBase slot names 64 GetFacingAngleYZ / 65 GetFacingAngleXZ / 67 SetFacingAngleYZ / 80 ResetInterpolation
  (from the CTCPhysicsStandard bodies and PDB member order; 63 GetFacingAngleXY / 66 SetFacingAngleXY are the ones
  this row calls).

### Descriptive
- `EUnsheatheWeaponType` enumerators UNSHEATHE_WEAPON_TYPE_0..5 (values only; enumerator names not in the dumps).
- `NCameraModes::EMode_ProjectileWeapon = 15` (value from slot 2).
- `CUnprovenSlotNN_<vtable>` placeholders in the CTCBase / CTCPhysicsBase / CEnginePersistentPrimitiveBase models.
- `EmitImplicitDestructor(T*)` helpers in 009536d0 / 0098d710 (emission device only, not retail functions).

## Label corrections
- 0x0085D0A0 class is NThingMap::CThingMap (catalog: CThingMap).
- 0x00878F50 class is NCameraModes::CProjectileWeapon (catalog: CProjectileWeapon).
- 0x0073A840 CTCCreatureModeManager::RefreshCurrentMode (landed label RestartCurrentMode, labels_chunk300_corrections).
- 0x006925E0 CCreatureActionBase::GetPercentageComplete (catalog: CInputTypeMouseMovementEvent::GetMovementRatio).
- 0x008E4770 CCreatureAction_ScorpionRecover ctor (bound as NCreatureMode::CCreatureAction_GoUnderground ctor).
- 0x00863790 ~CThingBodyReorienter (bound ??1CTCMapwho; manifest label "Catch@0047e8cc").
- 0x0088B6F0 CAIBrain::OnKill (bound OnLevelUnload).

## Header debt
engine/CTCBase.h, CCreatureActionBase.h, CIVCountedPointeeBase.h, CAIStateGroupBase.h, CAIBrain.h, CWorld.h (flagged
SHARED_HEADER_AVAILABLE) and CThingGameObject.h, CVertexShaderConstantLayout.h, CGameCameraManager.h all flatten the vptr
into a `void* __vftable` data member and use Ego_r.pdb (Dinkumware-width) offsets; the rows need real polymorphic bases
(virtual dtors / slot calls) and retail 12-byte STLport containers (e.g. CCombatActionBase retail 0x114 vs PDB 0x12C,
CGameCameraManager CurrentMode +0x98 vs PDB +0x9C, CProjectileWeapon PDB - 0x24), so every row uses a local model with
a `// Layout:` comment.

## Review at landing
- 008cc7a0: retail computes alpha `(1.0f - 2p) * 255` past half way (constant 1.0 at 0x0122DED8, read from .rdata), which
  is non-positive there, so the byte wraps (0 just past half way, about 255 falling to 1 at the end). Reproduced as is
  (that is what retail does); the test asserts the wrapped values.
- RELOC_NAME warnings: 0x0073A840 labelled RestartCurrentMode, 0x006925E0 labelled GetMovementRatio -- both labels are
  wrong (NAME_FIXES.tsv); respell the callers.
- 0x013BA848 is bound twice (`?g_NoMapwhoLevels@NThingMap@@3JA` from 0x0085CFE0, `?g_NumMapwhoLevels@@3JA` from
  0x006B2720); one address one name -- respell 0x006B2720's caller when its owner lands (not proven from a PDB, so not in
  NAME_FIXES).
- 0x00BFEA14 (free) and 0x00BFE9BC (operator delete) carry many invented spellings in reloc_bindings; CRT, left out of
  NAME_FIXES.
- 0097c5e0: re-split at 0x0097C720 (above) and land differ/0097c5e0.cpp.
