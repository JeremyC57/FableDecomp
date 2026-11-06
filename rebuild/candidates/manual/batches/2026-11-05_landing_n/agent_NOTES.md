# Hand chunk 787 (quest-script Mains + one Init): 6 of 6 byte-exact

All six rows are RELOCATION_MATCH under VC7.1 RTM 3077 with the final hdr/ overlay. Each has a g++ and VC7.1 behaviour
test (PASS), and vland reports 6 WINS (tmp/vland.log before cleanup). String-literal (??_C) order was checked by hand for
all six rows, because reloc_identity skips string relocations: 0 out of order.

Every source defines FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_NSCRIPT, FABLE_RETAIL_SUSPENDABLE_PROCESS and
FABLE_RETAIL_BASE_OBJECT, and follows the landed CQ_PrisonEscapeRescueMotherScript::Main (0x00DD2CE0) model:
- each binding is `AddEntityScriptBinding(new CEntityScriptBinding<Q>("Name", this, &Q::CName::Alloc, flag))`;
- then `PostAddScriptedEntities(); Game->PostAddScriptedEntities();`;
- each thread is `AddSpawnedFunction(new CSpawnedFunc<Q>("Name", 0, this, &Q::Fn), CCharString(""))`;
- each wait is a WAIT_UNTIL loop: `while(!cond){ Game->NewScriptFrame(); if(IsActiveThreadTerminating()) return; }`, then
  `if(IsActiveThreadTerminating()) return;`.

Nested entity classes are declared in the source with a one-line macro (class CQ_X::Name : public CEntityScriptBase, with
a static __fastcall Alloc). Their Alloc symbols are proven by the binding-name literal plus the PDB entry
?Alloc@<Name>@CQ_X@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@NScript@@@@AAV23@PAVCScriptDataBase@3@ABVCScriptThing@@@Z.

## 0x00D038E0 NScript::CQ_BanditCampBossBattleScript::Main (1 attempt)
- Shape:
  - 14 bindings, with flags 1, then 0 (BCGameMaster), then 1 x12;
  - PostAdd x2;
  - 7 threads;
  - CacheMusicSet(MUSIC_SET_CUTSCENE_TWINBLADE=48 / THERESA_01=49 / THERESA_02=50), from engine/EMusicSetType.h.
- Threads. Each name is proven by a PDB name-set entry plus exactly one reference, from this Main, paired with its literal:
  - 0x00D109F0 BanditKingMissionProcess
  - 0x00D04770 WatchForBanditCampGates
  - 0x00D0FE80 ProcessHostageCutscenes
  - 0x00D0EE70 CheckForFirstAreaMassacre
  - 0x00D0F640 CheckForSecondAreaMassacre
  - 0x00D043A0 CheckForSecondAreaDoorHelp
- Alloc: 0x00D0B2D0 CQ_BanditCampBossBattleScript::CCROWDBANDITS::Alloc.
- Labels to correct: 0x00D04770, 0x00D0EE70, 0x00D0F640 and 0x00D043A0 (see LABELS.tsv). 0x00D109F0, 0x00D0FE80 and
  0x00D0B2D0 are unlabelled.
- Header: hdr/engine/CQ_BanditCampBossBattleScript.h, an additive default-path change (no landed includers).

## 0x00DA34B0 NScript::CQ_MinionClifftopChaseScript::Main (1 attempt)
- Shape:
  - 20 bindings, all flag 1;
  - SetQuestCardObjective(GetActiveQuestName(), "TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_01", "BowerstoneJail",
    "BowerstoneSlums");
  - WAIT on IsRegionLoaded("BowerstoneJail");
  - OverrideMusic(MUSIC_SET_DANGER, false, false);
  - threads ProcessMission (0x00DA8510) and WatchForTermination (0x00DA3F40).
- Labels to correct:
  - 0x00DA3F40 is labelled ...::ProcessMission, but the "WatchForTermination" literal is paired with it;
  - the catalog name for 0x00DA8510 lacks NScript::.
- Header: new hdr/engine/CQ_MinionClifftopChaseScript.h (sizeof 0x6c).

## 0x00DE0850 NScript::CQ_RansomVictimScript::Main (1 attempt)
- Shape:
  - 10 bindings (NPCComment flag 0);
  - threads WatchForTermination (0x00DE14A0), WatchForNewRegion (0x00DE1770) and DisappearFollowers (0x00DE4710);
  - KnotholeGlade WAIT, then GossipSetup() (0x00DE1280);
  - `CScriptThing gate = Game->GetThingWithScriptName("KG_Gate"); Game->OpenDoor(gate);`;
  - objective 1, then WAIT QuestStartScreened;
  - ActivateQuest x2 and objective 2, then WAIT RescuedVictim;
  - DeactivateQuest("Q_RansomVictimStartedNature", 0), SetCreatureGeneratorsEnabled(Witchwood2/3/4, false), objective 3;
  - `if(!RansommerDied){ if(T) return; ActivateQuest("Q_RansomVictimNaturesRevenge"); }`, then WAIT RansommerDied;
  - ActivateQuest("Q_RansomVictimRevenge"), DeactivateQuest(NaturesRevenge, 0), `ActiveEnemyName = "RansomRevenge";`.
- Descriptive name: the local `gate` (DESCRIPTIVE; it is a stack temporary with no retail name).
- Labels to correct:
  - 0x00DE1770 is labelled CV_ChapelOfEvilScript::EndMission;
  - 0x00DE4710 is labelled Script_Witchwood_Cavern_Load.
- Header: hdr/engine/CQ_RansomVictimScript.h has an opt-in FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_METHODS block for the four
  methods. All 13 landed includers were rechecked: 13/13 WINS.

## 0x00DFA450 NScript::CQ_TraderConflictGoodScript::Main (2 attempts)
- Shape:
  - 7 bindings (flags 1,1,1,0,0,0,1);
  - BanditCampEntrance WAIT; OpenDoor(GetThingWithScriptName("Gate1")) and ("Gate2Outer");
  - PlayCutscene("CS_TRADERCON_GOOD_INTRO"); IntroDone = true; KickOffQuestStartScreen(q, true, false); FadeScreenIn;
  - WatchTimeLimit thread; UpdateLiveEnemies(); objective 1;
  - an enemy for-loop over AllCreatures: SetCombatNearbyBreakOffRange(10.0f), EntitySetSleepEnabled(false),
    EntitySetInFaction("FACTION_BANDITS");
  - 4 watcher threads, including WatchForTradersFreed (0x00DFCC10);
  - BanditCampCentre WAIT with a FirstRegionDead check; Gate2Inner; WAIT OutroDone;
  - SetQuestAsCompleted(q, true, true, false);
  - WAIT until BanditCampEntrance is unloaded;
  - DeactivateQuest("Q_TraderConflictGood_Extras", 0); DeactivateQuestLater(q, 0).
- Key spellings:
  - The last wait must be spelled `while (!(!Game->IsRegionLoaded("BanditCampEntrance")))`, which emits retail's
    test al,al / setne bl. Attempt 1 used `while (Game->IsRegionLoaded(...))`, which emits mov bl,al: DIFFER, 6 lines.
  - The enemy loop needs the copy constructor inline:
    `inline CScriptThing::CScriptThing(const CScriptThing& o) : CBaseClass(o), PImp(o.PImp) {}`.
- Label to correct: 0x00DFCC10 is labelled FUN_00dfcc10.
- Header: new hdr/engine/CQ_TraderConflictGoodScript.h (sizeof 0x68; vector AllCreatures at +0x48).

## 0x00CFB080 NScript::CQ_AwakeningTheOracleScript::Main (1 attempt)
- Shape:
  - 11 bindings (flag 0 x10, PretendGuard 1);
  - generators off and objective 1;
  - a PlayedOpeningCS once-only block: NorthernWastes3 WAIT with a single autosave guarded by AutosavedInNW3, the
    Snowspire objective and WAIT, CS_SNOWSPIRE_INTRO, fade in;
  - generators on and objective 2;
  - the WatchForAliveTablets thread (0x00CFF110);
  - NorthernWastes2 generators off;
  - a PlayedNecroCS once-only block: CS_ORACLE_NECROPOLIS_INTRO plus the start screen;
  - `while (TabletsCollected != 4)` WAIT;
  - SetQuestAsCompleted(q, true, false, false); DeactivateQuestLater(q, 0); FadeScreenIn.
- Label to correct: 0x00CFF110 is unlabelled.
- Header: new hdr/engine/CQ_AwakeningTheOracleScript.h (sizeof 0x98; vector at +0x74, maps at +0x80 and +0x8c).

## 0x00CF9A50 NScript::CQ_AmbushTradersScript::Init (1 attempt)
- Uses the repo header engine/CQ_AmbushTradersScript.h unchanged, plus CScriptDef.h / CDefPointer.h, and declares
  `extern CDefPointer<const CScriptDef> pGlobalScriptDef; extern CGameScriptInterfaceBase* pDSTGame;`.
- Body:
  - 4 AddQuestRegion calls;
  - member resets in retail store order;
  - two CTimer assignments, from AmbushTradersBanditSpeechTimerLong and AmbushTradersSpySpeechTimerLong, through
    `inline CTimer& CTimer::operator=(long t){ const long& index = TimerIndex; pDSTGame->SetTimer(index, t); return *this; }`;
  - `CSFlagMap["ConvoyTrigger1"] = false;`, an out-of-line STLport map<CCharString,bool>::operator[] at 0x008ADF10;
  - 3 generators off; the MasterScriptData tallies cleared;
  - 50 speech-table assignments (the SpySpeechSentry* tables reuse the SPY NORMAL / RANDOM lines);
  - 7 AddBoast calls (ids 1, 3, 6, 11, 12, 13, 14; true/2 for 11, true/4 for 12, an extra 6 for 13).
- Name fix: 0x008ADF10 has four invented landed spellings (NAME_FIXES.tsv).

## Header debt
None blocking. A Codex pass13 hdr_manifest once referenced engine/CQ_AwakeningTheOracleScript.h, which never landed;
the new header here supersedes it.
