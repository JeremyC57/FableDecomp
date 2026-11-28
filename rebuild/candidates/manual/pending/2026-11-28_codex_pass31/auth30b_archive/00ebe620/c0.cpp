// 00ebe620 NScript::CV_OracleScript::CVO_Woman::Main
// Leaf: ?Main@CVO_Woman@CV_OracleScript@NScript@@EAEXXZ
// Flags:
#define FABLE_RETAIL_DEMON_DOORS_REGISTRATION_PASS29
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_WHITE_BALVERINE_GUARDS_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CISTHINGCONSCIOUS_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_SCRIPT_MOVE_ENUM
#define FABLE_RETAIL_CQ_PRISONRACESCRIPT_GUARD_PASS27
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
// Header binding correction: NScript expands the conversion and calls GetString at 0x00415D70.
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR

// Inline-budget: shared pause constructor expands at both dialogue sites as in the adjacent DoDamage::Main.
#define FABLE_RETAIL_BE_EVIL_MORALITY_PASS29
#define FABLE_RETAIL_MURDER_ASSASSIN_GUARD_PASS26
#include "fable_compiler.h"
#include "engine/CCountedPointer.h"
// The retail entity factories allocate a control block only for a non-null object.
template <class T>
inline CCountedPointer<T>::CCountedPointer(T* data) : Data(data)
{
    if (Data)
        Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    else
        Info = 0;
}
template <class T>
inline void FABLE_FASTCALL CCountedPointer<T>::DeleteData(void* data)
{
    delete static_cast<T*>(data);
}

class CScriptGameResourceObjectMovieBase;
// Inline-budget: the movie deleting destructor expands the counted pointer release.
template <> FABLE_FORCEINLINE void CCountedPointer<CScriptGameResourceObjectMovieBase>::ReleaseData()
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data = 0;
    Info = 0;
}

// Inline-budget: the movie deleting destructor expands PImp and its release.
template <> FABLE_FORCEINLINE CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer() { ReleaseData(); }

#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EScriptAIPriority.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;

#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CPersonalScript_GlobalThingsScript.h"
namespace NScript {
inline CPersonalScript_GlobalThingsScript::COF_MainGates::COF_MainGates(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me)
 : CEntityScriptBase(parent.Game,me), ParentClass(parent)
{
    PMasterData = static_cast<CQ_SunnyvaleMasterData*>(data);
}
CCountedPointer<CEntityScriptBase> FABLE_FASTCALL CPersonalScript_GlobalThingsScript::COF_MainGates::Alloc(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new COF_MainGates(parent,data,me));
}
inline CScriptBase* CPersonalScript_GlobalThingsScript::COF_MainGates::GetParentScript() const {return &ParentClass;}
}
inline void NScript::CPersonalScript_GlobalThingsScript::COF_MainGates::Main() {
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!Game.IsQuestActive("Q_OrchardFarmRaid")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.OpenDoor(Me);
    }
}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline CScriptThing::~CScriptThing() {}

#include "engine/ECutsceneBehaviour.h"
#include "engine/C3DVector.h"
#include "engine/CQ_WhiteBalverineKnotholeGladeScript.h"
#include "engine/CIsThingConscious.h"
#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) {}
// Inline-budget: the leaf expands the predicate destructor and calls the script thing destructor.
#ifdef _MSC_VER
#pragma inline_depth(2)
#endif
FABLE_FORCEINLINE NScript::CIsThingConscious::~CIsThingConscious() {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline bool NScript::CIsThingConscious::operator()() const { return Thing.IsAlive() && !Thing.IsUnconscious(); }
inline NScript::CPredicateBase* NScript::CIsThingConscious::Clone() const { return new CIsThingConscious(Thing); }
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
inline CScriptThing::CScriptThing() {}
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
namespace NScript {
CCountedPointer<CEntityScriptBase> FABLE_FASTCALL CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::Alloc(
    CQ_WhiteBalverineKnotholeGladeScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CWB_Guard1(parent, data, me));
}
inline CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::CWB_Guard1(
    CQ_WhiteBalverineKnotholeGladeScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game, me), ParentClass(parent)
{
    PMasterData = static_cast<CQ_SunnyvaleMasterData*>(data);
}
CScriptBase* CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::GetParentScript() const { return &ParentClass; }
void CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::Init() {}
void CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::OnPredicateFail() {}
}

// Inline-budget: the native 41-byte STLport overload at 00CD3DA4 calls both
// resource special members and the two-argument overload. Its frame pointer is live.
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
template<>
inline void std::vector<CScriptGameResourceObjectScriptedThingBase>::resize(unsigned int n)
{
    resize(n, CScriptGameResourceObjectScriptedThingBase());
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
template void std::vector<CScriptGameResourceObjectScriptedThingBase>::resize(unsigned int);

inline void NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
    CScriptThing G1 = Game.GetThingWithScriptName("WB_Guard1Pos");
    seh_me.MoveToPosition(G1.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
    while (!IsDistanceBetweenThingsUnder(Me, G1, 5.0f)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    while (!GetMasterData()->WhiteBalverineFinished) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        CScriptThing Balv = Game.GetThingWithScriptName("WB_WhiteBalverine");
        if (IsDistanceBetweenThingsUnder(Me, Balv, 30.0f)) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
            float local_range = 2.0f;
            C3DVector local_position = Game.GetHero().GetPos();
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                seh_me.MoveToPosition(local_position, 0.0f, local_movetype, false, true);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.RemoveThing(Me, false, true);
}


#define FABLE_RETAIL_ORACLE_WOMAN_REGISTRATION_PASS29
#include "engine/EHeroAbility.h"
#define FABLE_RETAIL_ORACLE_WOMAN_PASS29
#include "engine/CV_OracleScript.h"

#include "engine/CEntityScriptBinding.h"
namespace NScript {
inline CV_OracleScript::CV_OracleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) {}
CScriptBase* FABLE_FASTCALL CV_OracleScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) { return new CV_OracleScript(data,game); }
inline void CV_OracleScript::Main() {
    AddEntityScriptBinding(new CEntityScriptBinding<CV_OracleScript>("VO_OracleMarker", this, &CVO_OracleMarker::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_OracleScript>("VO_Woman", this, &CVO_Woman::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
}
inline CV_OracleScript::CVO_Woman::CVO_Woman(CV_OracleScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game,me),ParentClass(parent) { PMasterData=static_cast<CQ_SunnyvaleMasterData*>(data); }
inline CCountedPointer<CEntityScriptBase> FABLE_FASTCALL CV_OracleScript::CVO_Woman::Alloc(CV_OracleScript& parent, CScriptDataBase* data,const CScriptThing& me) { return CCountedPointer<CEntityScriptBase>(new CVO_Woman(parent,data,me)); }
inline CScriptBase* CV_OracleScript::CVO_Woman::GetParentScript() const { return &ParentClass; }
}

inline void NScript::CV_OracleScript::CVO_Woman::Main()
{
    TRetVoid _Fail_Val;
    {
        // Inferred: guard scopes the anonymous predicate through SetPredicate.
        const CPredicateBase& guard = CIsThingAlive(Me);
        SetPredicate(guard);
        // Inline-budget: destroy the predicate out of line before the next frame.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    }
#ifdef _MSC_VER
#pragma inline_depth()
#endif

    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    }
    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    Game.EntitySetAsKillable(Me, false, false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game.SetThingHasInformation(Me, false, false, false);
    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    do {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (!SaidIntro) {
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_B20_WOMAN_ON_TALK", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    }
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                }
                SaidIntro = true;
            } else {
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_B20_WOMAN_REMINDER", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    }
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }

#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            CScriptGameResourceObjectMovieBase speech_movie;
#ifdef _MSC_VER
#pragma inline_depth()
#endif

            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_B20_WOMAN_ON_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                }
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            }
            Game.EntitySetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(), Me);
        }
        Game.NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
