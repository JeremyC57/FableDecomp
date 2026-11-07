// NScript::CQ_GuildTrainingScript::CAppleGirl::Main() -- the apple girl at the Guild: she asks a passing hero for help; talked to she chats and asks him to fetch apples (yes starts the apple hunt); then she takes up to four red apples, counts how many are still needed, and at four thanks him with a blueberry pie; while the pre-melee quest is no longer active in child mode she is removed.
// Grade: DIFFER(4252v4252) -- 28 masked lines, register colouring only: in the MANY_MORE_APPLES / NO_MORE_APPLES arms retail loads the Game vtable into eax and the line string address into ecx; ours keeps the ONE_MORE_APPLE arm's edx / eax in all three arms
// Next idea: chunk 835 c12 (switch case 1 / default / case 0: 4256 bytes, 38), c13 (ApplesGiven read through a reference to the local) and c14 (a one-member struct) compile to the same bytes as the plain local -- the colouring is not a memory-vs-register effect; 26 spellings in all (hand805 a2-a12, chunk 823 c1-c11). Lane gap: VC7.1 register allocation of a vtable temp after the `test eax,eax` arm split
// Leaf: ?Main@CAppleGirl@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3D150, 4252 bytes. Name: slot 1 of the entity vtable 0x012CD7E8 that the landed Alloc
//   0x00D3D060 stores (0 0x00D3D130, 2 Init 0x00D3D110, 3 GetParentScript 0x00D3D100, 5 OnPredicateFail 0x00D3D120); Ego_r.pdb and
//   FableWin.pdb ?Main@CAppleGirl@CQ_GuildTrainingScript@NScript@@EAEXXZ. Layout: engine/CQ_GuildTrainingScript.h opt-in
//   FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_ENTITIES (hdr overlay).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_ENTITIES
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_GuildTrainingScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::~CScriptThing() { }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
void NScript::CQ_GuildTrainingScript::CAppleGirl::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Me.SetFriendsWithEverythingFlag(true);
    Game->EntitySetAsKillable(Me,false,true);
    CScriptThing Village=Game->GetNearestWithDefName(Me,"VILLAGE_GUILD_COMPLEX_INSIDE");
    Game->EntityAttachToVillage(Me,Village);
    AppleMode=0;
    CTimer ChatTimer;
    ChatTimer=15;
    CurrentApples=0;
    ChildAppleMode=false;
    Game->SetThingHasInformation(Me,false,true,false);
    while (AppleMode==0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (IsDistanceBetweenThingsUnder(Game->GetHero(),Me,5.5f) && ChatTimer.GetTime()<=0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long Conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(Conversation,Game->GetHero());
            Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPLEGIRL_HELP",false,Me,Game->GetHero());
            ChatTimer=15;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (HaveChatted) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->GiveHeroYesNoQuestion("TEXT_QST_028_APPLEGIRL_QUESTION_AGAIN","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                HaveChatted=true;
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_028_APPLEGIRL_CHAT",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->GiveHeroYesNoQuestion("TEXT_QST_028_APPLEGIRL_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
            }
            long answer=Game->MsgIsQuestionAnsweredYesOrNo();
            while (answer<0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                answer=Game->MsgIsQuestionAnsweredYesOrNo();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (answer==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                AppleMode=1;
                if (Game->IsQuestActive("Q_GuildTrainingPreMelee")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ChildAppleMode=true;
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_028_APPLEGIRL_IMPLORE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (AppleMode==1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ChildAppleMode==true && !Game->IsQuestActive("Q_GuildTrainingPreMelee")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->RemoveThing(Me,false,true);
        }
        if (IsDistanceBetweenThingsUnder(Game->GetHero(),Me,5.5f) && ChatTimer.GetTime()<=0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (CurrentApples) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(Conversation,Game->GetHero());
                Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPLEGIRL_MORE_APPLES",false,Me,Game->GetHero());
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(Conversation,Game->GetHero());
                Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPLEGIRL_ANY_APPLES",false,Me,Game->GetHero());
            }
            ChatTimer=15;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long ApplesGiven=0;
            while (Game->IsObjectInThingsPossession("OBJECT_APPLE_RED_01",Game->GetHero()) && CurrentApples+ApplesGiven<4) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ++ApplesGiven;
                Game->TakeObjectFromHero("OBJECT_APPLE_RED_01");
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ApplesGiven==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(Conversation,Game->GetHero());
                Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPLEGIRL_ONE_MORE_APPLE",false,Me,Game->GetHero());
            }
            else if (ApplesGiven!=0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(Conversation,Game->GetHero());
                Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPLEGIRL_MANY_MORE_APPLES",false,Me,Game->GetHero());
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(Conversation,Game->GetHero());
                Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPLEGIRL_NO_MORE_APPLES",false,Me,Game->GetHero());
            }
            Game->Pause(1.0f);
            CurrentApples+=ApplesGiven;
            if (ApplesGiven!=0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation=Game->AddNewConversation(Me,false,false);
                switch (CurrentApples) {
                case 1:
                    Game->AddPersonToConversation(Conversation,Game->GetHero());
                    Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPLEGIRL_THREE_NEEDED",false,Me,Game->GetHero());
                    Game->Pause(1.0f);
                    break;
                case 2:
                    Game->AddPersonToConversation(Conversation,Game->GetHero());
                    Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPLEGIRL_TWO_NEEDED",false,Me,Game->GetHero());
                    Game->Pause(1.0f);
                    break;
                case 3:
                    Game->AddPersonToConversation(Conversation,Game->GetHero());
                    Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPLEGIRL_ONE_NEEDED",false,Me,Game->GetHero());
                    Game->Pause(1.0f);
                    break;
                case 4: {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_028_APPLEGIRL_THANKS",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game->GiveHeroObject("OBJECT_PIE_BLUEBERRY_01",-1,false);
                    AppleMode=2;
                    Game->ClearThingHasInformation(Me);
                    break;
                }
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ChildAppleMode==true && !Game->IsQuestActive("Q_GuildTrainingPreMelee")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->RemoveThing(Me,false,true);
        }
        if (IsDistanceBetweenThingsUnder(Game->GetHero(),Me,5.5f) && ChatTimer.GetTime()<=0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long Conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(Conversation,Game->GetHero());
            Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPLEGIRL_THANKS_AGAIN",false,Me,Game->GetHero());
            ChatTimer=15;
        }
    }
}
