# Hand chunk 576 (NScript CQ_AmbushTradersScript .. CV_BookCollectingScript): 10 of 10 byte-exact

All ten rows grade RELOCATION_MATCH (gall.sh) and pass their tests under g++ and the lane's VC7.1 (tall.sh / tall.py). The
read-only vland run reports 10 RELOCATION_MATCH + PASS, no RELOC_IDENTITY; its one remaining warning is a catalog label
(RELOC_NAME 0x00CB8930 labelled `CTCVillage::OnInitialActivate`, really CScriptBase::PostAddScriptedEntities; NAME_FIXES.tsv).
No retail_asm, no flags, no pragmas. Every string literal was paired with the retail address at the same offset (tmp/lit.py).

## What matched (10)
- **Entity-script `Main` (slot 1) rows (3):**
  - 0x00D86A30 NScript::CQ_HeroSoulsGuildmasterScript::CHSG_DistanceChecker::Main: two rotated waits on
    IsDistanceBetweenThingsUnder(Me, hero, (float)pGlobalScriptDef->HS_GMLongDistance / HS_GMShortDistance), setting the
    quest's DistanceReached to 1, then 2. First grade (the fild of the global before the prologue pushes is ordinary scheduling).
  - 0x00D00440 NScript::CQ_AwakeningTheOracleScript::CLostBayCutsceneTrigger::Main: unless PlayedLostBayCS, persist Me,
    SetHeroAge(GetHeroAge() + 1.0f), PlayCutscene("CS_LOSTBAY_INTRO", true) (by-value CCharString built in its slot),
    Pause(10), a one-line conversation and its wait; RemoveThing(Me, false, true) tail. First grade.
  - 0x00DD8DE0 NScript::CQ_PrisonEscapeRescueMotherScript::CCellDoorHero::Main: untargetable until PrisonKeyStolenByHero,
    then targetable; the used-by wait is a `do { if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) { ...success...; return; }
    NewScriptFrame(); } while (!IsActiveThreadTerminating());` -- `while (!used)` and `for (;;) { if (used) break; }` both rotate
    the loop (test duplicated, 309v259 / 298v259). Four grades.
- **Level-script `Init` / `OnPersist` (3):** 0x00E4F620 CV_BeardyBaldyScript::Init (flag stores + 13 CCharString::operator=
  in retail's store order; the landed CV_ArcheryCompetitionScript::Init model), 0x00E37520 CV_ArcheryCompetitionScript::OnPersist
  (ten Transfer<bool>), 0x00E54880 CV_BookCollectingScript::OnPersist (Transfer<bool>/<long> + two TransferVector<bool> on
  12-byte STLport vectors). All first grade from the landed models.
- **Level-script `Main` / threads (4):**
  - 0x00E54450 CV_BookCollectingScript::Main: region wait, then the CEntityScriptBinding new-expression for "BS_Teacher" with
    the landed CBS_Teacher::Alloc, PostAddScriptedEntities twice (the landed CV_RockTrollFirstEncounterScript::Main model).
  - 0x00EEEC00 CTestTheInventoryScript::TestForLampOn: the landed twin TestForLampOff with the wait negated; the CSpawnedFunc
    new-expression with a `__forceinline` ctor. First grade.
  - 0x00DA24F0 CQ_MinionCampScript::RunChantTimer: CTimer inline operator= / operator>= / GetTime over the global interface
    0x0143E8F8, the IF-macro arms each opening with a termination test, `if (!ChantTimerPaused) {..} else {..}` arm order
    (the positive test lays the arms swapped, 272v276), trailing termination test as a tail jmp. Two grades.
  - 0x00F088E0 CQ_AmbushTradersScript::EntityCountDecrease(CCharString): `if (== trader) { --TraderAliveCount } else if (guard
    || red || black || crossbow) { --GuardAliveCount }`, `++MasterScriptData->AmbushTradersKillCount`. The retail null test
    is threaded once through all five compares (two compare chains, PData cached in esi): that needs Compare's body in the TU
    under `#pragma auto_inline(off)` (the landed 0x004FDCB0 trick); declared only, every compare reloads the argument (355v325).

## Names and their evidence (all proven; none descriptive)
- Entity Mains: each row is referenced once, from slot 1 of the entity vtable its landed Alloc stores -- 0x012D4C04
  (CHSG_DistanceChecker::Alloc 0x00D86980), 0x012C7C5C (CLostBayCutsceneTrigger::Alloc 0x00CFC7D0), 0x012DC4F4
  (CCellDoorHero::Alloc 0x00DD8D20); Ego_r.pdb `?Main@<entity>@<quest>@NScript@@EAEXXZ` (private virtual), each right before
  its Alloc in the donor order. Note the PDB spelling CHSG_DistanceChecker (underscore).
- Level-script slots (0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist) of the vtables the landed ctors store: 0x012E5790
  slot 3 (CV_BeardyBaldyScript ctor 0x00E4F5B0), 0x012E2EE4 slot 4 (CV_ArcheryCompetitionScript ctor 0x00E32350), 0x012E5EFC
  slots 2 and 4 (CV_BookCollectingScript ctor 0x00E54310); Ego_r.pdb Init / Main / OnPersist `UAE` names for all three classes.
  Every persisted name string equals the Ego_r member at that retail offset.
- 0x00EEEC00: member pointer at +0x34 beside "TestForLampOn" in the landed Main 0x00EEEA90 and TestForLampOff 0x00EEED20;
  Ego_r ?TestForLampOn@CTestTheInventoryScript@NScript@@QAEXXZ 0x00F69770 right before TestForLampOff 0x00F698F0.
- 0x00DA24F0: member pointer of a spawned thread at 0x00DA1AD1 (CQ_MinionCampScript code); Ego_r PauseChantTimer 0x00DC9570
  (0x60 bytes) -> RunChantTimer 0x00DC95D0 matches retail PauseChantTimer 0x00DA2490 (0x60 bytes) -> this row; it consumes
  exactly PausedTime / ChantTimerPaused, which PauseChantTimer writes.
- 0x00F088E0: sole caller the landed WatchForEnemyDeath 0x00F08590 (passes the def name by value); the only by-value
  CCharString member of CQ_AmbushTradersScript in Ego_r is EntityCountDecrease, and FableWin.pdb places it right after
  WatchForEnemyDeath (0x0362FB70 -> 0x0362FE20).
- CScriptDef members (0x00D86A30): retail +0xc90 / +0xc94 = Ego_r HS_GMShortDistance 0xd40 / HS_GMLongDistance 0xd44. The Ego_r
  layout is 0xa8 larger before FHPatterns (FHPadRadius 0xcb8 -> retail 0xc10, landed FireHeart rows) and 0xb0 after the two
  Dinkumware vectors FHPatterns / FHTimeLimit (OVBRBanditSightRadius 0xe0c -> 0xd5c, landed). The landed CWaitingGuardInWoods::Main
  0x00D86680 reads +0xc94 / +0xc98 under descriptive names: they are HS_GMLongDistance / HS_GMGuardDistance.
- Callees (Ego_r spellings, checked with objdump -t): CEntityScriptBase::IsActiveThreadTerminating `QBE` 0x00F35B30,
  CScriptBase::IsActiveThreadTerminating `QAE` 0x00CB7940 / AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities
  0x00CB8930 / AddSpawnedFunction 0x00CB7E50, IsDistanceBetweenThingsUnder `YI_NABVCScriptThing@@0M@Z` 0x00CBE2FF,
  PlayCutscene `?PlayCutscene@CQ_AwakeningTheOracleScript@NScript@@QAEXVCCharString@@_N@Z` 0x00CFF9A0, CCharString ctor /
  copy / dtor / operator=(const char*) (`??4CCharString@@QAEABV0@PBD@Z`), CBasicString<char>::Compare `KIJPBD0@Z` 0x00411570,
  ??$Transfer@_N@ / ??$Transfer@J@ / ??$TransferVector@_N@CPersistContext (Ego_r exact), CSpawnedFuncBase ctor 0x00CDD450,
  CBS_Teacher::Alloc 0x00E54580 (landed spelling). CTimer::GetTime / operator= / operator>= are Ego_r members (inlined here).
- Interface slots (virtual, not relocated) from ghidra_out/cgsi_slots_aligned.tsv: 7 NewScriptFrame, 12 IsRegionLoaded,
  64 PostAddScriptedEntities, 70 GetHero, 89 SetTimer, 90 GetTimer, 108 RemoveThing, 111 ShowOnScreenMessage, 170 SetHeroAge,
  171 GetHeroAge, 185 IsHeroHandLampLit, 276 ActivateQuest, 332 UpdateQuestInfoBar, 364 AddNewConversation, 366
  AddLineToConversation, 368 IsConversationActive, 376 Pause, 425 OpenDoor, 469 EntitySetTargetable, 477 SetThingPersistent;
  CScriptThing slot 20 MsgIsUsedBy.
- Globals: pGlobalScriptDef 0x0143E90C (Ego_r); 0x0143E8F8 spelled `?g_pGameScriptInterface@@3PAVCGameScriptInterface@@A` as
  chunk 573 chose (no PDB data symbol: descriptive, the binding table holds several spellings).

## Parked for Fable 5.1 (0)

## Skipped
None (every extent checked: first and last instruction as the brief says, one or two `ret`s, no int3 inside).

## Label corrections
- 0x00F088E0 `CScriptBase_AmbushTradersFilter` -> NScript::CQ_AmbushTradersScript::EntityCountDecrease
- 0x00D00440 `sub_00d00440` -> NScript::CQ_AwakeningTheOracleScript::CLostBayCutsceneTrigger::Main
- 0x00E4F620 `sub_00e4f620` -> NScript::CV_BeardyBaldyScript::Init
- 0x00D86A30 `sub_00d86a30` -> NScript::CQ_HeroSoulsGuildmasterScript::CHSG_DistanceChecker::Main
- 0x00DD8DE0 `sub_00dd8de0` -> NScript::CQ_PrisonEscapeRescueMotherScript::CCellDoorHero::Main
- 0x00E37520 `sub_00e37520` -> NScript::CV_ArcheryCompetitionScript::OnPersist
- 0x00E54880 `sub_00e54880` -> NScript::CV_BookCollectingScript::OnPersist
- 0x00EEEC00 `sub_00eeec00` -> NScript::CTestTheInventoryScript::TestForLampOn
- 0x00DA24F0 `sub_00da24f0` -> NScript::CQ_MinionCampScript::RunChantTimer
- 0x00E54450 `sub_00e54450` -> NScript::CV_BookCollectingScript::Main
- 0x00CB8930 catalog `CTCVillage::OnInitialActivate` -> CScriptBase::PostAddScriptedEntities (vland RELOC_NAME).

## Header debt
- engine/CBaseObject.h and engine/CSpawnedFuncBase.h are flattened (vptr as a data member), so 0x00E54880 and 0x00EEEC00 model
  CBaseObject / CSpawnedFuncBase locally as polymorphic classes (`// Layout:` line naming the header; vland's
  SHARED_HEADER_AVAILABLE warning cleared). CScriptBase / CLevelScriptBase / CGameScriptInterfaceBase / CEntityScriptBase /
  CScriptThing are local models as in every landed quest row (no or flattened headers).
- CScriptDef has no engine header; 0x00D86A30 models the three HS_GM* longs at their retail offsets.
- CQ_MinionCampScript is modelled on CLevelScriptBase<CQ_SunnyvaleMasterData> (Ego_r: first member at 0x5c = the 0x5c-byte
  CLevelScriptBase, not the 0x58-byte CLevelScriptNoMasterBase the landed PauseChantTimer comment names).
