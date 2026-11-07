// NScript::CV_HeroDollsScript::CBS_Teacher::Main() -- the Bowerstone schoolteacher of the hero-dolls swap: hide the doll kid's information marker, then until the doll script ends: when the hero carries all the dolls, take the teacher over, wait for the hero to come close, follow him, introduce the swap and ask; yes swaps the dolls for the Jack of Blades doll and ends the script, no ends the loop; when the hero first carries a doll, the doll kid walks up, introduces the dolls and hands over the quest card and objective; otherwise note a newly received doll; finally release the teacher, complete and deactivate the quest
// Grade: DIFFER(3619v3619) -- same size; the only residue is retail's entry store of ebx (0) to [esp+20h] (a conditional-temporary flag home that later holds the GetActiveQuestName temporaries) and the 4-byte slot shift of the locals above it: the same unexplained flag as Book 0x00E54E90
// Next idea: find the threaded-away conditional temporary (the same construct as in Book); the slot shift then disappears
// Leaf: ?Main@CBS_Teacher@CV_HeroDollsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E97070, 3619 bytes; entity vtable 0x012ECEB4 slot 1.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_HeroDollsScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
#define HD_T if (IsActiveThreadTerminating()) return
#define HD_SAY(seh, key) \
    if (Game->GetHealth(seh.GetScriptThing()) > 0.0f) { \
        seh.Speak(Game->GetHero(), key, GROUP_SELECT_NONE, false, true, false); \
        while (seh.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            HD_T; \
        } \
        HD_T; \
    }

void NScript::CV_HeroDollsScript::CBS_Teacher::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (KidIntro.IsAlive()) {
        HD_T;
        Game->ClearThingHasInformation(KidIntro);
    }
    bool hero_has_dolls = ParentClass->HeroHasAnyDolls();
    Game->Pause(0.5f);
    while (!ParentClass->HeroDollScriptEnded) {
        Game->NewScriptFrame();
        HD_T;
        if (ParentClass->HeroHasAllDolls()) {
            HD_T;
            PMasterData->HeroDollsScriptUsingTeacher = true;
            Game->SetThingHasInformation(Me, false, true, false);
            Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST);
            while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 20.0f)) {
                Game->NewScriptFrame();
                HD_T;
            }
            HD_T;
            seh_me.FollowThing(Game->GetHero(), 1.0f, true);
            while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 5.0f)) {
                Game->NewScriptFrame();
                HD_T;
            }
            HD_T;
            seh_me.ClearCommands();
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            HD_SAY(seh_me, "TEXT_QST_083_TEACHER_INTRODUCTION");
            Game->GiveHeroYesNoQuestion("TEXT_QST_083_TEACHER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long answer;
            while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                Game->NewScriptFrame();
                HD_T;
            }
            HD_T;
            if (answer == 1) {
                HD_T;
                HD_SAY(seh_me, "TEXT_QST_083_TEACHER_DO_SWAP");
                ParentClass->RemoveHeroDolls();
                Game->GiveHeroObject("OBJECT_DOLL_JACK_OF_BLADES_01", -1, false);
                ParentClass->HeroDollScriptEnded = true;
            } else {
                HD_T;
                HD_SAY(seh_me, "TEXT_QST_083_TEACHER_DONT_SWAP");
                break;
            }
        } else if (!ParentClass->KidIntroDone && hero_has_dolls) {
            HD_T;
            CScriptGameResourceObjectScriptedThingBase seh_kid;
            KidIntro = Game->GetNearestWithDefName(Game->GetHero(), "CREATURE_BS_VILLAGER_BOY");
            if (KidIntro.GetName() != "") {
                HD_T;
                KidIntro = Game->GetNearestWithDefName(KidIntro, "CREATURE_BS_VILLAGER_BOY");
            }
            Game->SetThingHasInformation(KidIntro, false, true, false);
            Game->StartScriptingEntity(KidIntro, seh_kid, SCRIPT_AI_PRIORITY_HIGHEST);
            seh_kid.FollowThing(Game->GetHero(), 1.0f, true);
            Game->NewScriptFrame();
            HD_T;
            while (!IsDistanceBetweenThingsUnder(Game->GetHero(), KidIntro, 5.0f)) {
                Game->NewScriptFrame();
                HD_T;
            }
            HD_T;
            while (!Game->IsHeroControlledByPlayer() || Game->IsInCutscene() || Game->IsHeroInTavernGame()) {
                Game->NewScriptFrame();
                HD_T;
            }
            HD_T;
            seh_kid.ClearCommands();
            {
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                HD_SAY(seh_kid, "TEXT_QST_083_DOLLKID_INTRODUCTION");
            }
            Game->ClearThingHasInformation(KidIntro);
            if (seh_kid.IsValid())
                seh_kid.Release();
            Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_DOLLS", Game->GetActiveQuestName(), false);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HERO_DOLLS_OBJECTIVE_01", "", "");
            ParentClass->KidIntroDone = true;
        } else {
            HD_T;
            if (Game->GetHero().MsgReceivedInventoryItem()) {
                HD_T;
                if (ParentClass->HeroHasAnyDolls()) {
                    HD_T;
                    hero_has_dolls = true;
                }
            }
        }
    }
    HD_T;
    Game->ClearThingHasInformation(Me);
    if (seh_me.IsValid())
        seh_me.Release();
    PMasterData->HeroDollsScriptUsingTeacher = false;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
