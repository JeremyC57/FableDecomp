// NScript::CV_KnotholeGladeGatesScript::CKGG_VillagerOutside::Main() -- a Knothole Glade villager left outside the gates: idle while the balverines live, give the intro line when talked to, panic near the gate, and hammer on the closed gates (panicking about the hero if he killed a villager)
// Leaf: ?Main@CKGG_VillagerOutside@CV_KnotholeGladeGatesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA0090, 2392 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 1 (Main) of the entity vtable 0x012EDA38 (its only reference, 0x012EDA3C); Ego_r.pdb / FableWin.pdb name set
//   ?Main@CKGG_VillagerOutside@CV_KnotholeGladeGatesScript@NScript@@EAEXXZ; reads the parent's BalvsKilled +0x51, GatesOpened +0x4d,
//   HeroKilledAVillager +0x4f and TheGate +0x5c (the layout of the chunk 782 KGG rows); TEXT_QST_050_VILLAGER_* literals.
// Callees: IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), rand 0x00BFEB16, ~CIsThingAlive 0x00E241B0, CScriptThing copy 0x004ABE90,
//   scripted-thing dtor 0x007E74D0 (out of line), movie dtor 0x006E7B80; game slots 7, 8, 70, 264, 360 SetThingHasInformation, 361
//   ClearThingHasInformation, 364-366 conversations, 370 StartMovieSequence, 379 (pause), 460 EntitySetAttackThingImmediately, 498
//   EntityForceToLookAtThing.
// Layout: Ego_r.pdb CKGG_VillagerOutside {ParentClass +0x14, PMasterData +0x18, Moved +0x1c, Attacked +0x1d}.
// Shape: the MurderTwist Friend defines; the name test is the inline CCharString == literal (strcmp against "" on the null arm,
//   Compare otherwise: FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ + FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE); /Gr makes no difference.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include <string.h>
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline CScriptThing::~CScriptThing() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing1, const CScriptThing& thing2, float distance);   // 0x00CBE2FF
namespace NScript {
class CV_KnotholeGladeGatesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                                  // +0x48
    bool MissionFailed;                                     // +0x49
    bool MissionOver;                                       // +0x4a
    bool Cutscene1Start;                                    // +0x4b
    bool HeroAlertedToProblem;                              // +0x4c
    bool GatesOpened;                                       // +0x4d
    bool SaidBefore;                                        // +0x4e
    bool HeroKilledAVillager;                               // +0x4f
    bool IntroductionFinished;                              // +0x50
    bool BalvsKilled;                                       // +0x51
    bool CutsceneStarting;                                  // +0x52
    bool Mentioned;                                         // +0x53
    bool ToldSuccess;                                       // +0x54
    long BalvKillCount;                                     // +0x58
    CScriptThing TheGate;                                   // +0x5c
    bool FirstBalvDead;                                     // +0x68
    class CKGG_VillagerOutside : public CEntityScriptBase {
    public:
        CV_KnotholeGladeGatesScript* ParentClass;           // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                // +0x18
        bool Moved;                                         // +0x1c
        bool Attacked;                                      // +0x1d
    private:
        virtual void Main();
    };
};
}
void NScript::CV_KnotholeGladeGatesScript::CKGG_VillagerOutside::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (Attacked) {
        if (IsActiveThreadTerminating()) return;
        if (seh_me.IsValid()) seh_me.Release();
        for (;;) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
    }
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->SetThingHasInformation(Me,false,true,false);
    bool male=false;
    if (Me.GetDefName()=="CREATURE_KN_VILLAGER_MALE") {
        if (IsActiveThreadTerminating()) return;
        male=true;
    }
    while (!ParentClass->BalvsKilled) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (male) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_050_VILLAGER_MALE_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_050_VILLAGER_FEMALE_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        } else if (IsDistanceBetweenThingsUnder(Game->GetHero(),ParentClass->TheGate,30.0f) && rand()%200==0) {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            if (male) {
                if (IsActiveThreadTerminating()) return;
                Game->AddLineToConversation(conversation,"TEXT_QST_050_VILLAGER_MALE_PANIC",false,Me,Game->GetHero());
            } else {
                if (IsActiveThreadTerminating()) return;
                Game->AddLineToConversation(conversation,"TEXT_QST_050_VILLAGER_FEMALE_PANIC",false,Me,Game->GetHero());
            }
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        } else {
            if (IsActiveThreadTerminating()) return;
            if (!ParentClass->GatesOpened) {
                if (IsActiveThreadTerminating()) return;
                Game->EntityForceToLookAtThing(Me,ParentClass->TheGate);
                Game->EntitySetAttackThingImmediately(Me,ParentClass->TheGate,false,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                if (ParentClass->HeroKilledAVillager) {
                    if (IsActiveThreadTerminating()) return;
                    if (seh_me.IsValid()) seh_me.Release();
                    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    long conversation = Game->AddNewConversation(Me,false,false);
                    Game->AddPersonToConversation(conversation,Game->GetHero());
                    if (male) {
                        if (IsActiveThreadTerminating()) return;
                        Game->AddLineToConversation(conversation,"TEXT_QST_050_VILLAGER_MALE_PANIC_ABOUT_HERO",false,Me,Game->GetHero());
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        Game->AddLineToConversation(conversation,"TEXT_QST_050_VILLAGER_FEMALE_PANIC_ABOUT_HERO",false,Me,Game->GetHero());
                    }
                    if (seh_me.IsValid()) seh_me.Release();
                    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->ClearThingHasInformation(Me);
    seh_me.Release();
}