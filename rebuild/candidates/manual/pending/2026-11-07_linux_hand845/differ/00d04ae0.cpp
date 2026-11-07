// NScript::CQ_BanditCampBossBattleScript::CAssassin1::Main() -- the first assassin in the bandit camp: alive filter on Me, scripted control, then until the assassins are attacked or their cutscene is triggered, mark them under attack when the hero hits him, and when the hero talks to him say he is not needed (gate 3 open), or ask (intro, then repeat) whether the hero will pay for the assassins; a yes with enough gold triggers the cutscene, takes the gold and grants small experience
// Grade: DIFFER(3847v3623) -- the documented caller-size inline budget: the implicit MovieBase dtor at the three scope ends and the speech-exit pads is called (COMDAT 0x006E7B80) where retail expands it, so the pads do not cross-jump; probe b (two idle loops) graded RELOCATION_MATCH under the diagnostic S15 inline-budget meter only (never in this source)
// Next idea: a pragma-free spelling that lowers the caller's inline cost so the movie dtor expands at those sites; start from probe b's two-idle-loop shape
// Leaf: ?Main@CAssassin1@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D04AE0, 3623 bytes; entity vtable 0x012C8B9C slot 1.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSIN1
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/EHeroAbility.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
#define A1_T if (IsActiveThreadTerminating()) return
#define A1_SAY(key) do { \
    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
        seh_me.Speak(Game->GetHero(), key, GROUP_SELECT_NONE, false, true, false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            A1_T; \
        } \
        A1_T; \
    } } while (0)
#define A1_ANSWER(answer) \
    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) { \
        Game->NewScriptFrame(); \
        A1_T; \
    } \
    A1_T

void NScript::CQ_BanditCampBossBattleScript::CAssassin1::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    A1_T;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        A1_T;
    }
    A1_T;
    bool asked = false;
    while (!ParentClass.AssassinsUnderAttack && !ParentClass.AssassinCutsceneTriggered) {
        Game->NewScriptFrame();
        A1_T;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            A1_T;
            ParentClass.AssassinsUnderAttack = true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            A1_T;
            if (ParentClass.Gate3Open) {
                A1_T;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                A1_SAY("TEXT_QST_009_ASSASSIN1_NOT_NEEDED");
            } else {
                A1_T;
                if (!asked) {
                    A1_T;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    A1_SAY("TEXT_QST_009_ASSASSIN1_INTRO");
                    Game->GiveHeroYesNoQuestion("TEXT_QST_009_ASSASSIN1_INTRO_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    A1_ANSWER(answer);
                    if (answer == 1) {
                        A1_T;
                        if (Game->GetHeroGold() < pGlobalScriptDef->BAC_AssassinGoldAmount) {
                            A1_T;
                            A1_SAY("TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_POOR");
                        } else {
                            A1_T;
                            A1_SAY("TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES");
                            ParentClass.AssassinCutsceneTriggered = true;
                            Game->GiveHeroGold((long)-pGlobalScriptDef->BAC_AssassinGoldAmount);
                            Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        }
                    } else {
                        A1_T;
                        A1_SAY("TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_NO");
                    }
                    ParentClass.TalkedToAssassin = true;
                    asked = true;
                } else {
                    A1_T;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->GiveHeroYesNoQuestion("TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    A1_ANSWER(answer);
                    if (answer == 1) {
                        A1_T;
                        if (Game->GetHeroGold() < pGlobalScriptDef->BAC_AssassinGoldAmount) {
                            A1_T;
                            A1_SAY("TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_POOR");
                        } else {
                            A1_T;
                            A1_SAY("TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES");
                            ParentClass.AssassinCutsceneTriggered = true;
                            Game->GiveHeroGold((long)-pGlobalScriptDef->BAC_AssassinGoldAmount);
                            Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        }
                    } else {
                        A1_T;
                        A1_SAY("TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_NO");
                    }
                }
            }
        }
    }
    A1_T;
    if (ParentClass.AssassinCutsceneTriggered || ParentClass.AssassinsUnderAttack) {
        A1_T;
        Game->ClearThingHasInformation(Me);
        seh_me.Release();
        for (;;) {
            Game->NewScriptFrame();
            A1_T;
        }
    }
    A1_T;
    for (;;) {
        Game->NewScriptFrame();
        A1_T;
    }
}
