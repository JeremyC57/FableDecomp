// 00cfce90 NScript::CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets::Main
// Leaf: ?Main@CScytheBeforeGettingTablets@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ
// Flags:
// Inline-budget: the native predicate keeps its CScriptThing copy out of line.
// Native weak destructor selection is documented in LINK_REVIEW.json.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_SCYTHE_BEFORE_TABLETS_PASS27
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectScriptedThingBase;
template<> FABLE_FORCEINLINE CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::~CCountedPointer();
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CQ_AwakeningTheOracleScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_text.h"

inline CScriptThing::~CScriptThing() {}
namespace NScript {
#ifdef _MSC_VER
#pragma inline_depth(2)
#endif
inline CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline CIsThingAlive::~CIsThingAlive() {}

CCountedPointer<CEntityScriptBase> FABLE_FASTCALL CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets::Alloc(
        CQ_AwakeningTheOracleScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CScytheBeforeGettingTablets(parent, data, me));
}

inline CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets::CScytheBeforeGettingTablets(
        CQ_AwakeningTheOracleScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
CScriptBase* CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets::GetParentScript() const
{
    return ParentClass;
}
inline void CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets::Main()
{
    Game.EntitySetAsKillable(Me, false, false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game.SetIsPushableByHero(Me, false);
    Game.EntityAttachToScript(Me, Game.GetActiveQuestName());
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    TRetVoid _Fail_Val;
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.SetThingPersistent(Me, true);
    while (!ParentClass->PlayedOpeningCS) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.SetThingHasInformation(Me, false, false, false);
    Game.EntitySetFacingAngleTowardsThing(Me, Game.GetThingWithScriptName("MK_OW_SCYTHE3"), false);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, (EScriptAIPriority)4)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    long NextLine = 10;
    while (ParentClass->TabletsCollected < 4) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (!Game.TextEntryExists("TEXT_QST_B03_SCYTHE_ON_SPEAK_" + GFIntToCharString(NextLine))) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                NextLine = 10;
            }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_B03_SCYTHE_ON_SPEAK_" + GFIntToCharString(NextLine), GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    do { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); } while (false);
                }
                if (IsActiveThreadTerminating()) {
                    return _Fail_Val.Return();
                }
            }
            NextLine += 10;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_B03_SCYTHE_ON_HIT_NEW", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    do { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); } while (false);
                }
                if (IsActiveThreadTerminating()) {
                    return _Fail_Val.Return();
                }
            }
            Game.EntitySetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(), Me);
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.RemoveThing(Me, false, true);
}
}

// Inline-budget: preserve the out-of-line CScriptThing copy in the native factory.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline CEntityScriptBase::CEntityScriptBase(CGameScriptInterfaceBase& game, const CScriptThing& me)
    : Game(game), Me(me)
{}
#ifdef _MSC_VER
#pragma inline_depth()
#endif

// Inline-budget: expand the original counted release inside the native scripted-resource scalar.
template<> FABLE_FORCEINLINE CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::~CCountedPointer()
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data=0;
    Info=0;
}

inline bool NScript::CIsThingAlive::operator()() const { return Thing.IsAlive(); }
inline NScript::CPredicateBase* NScript::CIsThingAlive::Clone() const
{
    return new CIsThingAlive(*this);
}