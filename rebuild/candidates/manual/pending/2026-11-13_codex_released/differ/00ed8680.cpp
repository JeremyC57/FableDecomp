// NScript::CV_TalentlessBardScript::CTB_Bard::Main() -- the Bowerstone bard: with the alive predicate, spawn the parent's attack and wander-centre watchers on him, then every frame: when the hero hits him (not with heal life), face the hero and say the on-hit line; when the hero talks to him, face the hero, play the intro or a random return line, ask whether to sing; yes and enough gold takes the fee and sings the verses with mandolin chords, yes without the fee says a random no-cash line, no says the rejected line
// Grade: DIFFER(3793v3652) -- the documented CIsThingAlive / CSpawnedFunc1 nested-copy blocker: retail calls the CScriptThing copy 0x004ABE90 for the CIsThingAlive member copy and the CSpawnedFunc1 Par1 copy inside the inlined ctor while it expands the by-value Me copy; plain VC7.1 expands all (row-local inline copy) or calls all (header copy); the register colouring after it follows
// Next idea: only inline-depth pragma regions reproduce the split (forbidden in rows); land once a pragma-free mechanism is found (docs/pipeline/INLINE_DEPTH_REVISIT.md)
// Leaf: ?Main@CTB_Bard@CV_TalentlessBardScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED8680, 3652 bytes; entity vtable 0x012F3498 slot 1.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SPRINTF
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSCRIPTBASE_ADDSPAWNEDFUNCTION_DEFAULT_REGION
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_TalentlessBardScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/CIsThingAlive.h"
#include "engine/EHeroAbility.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
CCharString FABLE_FASTCALL operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#define TB_T if (IsActiveThreadTerminating()) return
#define TB_SAY(key, method) \
    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
        seh_me.Speak(Game->GetHero(), key, method, false, true, false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            TB_T; \
        } \
        TB_T; \
    }
#define TB_START_SCRIPTING \
    if (seh_me.IsValid()) \
        seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        TB_T; \
    } \
    TB_T

void NScript::CV_TalentlessBardScript::CTB_Bard::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    TB_T;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CCharString speech;
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CV_TalentlessBardScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForAttack"), 0, ParentClass, &CV_TalentlessBardScript::WatchForAttack, Me));
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CV_TalentlessBardScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForResetWanderCentre"), 0, ParentClass, &CV_TalentlessBardScript::WatchForResetWanderCentre, Me));
    while (!IsActiveThreadTerminating()) {
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))
            && Game->GetHealth(Me) > 0.0f) {
            TB_T;
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false);
            TB_START_SCRIPTING;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            TB_SAY("TEXT_QST_163_ON_HIT", GROUP_SELECT_NONE);
            if (seh_me.IsValid())
                seh_me.Release();
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            TB_T;
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false);
            TB_START_SCRIPTING;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (!ParentClass->BardIntroComplete) {
                TB_T;
                TB_SAY("TEXT_QST_163_INTRO", GROUP_SELECT_NONE);
                ParentClass->BardIntroComplete = true;
            } else {
                TB_T;
                CCharString return_speech;
                return_speech.Sprintf("TEXT_QST_163_RETURN%d", rand() % 4 + 1);
                TB_SAY(return_speech, GROUP_SELECT_NONE);
            }
            Game->GiveHeroYesNoQuestion("TEXT_QST_163_QUESTION_SONG", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long answer;
            while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                Game->NewScriptFrame();
                TB_T;
            }
            TB_T;
            if (answer == 1) {
                TB_T;
                if (Game->GetHeroGold() > pGlobalScriptDef->BardSingingFee) {
                    TB_T;
                    Game->GiveHeroGold(-pGlobalScriptDef->BardSingingFee);
                    TB_SAY("TEXT_QST_163_START", GROUP_SELECT_RANDOM_NO_REPEAT);
                    for (long verse = 0; verse < pGlobalScriptDef->BardNumVersesPerVisit; verse++) {
                        TB_T;
                        Game->PlaySoundOnThing(Me, "SND_MANDOLINCHORD_01");
                        Game->Pause(2.0f);
                        speech = GetRandomSpeech();
                        TB_SAY(speech, GROUP_SELECT_NONE);
                        Game->Pause(1.0f);
                    }
                    TB_T;
                } else {
                    TB_T;
                    CCharString no_cash_speech;
                    no_cash_speech.Sprintf("TEXT_QST_163_NO_CASH%d", rand() % 2 + 1);
                    TB_SAY(no_cash_speech, GROUP_SELECT_NONE);
                }
            } else {
                TB_T;
                TB_SAY("TEXT_QST_163_REJECTED", GROUP_SELECT_RANDOM_NO_REPEAT);
            }
            if (seh_me.IsValid())
                seh_me.Release();
        }
        Game->NewScriptFrame();
    }
}
