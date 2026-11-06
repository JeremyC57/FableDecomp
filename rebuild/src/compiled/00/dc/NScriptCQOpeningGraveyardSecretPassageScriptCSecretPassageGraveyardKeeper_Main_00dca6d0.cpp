// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Main() -- the graveyard keeper: waits for the keeper cutscene, meets the hero (CS_OPENGRAVE_KEEPERMEET), follows him, complains when hit, gives directions / mumbles when talked to, removes the talk tags near him and comments on each ceremonial artifact the hero finds.
// Leaf: ?Main@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCA6D0, length 6395 bytes, thiscall, no EH frame, ends in its `ret` at 0x00DCBFCA.
// Name: slot 1 of the entity vtable 0x012DA658 (slot 0 scalar deleting dtor 0x00DC4C20, slot 2 Init 0x00DC4B50, landed as
//   CSecretPassageGraveyardKeeper); Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ.
// Data: own already_talked_to +0x1C, Helmfound +0x1D, Swordfound +0x1E, Shieldfound +0x1F, Armourfound +0x20; ParentClass
//   DoorChange +0x49, skeleton_visited +0x4B, doors_open +0x4C, GravekeeperIntroduced +0x4D, GravekeeperCutsceneStart +0x4E,
//   UndeadCutscenePlayed +0x4F, UndeadCutsceneStarted +0x50.
// Shape: generated entity script, a T-check before every statement, plain returns.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_OpeningGraveyardSecretPassageScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();      // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();     // 0x00CDBFB0
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
#define GK_SPEAK(TEXT) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),TEXT,GROUP_SELECT_NONE,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            if (IsActiveThreadTerminating()) return; \
        } \
        if (IsActiveThreadTerminating()) return; \
    }
#define GK_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        if (IsActiveThreadTerminating()) return; \
    } \
    if (IsActiveThreadTerminating()) return;
#define GK_ARTIFACT(FOUND,OBJ,TEXT) \
    if (!FOUND && Game->IsObjectInThingsPossession(OBJ,Game->GetHero())) { \
        if (IsActiveThreadTerminating()) return; \
        FOUND=true; \
        if (IsDistanceBetweenThingsUnder(Me,Game->GetHero(),GHOST_DISTANCE)) { \
            if (IsActiveThreadTerminating()) return; \
            long conversation=Game->AddNewConversation(Me,false,false); \
            Game->AddPersonToConversation(conversation,Game->GetHero()); \
            Game->AddLineToConversation(conversation,TEXT,false,Me,Game->GetHero()); \
        } \
    }
void NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    GK_START
    Game->SetThingHasInformation(Me,false,false,false);
    Game->EntitySetOpinionReactionsEnabled(Me,false);
    Game->EntitySetAsKillable(Me,false,true);
    Game->EntitySetTargetable(Me,false);
    Game->EntitySetAsDamageable(Me,false);
    while (!ParentClass.GravekeeperCutsceneStart) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetAsKillable(Me,true,true);
    Game->EntitySetTargetable(Me,true);
    Game->EntitySetAsDamageable(Me,true);
    Game->SetIsPushableByHero(Me,false);
    Game->EntitySetAsUseMovementInActions(Me,false);
    if (seh_me.IsValid()) seh_me.Release();
    while (!ParentClass.GravekeeperIntroduced) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    GK_START
    if (!ParentClass.doors_open) {
        if (IsActiveThreadTerminating()) return;
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->SetIsPushableByHero(Me,true);
        Game->EntitySetAsUseMovementInActions(Me,true);
        Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_02","","Graveyard");
        {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
        GK_START
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"]=seh_hero;
        mymap["KEEPER"]=seh_me;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("",movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        RunCutsceneMacro("CS_OPENGRAVE_KEEPERMEET",mymap,0,0,false,true);
        }
        ParentClass.doors_open=true;
    }
    if (seh_me.IsValid()) seh_me.Release();
    Game->EntityFollowThing(Me,Game->GetHero(),1.0f,true);
    long talk_count=0;
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (ParentClass.UndeadCutsceneStarted && !ParentClass.UndeadCutscenePlayed) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!ParentClass.UndeadCutscenePlayed) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            GK_START
            seh_me.ClearCommands();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                GK_SPEAK("TEXT_QST_049_GRAVEKEEPER_HIT")
                if (seh_me.IsValid()) seh_me.Release();
            }
            Game->EntityFollowThing(Me,Game->GetHero(),1.0f,true);
            Me.SetFriendsWithEverythingFlag(true);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            GK_START
            seh_me.ClearCommands();
            if (ParentClass.skeleton_visited) {
                if (IsActiveThreadTerminating()) return;
                if (talk_count<2) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    GK_SPEAK("TEXT_QST_049_GRAVEKEEPER_POST_SKELETON_MUMBLING_FIRST")
                    talk_count=2;
                }
                else if (talk_count==2) {
                    if (IsActiveThreadTerminating()) return;
                    talk_count=3;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    GK_SPEAK("TEXT_QST_049_GRAVEKEEPER_POST_SKELETON_MUMBLING_SECOND")
                }
                else if (talk_count==3) {
                    if (IsActiveThreadTerminating()) return;
                    talk_count=4;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    GK_SPEAK("TEXT_QST_049_GRAVEKEEPER_POST_SKELETON_MUMBLING_THIRD")
                }
                else if (talk_count==4) {
                    if (IsActiveThreadTerminating()) return;
                    talk_count=3;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    GK_SPEAK("TEXT_QST_049_GRAVEKEEPER_POST_SKELETON_MUMBLING_FOURTH")
                }
            }
            else {
                if (IsActiveThreadTerminating()) return;
                if (talk_count==0) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    GK_SPEAK("TEXT_QST_049_GRAVEKEEPER_PRE_SKELETON_DIRECTIONS_FIRST")
                    talk_count=1;
                }
                else {
                    if (IsActiveThreadTerminating()) return;
                    talk_count=1;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    GK_SPEAK("TEXT_QST_049_GRAVEKEEPER_PRE_SKELETON_DIRECTIONS_SECOND")
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
            Game->EntityFollowThing(Me,Game->GetHero(),1.0f,true);
        }
        CScriptThing talk_tag=Game->GetNearestWithScriptName(Me,"SecretPassageTalkTag");
        if (IsDistanceBetweenThingsUnder(Me,talk_tag,GHOST_DISTANCE) && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),GHOST_DISTANCE)) {
            if (IsActiveThreadTerminating()) return;
            if (!ParentClass.DoorChange) {
                if (IsActiveThreadTerminating()) return;
                CCharString text=talk_tag.GetDataString();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                Game->AddLineToConversation(conversation,text,false,Me,Game->GetHero());
            }
            Game->RemoveThing(talk_tag,false,true);
        }
        GK_ARTIFACT(Helmfound,"OBJECT_CEREMONIAL_HELMET","TEXT_QST_049_GRAVEKEEPER_HELM_FOUND")
        GK_ARTIFACT(Armourfound,"OBJECT_CEREMONIAL_ARMOUR","TEXT_QST_049_GRAVEKEEPER_ARMOUR_FOUND")
        GK_ARTIFACT(Swordfound,"OBJECT_CEREMONIAL_SWORD","TEXT_QST_049_GRAVEKEEPER_SWORD_FOUND")
        GK_ARTIFACT(Shieldfound,"OBJECT_CEREMONIAL_SHIELD","TEXT_QST_049_GRAVEKEEPER_SHIELD_FOUND")
    }
}