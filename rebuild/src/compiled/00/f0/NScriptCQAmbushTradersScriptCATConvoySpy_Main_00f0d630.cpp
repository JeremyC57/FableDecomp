// NScript::CQ_AmbushTradersScript::CAT_ConvoySpy::Main() -- the Ambush Traders convoy spy (Tanya): restart her allegiance refresh and the death / hero-proximity / speech-marker threads, follow the hero if she already does, wait in the Deserted Farm for the convoy trigger, walk to the convoy exit point; then each frame: follow the hero once seen, stop at the no-entry marker until the hero reaches the reset marker, play the Bowerstone Posh outro cut scene (reward gold and the bonus weapon), walk out when the quest fails or ends, record a teleport entry at the exit point (Deserted Farm: update the quest card, fade, wait for the Gibbet Woods and idle; Gibbet Woods: escaped), talk to the hero, and obey the FOLLOW / WAIT expressions
// Leaf: ?Main@CAT_ConvoySpy@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F0D630, 5989 bytes, thiscall, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012F7E14 stored by
//   CAT_ConvoySpy::Alloc 0x00F04360 (slot 0 the landed scalar deleting dtor 0x00F044F0, slot 2 the landed Init 0x00F04410, slot 4 the
//   landed OnPersist 0x00F04470); Ego_r.pdb and FableWin.pdb both hold ?Main@CAT_ConvoySpy@CQ_AmbushTradersScript@NScript@@EAEXXZ.
// Callees: as CAT_BanditAlly::Main 0x00F063B0 plus CScriptThing() 0x006E7B40 / operator= 0x008AB980, the scripted-thing ctor
//   0x007E72A0 / FollowThing 0x007E7320 / ClearCommands 0x007E7360 / ClearAllActions 0x007E7400 / operator= 0x008ABD10, cast map ctor
//   0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D, CDefString conversion 0x00415D70, __ftol2,
//   TeleportData dtor 0x00CDBC50 (its ctor expanded here), vector<TeleportData>::push_back 0x00F0D490, CCharString::operator=
//   0x0099EFB0 / 0x0099EFE0; parent RefreshSpyAllegiences 0x00F05A10, ProcessInitialSpyExtraction 0x00F0F680 and the spawned threads
//   WatchForSpyDeath 0x00F0EDA0, WatchForHeroProximity 0x00F0EE90, WatchForSpySpeechMarkers 0x00F0EF60.
// Data: ParentClass (+0x14) QuestFailed +0x48, ConvoyTrigger2 +0x51, SpySeenBySentry +0x5a, SpyFollowingHero +0x5c, SpyHeroProximity
//   +0x5d, SpyWanderReset +0x5e, SpyContactMade +0x5f, SpyEscaped +0x60, SpyExited +0x61, SpyReachedNoEntryMarker +0x62,
//   QuestSuccessSpeechComplete +0x65, BanditContactKilledBeforeQuestEnd +0x68, SpyHealthBarID +0x84, TeleportInfo +0x1c4;
//   pGlobalScriptDef AmbushTradersQuestSuccessReward +0x9fc, AmbushTradersQuestSuccessBonus +0xa00.
// Shape: generated entity script (CAT_ConvoyMember 0x00F0CAA0 / CAT_BanditAlly 0x00F063B0 models): function-scope locals (the
//   expression name, the teleport record, an unused CScriptThing, seh_me), a T-check before every statement, `for (;;)` main loop.
#include "fable_compiler.h"
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYMEMBER
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYSPY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/fable_script_text.h"
#include "engine/fable_script_cutscene.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
enum EScriptingStateGroups { SCRIPTING_STATE_GROUP_NONE=0, SCRIPTING_STATE_GROUP_WANDER=4 };
extern "C" int __cdecl rand();
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
extern CGameScriptInterfaceBase* pDSTGame;
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
inline NScript::CQ_AmbushTradersScript::TeleportData::TeleportData() {}
inline CScriptThing::CScriptThing() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
// The body of CBasicString<char>::Compare 0x00411570 (the landed 0x00411570 loop; chunk 807 lesson): same-TU and not inlined, as in
// retail, so VC7.1 knows the call leaves the string alone and keeps `expression`'s data pointer in esi across the compare chain.
template<> inline __declspec(noinline) long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}


void NScript::CQ_AmbushTradersScript::CAT_ConvoySpy::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CCharString expression;
    TeleportData teleport;
    CScriptThing target;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    ParentClass.RefreshSpyAllegiences();
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_AmbushTradersScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForSpyDeath"),0,&ParentClass,&CQ_AmbushTradersScript::WatchForSpyDeath,Me),CCharString(""));
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_AmbushTradersScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForHeroProximity"),0,&ParentClass,&CQ_AmbushTradersScript::WatchForHeroProximity,Me),CCharString(""));
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_AmbushTradersScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForSpySpeechMarkers"),0,&ParentClass,&CQ_AmbushTradersScript::WatchForSpySpeechMarkers,Me),CCharString(""));
    if (ParentClass.SpyFollowingHero) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        seh_me.FollowThing(Game->GetHero(), 1.0f, true);
    }
    ParentClass.SpyWanderReset = true;
    if (Game->IsRegionLoaded("DesertedFarm")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.GetDataString() == "WaitForTrigger") {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (!ParentClass.ConvoyTrigger2) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    CScriptThing exit_point = Game->GetThingWithScriptName("M_AT_ConvoyExitPoint");
    if (!ParentClass.SpyContactMade && !ParentClass.SpyHeroProximity) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        seh_me.MoveToPosition(exit_point.GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
    }
    CScriptThing no_entry = Game->GetThingWithScriptName("M_AT_NoEntry");
    CScriptThing reset_no_entry = Game->GetThingWithScriptName("M_AT_ResetNoEntry");
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if ((ParentClass.SpySeenBySentry || ParentClass.SpyHeroProximity) && !ParentClass.SpyContactMade && !ParentClass.SpyReachedNoEntryMarker) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.SpyWanderReset) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.ClearCommands();
                    seh_me.FollowThing(Game->GetHero(), 2.0f, true);
                    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
                }
                ParentClass.SpyWanderReset = false;
            }
            if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 3.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.SpyWanderReset = true;
            }
        }
        if (!ParentClass.SpyContactMade) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (no_entry.IsAlive() && IsDistanceBetweenThingsUnder(Me, no_entry, 2.0f) && !ParentClass.SpyReachedNoEntryMarker) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntityStopFollowing(Me);
                if (seh_me.IsValid()) seh_me.Release();
                Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
                ParentClass.SpyReachedNoEntryMarker = true;
            }
            else if (reset_no_entry.IsAlive() && IsDistanceBetweenThingsUnder(Game->GetHero(), reset_no_entry, 2.0f) && ParentClass.SpyReachedNoEntryMarker) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                seh_me.FollowThing(Game->GetHero(), 2.0f, true);
                Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
                ParentClass.SpyReachedNoEntryMarker = false;
            }
        }
        if (Game->IsRegionLoaded("BowerstonePosh") && !ParentClass.QuestSuccessSpeechComplete) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.0f);
            Game->EntityStopFollowing(Me);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearAllActions();
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["SPY"] = seh_me;
            Game->RemoveQuestInfoElement(ParentClass.SpyHealthBarID);
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            if (!ParentClass.BanditContactKilledBeforeQuestEnd) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_AMBUSH_TRADERS_OUTRO_TANYA", mymap, 0, 0, false, true);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_AMBUSH_TRADERS_OUTRO_NOTANYA", mymap, 0, 0, false, true);
            }
            Game->FixMovieSequenceCamera(false);
            Game->GiveHeroGold((long)pGlobalScriptDef->AmbushTradersQuestSuccessReward);
            Game->GiveHeroWeapon(pGlobalScriptDef->AmbushTradersQuestSuccessBonus, false);
            ParentClass.QuestSuccessSpeechComplete = true;
        }
        if (ParentClass.QuestFailed || ParentClass.QuestSuccessSpeechComplete) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing region_exit;
            if (Game->IsRegionLoaded("BowerstonePosh")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                region_exit = Game->GetThingWithScriptName("M_AT_BanditExitPoint");
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                region_exit = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
            }
            if (region_exit.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(region_exit.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                }
                while (!IsDistanceBetweenThingsUnder(Me, region_exit, 2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.SpyExited = true;
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
        }
        if (IsDistanceBetweenThingsUnder(Me, exit_point, 5.0f) && !ParentClass.SpyContactMade && !ParentClass.SpyHeroProximity) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntityGetAppearanceSeed(Me, teleport.appearanceSeed);
            teleport.defName = Me.GetDefName();
            teleport.health = Game->GetHealth(Me);
            teleport.scriptName = "AT_ConvoySpy";
            Game->GetItemDefNamesFromContainer(Me, teleport.itemsCarried);
            ParentClass.TeleportInfo.push_back(teleport);
            if (Game->IsRegionLoaded("DesertedFarm")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_AMBUSH_TRADERS_OBJECTIVE_03", "GibbetWoods", "BowerstoneSlums");
                FadeOutAndKillEntity(Me, 1.0f, true, true);
                while (!Game->IsRegionLoaded("GibbetWoods")) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                for (;;) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (Game->IsRegionLoaded("GibbetWoods")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.SpyEscaped = true;
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.SpyFollowingHero) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_003_SPY_FOLLOWING", false, Me, Game->GetHero());
            }
            else if (!ParentClass.SpyContactMade) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.SpyContactMade = true;
                Game->EntitySetAsScared(Me, false);
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_003_SPY_FOLLOW", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (seh_me.IsValid()) seh_me.Release();
                ParentClass.ProcessInitialSpyExtraction(Me, seh_me);
            }
        }
        if (Me.MsgExpressionPerformedTo(expression)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (expression == "EXPRESSION_FOLLOW") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->AddLineToConversation(conversation, "TEXT_QST_003_SPY_FOLLOW", false, Me, Game->GetHero());
                if (!ParentClass.SpyContactMade) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.SpyContactMade = true;
                    ParentClass.ProcessInitialSpyExtraction(Me, seh_me);
                }
                ParentClass.SpyFollowingHero = true;
            }
            else if (expression == "EXPRESSION_WAIT") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.SpyFollowingHero = false;
                Game->AddLineToConversation(conversation, "TEXT_QST_003_SPY_WAIT", false, Me, Game->GetHero());
            }
        }
    }
}