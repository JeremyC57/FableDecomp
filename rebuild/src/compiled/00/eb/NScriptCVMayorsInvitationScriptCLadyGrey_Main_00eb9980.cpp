// NScript::CV_MayorsInvitationScript::CLadyGrey::Main() -- Lady Grey in the Mayor's Invitation quest: mark her on the minimap, beckon the hero and play the intro cut scene, then the quest stages by WhichQuest (1 give the black rose, 2 buy a house, 3 the stolen necklace / Thunder, 4 the fearless quest until the deal is struck), the Thunder duel wait with its reminders, and the marriage offer that ends the mission
// Leaf: ?Main@CLadyGrey@CV_MayorsInvitationScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB9980, 12753 bytes (to its `ret` at 0x00EBCB50), thiscall, plain `ret`, no EH frame.
//   The catalog extent (12944) also holds CV_MayorsInvitationScript::MonitorParanoidQuest 0x00EBCB60 after int3 padding (the
//   spawn target this Main stores at 0x00EBAFEE; Ego_r.pdb ?MonitorParanoidQuest@CV_MayorsInvitationScript@NScript@@QAEXXZ).
// Identity (proven, two sources): slot 1 of the entity vtable 0x012F05F4 that the landed Alloc 0x00EB78B0 stores (0x00EB78E6); slot 0
//   the landed scalar deleting dtor 0x00EB79D0, slot 2 Init 0x00EB7960, slot 3 GetParentScript 0x00EB7950; Ego_r.pdb and FableWin.pdb
//   both hold ?Main@CLadyGrey@CV_MayorsInvitationScript@NScript@@EAEXXZ.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CV_MAYORSINVITATIONSCRIPT_LADYGREY
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_MayorsInvitationScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
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
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define LG_START(PRIO) \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me,seh_me,PRIO)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define LG_HERO \
    CScriptGameResourceObjectScriptedThingBase seh_hero; \
    Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
#define LG_MAP \
    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap; \
    mymap["HERO"]=seh_hero; \
    mymap["LADY"]=seh_me;
#define LG_MOVIE \
    CScriptGameResourceObjectMovieBase movie; \
    Game->StartMovieSequence("",movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
#define LG_ONELINER(TEXT) \
    std::map<CCharString,CCharString> args; \
    args["$LINE"]=TEXT; \
    RunCutsceneMacro("CS_INVITATION_ONELINER",mymap,0,&args,false,true);
#define LG_ASK(QUESTION) \
    Game->GiveHeroYesNoQuestion(QUESTION,"TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true); \
    long answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    while (answer<0) { \
        NSF; \
        if (T) return; \
        answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    } \
    if (T) return;
#define LG_SPAWN(FUNC) \
    ParentClass.AddSpawnedFunction(new CSpawnedFunc<CV_MayorsInvitationScript>(CCharString("ParentClass.")+CCharString(#FUNC),0,&ParentClass,&CV_MayorsInvitationScript::FUNC),CCharString(""));
#define LG_THUNDER \
    Game->TakeObjectFromHero("OBJECT_LADY_GREY_STOLEN_NECKLACE"); \
    if (PMasterData->KilledThunder) { \
        if (T) return; \
        ParentClass.HeroHasDefeatedThunder=true; \
        RunCutsceneMacro("CS_INVITATION_THUNDER_DEAD",mymap,0,0,false,true); \
    } \
    else { \
        if (T) return; \
        RunCutsceneMacro("CS_INVITATION_THUNDER_START",mymap,0,0,false,true); \
    } \
    ParentClass.HeroReturnedFromParanoidQuest=true; \
    ParentClass.WhichQuest=5; \
    LG_SPAWN(MonitorThunderHeroDuel)

void NScript::CV_MayorsInvitationScript::CLadyGrey::Main()
{
    Game->SetThingHasInformation(Me,false,true,false);
    Game->MiniMapAddMarker(Me,"HUD_ORB_QUEST_VIGNETTE");
    SetPredicate(CIsThingAlive(Me));
    NSF;
    if (T) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    LG_START(SCRIPT_AI_PRIORITY_HIGH)
    if (PMasterData->StruckDealWithLadyGrey) {
        if (T) return;
        ParentClass.WhichQuest=5;
    }
    if (ParentClass.WhichQuest==5 && !ParentClass.HeroHasDefeatedThunder && !ParentClass.HeroHasLostToThunder) {
        if (T) return;
        LG_SPAWN(MonitorThunderHeroDuel)
    }
    if (ParentClass.WhichQuest==0) {
        if (T) return;
        while (!IsDistanceBetweenThingsUnder(Me,Game->GetHero(),10.0f)) {
            NSF;
            if (T) return;
        }
        if (T) return;
        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
        seh_me.PlayAnimation("ST_RESPONSE_WAVE",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
        long conversation=Game->AddNewConversation(Me,false,false);
        Game->AddPersonToConversation(conversation,Game->GetHero());
        Game->AddLineToConversation(conversation,"TEXT_QST_044_LADY_GREY_BECKON",false,Me,Game->GetHero());
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            NSF;
            if (T) return;
        }
        if (T) return;
        {
            LG_HERO
            LG_START(SCRIPT_AI_PRIORITY_HIGHEST)
            LG_MAP
            LG_MOVIE
            RunCutsceneMacro("CS_INVITATION_INTRO",mymap,0,0,false,true);
            ParentClass.WhichQuest=1;
            LG_START(SCRIPT_AI_PRIORITY_HIGH)
        }
        if (!ParentClass.QuestCardGiven) {
            if (T) return;
            Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_MAYORS_INVITATION",Game->GetActiveQuestName(),false);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_01","","");
            ParentClass.QuestCardGiven=true;
        }
    }
    CCharString item("");
    while (ParentClass.WhichQuest==1) {
        NSF;
        if (T) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            LG_MOVIE
            LG_HERO
            LG_START(SCRIPT_AI_PRIORITY_HIGHEST)
            LG_MAP
            LG_ASK("TEXT_QST_044_QUESTION_GIVE_ROSE")
            if (answer==1 && Game->IsObjectInThingsPossession("OBJECT_ROSE_BLACK_01",Game->GetHero())) {
                if (T) return;
                Game->TakeObjectFromHero("OBJECT_ROSE_BLACK_01");
                RunCutsceneMacro("CS_INVITATION_ROSEGIVEN",mymap,0,0,false,true);
                ParentClass.WhichQuest=2;
                Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_02","","");
            }
            else if (answer==1) {
                if (T) return;
                LG_ONELINER("TEXT_QST_044_LADY_GREY_LIED_ABOUT_ROSE")
            }
            else {
                if (T) return;
                LG_ONELINER("TEXT_QST_044_LADY_GREY_NOT_GOT_ROSE")
            }
            LG_START(SCRIPT_AI_PRIORITY_HIGH)
        }
        if (Me.MsgIsPresentedWithItem(item)) {
            if (T) return;
            if (item=="OBJECT_ROSE_BLACK_01" || item=="OBJECT_CARROT") {
                if (T) return;
                {
                    LG_MOVIE
                    LG_HERO
                    LG_START(SCRIPT_AI_PRIORITY_HIGHEST)
                    LG_MAP
                    Game->TakeObjectFromHero("OBJECT_ROSE_BLACK_01");
                    RunCutsceneMacro("CS_INVITATION_ROSEGIVEN",mymap,0,0,false,true);
                }
                ParentClass.WhichQuest=2;
                Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_02","","");
            }
        }
    }
    if (T) return;
    while (ParentClass.WhichQuest==2) {
        NSF;
        if (T) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            LG_MOVIE
            LG_HERO
            LG_START(SCRIPT_AI_PRIORITY_HIGHEST)
            LG_MAP
            if (Game->GetNumHousesOwned()>=1) {
                if (T) return;
                RunCutsceneMacro("CS_INVITATION_HOUSEBOUGHT",mymap,0,0,false,true);
                ParentClass.WhichQuest=3;
                Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_03","","");
            }
            else {
                if (T) return;
                LG_ONELINER("TEXT_QST_044_LADY_GREY_HOUSE_FAIL")
            }
            LG_START(SCRIPT_AI_PRIORITY_HIGH)
        }
    }
    if (T) return;
    if (!ParentClass.HeroReturnedFromParanoidQuest) {
        if (T) return;
        LG_SPAWN(MonitorParanoidQuest)
    }
    while (ParentClass.WhichQuest==3) {
        NSF;
        if (T) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            LG_MOVIE
            LG_HERO
            LG_START(SCRIPT_AI_PRIORITY_HIGHEST)
            LG_MAP
            if (Game->IsObjectInThingsPossession("OBJECT_LADY_GREY_STOLEN_NECKLACE",Game->GetHero())) {
                if (T) return;
                LG_ASK("TEXT_QST_044_QUESTION_GIVE_NECKLACE")
                if (answer==1) {
                    if (T) return;
                    LG_THUNDER
                }
                else {
                    if (T) return;
                    LG_ONELINER("TEXT_QST_044_LADY_GREY_GOSSIP_QUEST_REMINDER")
                }
            }
            else if (ParentClass.HeroLearnedSecret) {
                if (T) return;
                RunCutsceneMacro("CS_INVITATION_FEARLESS_START",mymap,0,0,false,true);
                ParentClass.WhichQuest=4;
                Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_04","OakvaleMemorialGarden","");
            }
            else {
                if (T) return;
                LG_ONELINER("TEXT_QST_044_LADY_GREY_GOSSIP_QUEST_REMINDER")
            }
            LG_START(SCRIPT_AI_PRIORITY_HIGH)
        }
        if (PMasterData->KilledThunder && Game->IsObjectInThingsPossession("OBJECT_LADY_GREY_STOLEN_NECKLACE",Game->GetHero())) {
            if (T) return;
            ParentClass.WhichQuest=5;
            ParentClass.HeroHasDefeatedThunder=true;
        }
    }
    if (T) return;
    while (ParentClass.WhichQuest==4 && !PMasterData->StruckDealWithLadyGrey) {
        NSF;
        if (T) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            LG_MOVIE
            LG_HERO
            LG_START(SCRIPT_AI_PRIORITY_HIGHEST)
            LG_MAP
            if (Game->IsObjectInThingsPossession("OBJECT_LADY_GREY_STOLEN_NECKLACE",Game->GetHero())) {
                if (T) return;
                LG_ASK("TEXT_QST_044_QUESTION_GIVE_NECKLACE")
                if (answer==1) {
                    if (T) return;
                    LG_THUNDER
                }
                else {
                    if (T) return;
                    LG_ONELINER("TEXT_QST_044_LADY_GREY_NECKLACE_REMINDER")
                }
            }
            else {
                if (T) return;
                LG_ONELINER("TEXT_QST_044_LADY_GREY_NECKLACE_REMINDER")
            }
            LG_START(SCRIPT_AI_PRIORITY_HIGH)
        }
    }
    if (T) return;
    LG_START(SCRIPT_AI_PRIORITY_HIGH)
    while (!ParentClass.HeroHasDefeatedThunder && !ParentClass.HeroHasLostToThunder) {
        NSF;
        if (T) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            if (PMasterData->StruckDealWithLadyGrey) {
                if (T) return;
                LG_MOVIE
                LG_HERO
                LG_START(SCRIPT_AI_PRIORITY_HIGHEST)
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_044_LADY_GREY_THUNDER_TASK_STRUCK_DEAL",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        NSF;
                        if (T) return;
                    }
                    if (T) return;
                }
                LG_START(SCRIPT_AI_PRIORITY_HIGH)
            }
            else {
                if (T) return;
                LG_MOVIE
                LG_HERO
                LG_START(SCRIPT_AI_PRIORITY_HIGHEST)
                LG_MAP
                LG_ONELINER("TEXT_QST_044_LADY_GREY_THUNDER_TASK_INCOMPLETE_10")
                LG_START(SCRIPT_AI_PRIORITY_HIGH)
            }
        }
    }
    if (T) return;
    Game->EntitySetAsKillable(Me,true,true);
    while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
        NSF;
        if (T) return;
    }
    if (T) return;
    {
    LG_HERO
    LG_START(SCRIPT_AI_PRIORITY_HIGHEST)
    LG_MAP
    LG_MOVIE
    if (ParentClass.HeroHasDefeatedThunder) {
        if (T) return;
        LG_ASK("TEXT_QST_044_QUESTION_OFFER_MARRIAGE")
        if (answer==1) {
            if (T) return;
            RunCutsceneMacro("CS_INVITATION_WEDDINGBELLS",mymap,0,0,false,true);
            ParentClass.MissionSucceeded=true;
        }
        else {
            if (T) return;
            RunCutsceneMacro("CS_INVITATION_WEDDINGOFF",mymap,0,0,false,true);
            ParentClass.MissionFailed=true;
        }
    }
    else {
        if (T) return;
        LG_ONELINER("TEXT_QST_044_LADY_GREY_REJECTED_BY_HERO_20")
        ParentClass.MissionFailed=true;
    }
    if (seh_me.IsValid()) seh_me.Release();
    }
}