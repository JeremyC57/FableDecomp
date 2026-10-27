# Hand chunk 508 (CQ_PrisonEscapeRescueMotherScript::CCellDoor .. CHeroTurningDebuggerScript): 29 of 30 byte-exact

All 29 matched rows are RELOCATION_MATCH (no plain MATCH, no retail_asm rows). Behaviour tests PASS under g++ (tall.sh) and
VC7.1 (tall.py). The vland dry run reports 29 WINS, no RELOC_IDENTITY and no DIFFER.

## What matched (29)
- Entity-script Init rows (CTraderToRescue, CMagicman, CBordelloClient, CChickenSign, CEM_Rhodri, CTyler, CGuildMasterGameFlow,
  CLostTraderTrader, CMurderTwistSheriffGuard, CMurderTwistAssassin): flag resets in source order, then interface calls. Shape
  lessons: float zeros compile to integer stores; `ID == 0 ? "_WEAK" : "_TOUGH"` gives retail's `mov eax,A ; je` (the `!= 0`
  form flips the operands); a by-value `SetIsPushableByHero(Me, ..)` needs CScriptThing's user copy constructor.
- Level-script Init rows (CV_AmbushScamScript, CV_ArcheryCompetitionScript, CV_ChapelOfEvilScript): member resets in Ego_r order.
  The inline CTimer::operator=(long) (g_pGameScriptInterface slot 89 SetTimer) must be declared after CGameScriptInterface.
- OnPersist rows (CV_AssassinAttacksScript, CV_TalentlessBardScript): two-argument CPersistContext::Transfer with an inline
  GetDefaultVal<T> (long / unsigned long / bool stores 0 in the default slot); TransferVector<bool> 0x00CDCF80 over an STLport
  20-byte bit vector.
- OnPredicateFail rows (CBodyGuard, CHHUndead, CRandomPopulationCreature, CManInLove, CTH_SnowTroll): `Me.MsgIsKilledBy("")`
  with a CCharString temporary. `!GotReward && Me.MsgIsKilledBy("")` gives the conditional temporary with an ebx flag.
- Wait-loop Main rows (CCellDoor, CCampHostageDoor, CFishermanBulletinBoard, CTOL_AltarGood, CTH_Trigger_NorthernWastes) and
  the spawned functions (CQR_EscortTraderScript::WatchForRegionLoads, CV_TravellingHeroesScript::ThunderChatter). A single-test
  wait whose test builds a temporary is
  `do { if (used) { if (term) return; act; return; } frame; } while (!term);` (0x00DD8FD0). A plain `while (cond)` gives the
  rotated test / body / test shape.
- CEmptyScript::Main and CHeroTurningDebuggerScript::Main: the landed CQR_EscortTraderScript::Main CSpawnedFunc template.
  `SpawnFunction(new CSpawnedFunc<T>("Name", 0, this, &T::F))`. The name string is a conditional temporary, which gives the
  entry-time zero store.

## Parked for Fable 5.1 (1)
- 00ee82d0 CCS_OakValeRevisitedScript::Main DIFFER(172v181): retail opens with `mov dword ptr [esp+8],0` (8 bytes, a dead zero
  store into the one local slot the "OakBay" temporary later uses) plus a 1-byte `nop` that aligns the loop head. The rest is
  identical. | tried: plain `while (!Game->IsRegionLoaded("OakBay"))` (172v181); a `true &&` guard (keeps live flag code,
  171v181); a dead `|| false && ...` temporary (C1001 ICE) | next: find which dead conditional temporary leaves only the
  flag's entry-time init behind. Rows 29/30 show the same store next to a real conditional temporary. Candidates are a
  `new`-expression or ternary that the backend removes.

## Skipped
None.

## Names
### Proven
- 00dd8fd0 NScript::CQ_PrisonEscapeRescueMotherScript::CCellDoor::Main (entity vtable slot 1, landed Alloc stores the vtable)
- 00dfb1b0 NScript::CQ_TraderConflictGoodScript::CTraderToRescue::Init
- 00dfc050 NScript::CQ_TraderConflictGoodScript::CCampHostageDoor::Main
- 00e1e470 NScript::CQR_EscortTraderScript::WatchForRegionLoads (Func of the "WatchForRegionLoads" CSpawnedFunc in the landed Main 0x00E1E110)
- 00e2abc0 NScript::CV_AmbushScamScript::Init (level slot 3; registration "S_VAS", Alloc size 0x94)
- 00e323b0 NScript::CV_ArcheryCompetitionScript::Init
- 00e39780 NScript::CV_AssassinAttacksScript::OnPersist (level slot 4; "S_VAA", size 0xB8, keys = Ego_r member names)
- 00e3b060 NScript::CV_BordelloScript::CMagicman::Init
- 00e3b1a0 NScript::CV_BordelloScript::CBordelloClient::Init
- 00e5fb80 NScript::CV_BodyGuardScript::CBodyGuard::OnPredicateFail (binding "BodyGuard", Alloc 0x00E5DD60)
- 00e68dc0 NScript::CV_ChickenKickingScript::CChickenSign::Init
- 00e68fa0 NScript::CV_ChapelOfEvilScript::Init ("S_VCOE", size 0x90)
- 00e766a0 NScript::CV_ExposeMayorScript::CEM_Rhodri::Init
- 00e7bca0 NScript::CV_FishermanScript::CFishermanBulletinBoard::Main
- 00e836f0 NScript::CV_FisticuffsClubScript::CTyler::Init
- 00e909a0 NScript::CV_GuildMasterScript::CGuildMasterGameFlow::Init
- 00e95dc0 NScript::CV_HauntedHouseScript::CHHUndead::OnPredicateFail
- 00ea35a0 NScript::CV_LostTraderScript::CLostTraderTrader::Init
- 00eb2070 NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::Init (binding "MurderTwistSheriffGuard", Alloc 0x00EB1F80, ctor 0x00EB2000)
- 00eb3e40 NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Init
- 00ec1c20 NScript::CV_RandomPopulationSimScript::CRandomPopulationCreature::OnPredicateFail (vtable 0x012F1390 slot 5, Alloc 0x00EC1B70)
- 00ec8230 NScript::CV_SickChildScript::CManInLove::OnPredicateFail (vtable 0x012F1A94 slot 5, Alloc 0x00EC8160 news 0x20 = Ego_r CManInLove size)
- 00ed85a0 NScript::CV_TalentlessBardScript::OnPersist (vtable 0x012F3428 slot 4, landed ctor 0x00ED76E0; 8 keys = Ego_r member names)
- 00edc390 NScript::CV_TempleOfLightScript::CTOL_AltarGood::Main (vtable 0x012F3EB0 slot 1, landed Alloc 0x00EDC2D0)
- 00ee0350 NScript::CV_TravellingHeroesScript::CTH_SnowTroll::OnPredicateFail (vtable 0x012F42B0 slot 5, landed ctor 0x00EE02C0)
- 00ee29d0 NScript::CV_TravellingHeroesScript::ThunderChatter (Func of the "ParentClass." + "ThunderChatter" CSpawnedFunc in CTH_Thunder::Main 0x00EE2490; Ego_r ?ThunderChatter@CV_TravellingHeroesScript@NScript@@QAEXXZ)
- 00ee3fc0 NScript::CV_TravellingHeroesScript::CTH_Trigger_NorthernWastes::Main (vtable 0x012F4438 slot 1, landed Alloc 0x00EE1AF0)
- 00ee82d0 NScript::CCS_OakValeRevisitedScript::Main (parked; vtable 0x012F53C0 slot 2, landed ctor 0x00EE8100)
- 00eee800 NScript::CEmptyScript::Main (vtable 0x012F5954 slot 2, landed ctor 0x00EEE730)
- 00efc260 NScript::CHeroTurningDebuggerScript::Main (vtable 0x012F6C58 slot 2, landed ctor 0x00EFC190)
- callee 00eee8b0 NScript::CEmptyScript::Bum (spawn name "Bum" 0x012F5984 + Ego_r ?Bum@CEmptyScript@NScript@@QAEXXZ)
- callee 00efc300 NScript::CHeroTurningDebuggerScript::MoveHeroTest (spawn name "MoveHeroTest" 0x012F6C88 + Ego_r)
- callee 00e241b0 NScript::CIsThingAlive::~CIsThingAlive (stores vtable 0x012C32F0; Ego_r ??1CIsThingAlive@NScript@@UAE@XZ)
- callee 00cdcf80 CPersistContext::TransferVector<bool> (reloc_bindings)
- data pGlobalScriptDef retail offsets: IngredientObject +0x738 (Ego_r +0x794), KilledSomeoneForMushroom +0x768 (Ego_r +0x7c8,
  4 less after the SickChildsSisterAttentionAnims vector), TH_ThunderConv_Length +0x928 (Ego_r +0x9ac; the landed
  DoConversation reads the vectors after it at +0x92c).

### Probable
- callee 00ee8390 NScript::CCS_OakValeRevisitedScript::OakVale: Ego_r lists Alloc, Main, OakVale, OakValeFire and RegisterMain
  for the class. ctor 0x00EE8100, RegisterMain 0x00EE8210 and OakValeFire 0x00EE8870 are landed, Main is 0x00EE82D0, so
  0x00EE8390 is OakVale by elimination.
- callee 00415d70 CDefString::operator CCharString() (non-const) and 00415dd0 operator CCharString() const: the bodies are
  byte-identical twins. The landed 0x008E7040 binds 0x00415D70 non-const and 0x00415DD0 const. The landed 0x0071C7C0
  reaches 0x00415DD0 through a const path. 0x00EC8230 reaches 0x00415D70 through the non-const `CScriptDef*` pGlobalScriptDef.
  Ego_r keeps only the const spelling, so the non-const symbol is not PDB-confirmed.

### Descriptive
None.

## Label corrections
- 0x00E76540, landed as "CEM_Rhodri::Init", is CEM_Sheriff::Init: CEM_Sheriff::Alloc 0x00E76470 stores its vtable 0x012E98B4.
  The real CEM_Rhodri::Init is 0x00E766A0.
- 0x00E5FC70 GetHenchmanGoldCost (descriptive) -> CV_BodyGuardScript::CBodyGuard::GetGoldCost (Ego_r ?GetGoldCost@CBodyGuard@...@@AAEJXZ).
- 0x00E376D0 CInputProcessManager::CInputProcessManager -> NScript::CV_AssassinAttacksScript ctor (stores vtable 0x012E35B0).
- 0x00E5DDE0 CDarkwoodTrader ctor -> NScript::CV_BodyGuardScript::CBodyGuard ctor.
- 0x00E64D90 CACompressor ctor -> NScript::CV_ChickenKickingScript::CChickenSign ctor.
- 0x00412B20 vector<pair<CCharString,long>>::resize -> std::vector<CWideString>::resize.
- Stand-in CVtable_012E2428 / CVtable_012E8080 / CVtable_012efee8 constructors are CV_AmbushScamScript, CV_ChapelOfEvilScript
  and CMurderTwistSheriffGuard.
- 0x00E241B0 CScriptThingFunctor_00e241b0::~CScriptThingFunctor_00e241b0 -> NScript::CIsThingAlive::~CIsThingAlive.
- 0x00EFC300 Script_MarkerToFace_Thread -> NScript::CHeroTurningDebuggerScript::MoveHeroTest.
- 0x00EE02C0 "CTH_SnowTroll::CTH_SnowTroll" should carry its namespace: NScript::CV_TravellingHeroesScript::CTH_SnowTroll::CTH_SnowTroll.
- 0x00EEE7F0 and 0x00EFC250 are labelled CSpawnedFunc<NScript::CQ_RansomVictimChiefsHouseScript>::SuspendableProcess, but
  they are slot 4 of the CSpawnedFunc vtables 0x012F5968 (CEmptyScript) and 0x012F6C6C (CHeroTurningDebuggerScript): the
  label was propagated across byte-identical bodies.
- 0x0099B800 CCharString::AssignFromWide is bound here as CWideString::operator=(const wchar_t*) (??4CWideString@@QAEABV0@PB_W@Z).
  vland reports RELOC_NAME for it; check it at landing.

## Header debt
- All rows use local polymorphic models of CGameScriptInterface (gsi slot declarations), CScriptThing, CScriptBase /
  CLevelScriptBase and CEntityScriptBase, the same models agents 499-501 used. CSpawnedFuncBase (rows 29/30) is also local.
  vland reports SHARED_HEADER_AVAILABLE (27 warnings) for them.
- CScriptDef is modelled as padding plus the touched members at retail offsets. The CDefString and STLport vector<bool>
  models are local.
