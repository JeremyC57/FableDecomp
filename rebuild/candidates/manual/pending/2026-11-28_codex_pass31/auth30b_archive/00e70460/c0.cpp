// 00e70460 NScript::CV_DemonDoorsScript::CDF_BeEvil::Main
// Leaf: ?Main@CDF_BeEvil@CV_DemonDoorsScript@NScript@@EAEXXZ
// Flags:
#define FABLE_RETAIL_DEMON_DOORS_REGISTRATION_PASS29
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
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
class CScriptGameResourceObjectScriptedThingBase;
// Inline-budget: the release of seh_me on the first terminating branch expands through the delete.
template <> FABLE_FORCEINLINE void CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::ReleaseData()
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
class CScriptGameResourceObjectMovieBase;
// Inline-budget: the movie handle uses the same expanded final-release path.
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

#include "engine/CV_DemonDoorsScript.h"
namespace NScript {
inline CV_DemonDoorsScript::CV_DemonDoorsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data,game) {}
CScriptBase* FABLE_FASTCALL CV_DemonDoorsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) { return new CV_DemonDoorsScript(data,game); }
}

#include "engine/CEntityScriptBinding.h"
inline void NScript::CV_DemonDoorsScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_ShineLight", this, &CDF_ShineLight::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_ThunderHelmet", this, &CDF_ThunderHelmet::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_CombatMult", this, &CDF_CombatMult::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_Dragon", this, &CDF_Dragon::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_SummonsCreatures", this, &CDF_SummonsCreatures::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_BeEvil", this, &CDF_BeEvil::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_DoDamage", this, &CDF_DoDamage::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_EatPies", this, &CDF_EatPies::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_GiveGift", this, &CDF_GiveGift::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_DressAsBandit", this, &CDF_DressAsBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_ThreeSuits", this, &CDF_ThreeSuits::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_MarryLadyGrey", this, &CDF_MarryLadyGrey::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_SexMad", this, &CDF_SexMad::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_KeyHoarder", this, &CDF_KeyHoarder::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
}

namespace NScript {
inline CCountedPointer<CEntityScriptBase> FABLE_FASTCALL CV_DemonDoorsScript::CDF_BeEvil::Alloc(CV_DemonDoorsScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CDF_BeEvil(parent, data, me));
}
inline CV_DemonDoorsScript::CDF_BeEvil::CDF_BeEvil(CV_DemonDoorsScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game, me), ParentClass(parent)
{
    PMasterData = static_cast<CQ_SunnyvaleMasterData*>(data);
}
inline CScriptBase* CV_DemonDoorsScript::CDF_BeEvil::GetParentScript() const { return &ParentClass; }
}

inline void NScript::CV_DemonDoorsScript::CDF_BeEvil::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    {
        // Inferred: guard names the original anonymous predicate only for this expression.
        // The native constructor is inlined and the ordinary destructor is called immediately after SetPredicate.
        const CPredicateBase& guard = CIsThingAlive(Me);
        SetPredicate(guard);
        // Inline-budget: split the scope exit from construction. The plain expression expands teardown,
        // while applying zero depth to the whole statement removes 16 bytes of inlined construction.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    }
#ifdef _MSC_VER
#pragma inline_depth()
#endif

    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) {
            _Fail_Val.Return();
            // Inline-budget: this early return calls the scripted resource destructor.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            return;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        }
    }
    if (IsActiveThreadTerminating()) {
            _Fail_Val.Return();
            // Inline-budget: this early return calls the scripted resource destructor.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            return;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        }
    Game.SetThingPersistent(Me, true);
    Game.EntitySetAsDamageable(Me, false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game.EntitySetAsKillable(Me, false, false);
    CScriptThing region_exit = Game.GetNearestWithDefName(Me, "REGION_EXIT_POINT");
    Game.SetRegionExitAsActive(region_exit, false);
    EvilEnough = false;
    for (; true; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
        while (!IsDistanceBetweenThingsUnder(Me, Game.GetHero(), (float)pGlobalScriptDef->DemonDoor_BeEvil_TriggerDistance)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
        }
        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
        float initial_morality = Game.GetHeroMorality();
        for (; true; Game.NewScriptFrame()) {
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            bool at_max_morality = Game.GetHeroMoralityCategory() == MORALITY_SUPER_EVIL;
            if (at_max_morality || initial_morality - Game.GetHeroMorality() >= pGlobalScriptDef->DemonDoor_BeEvil_EvilnessNeeded) {
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                EvilEnough = true;
            }
            if (IsDistanceBetweenThingsOver(Me, Game.GetHero(), (float)pGlobalScriptDef->DemonDoor_BeEvil_TriggerDistance))
                break;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if (EvilEnough) {
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (!at_max_morality) {
                        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_088_BE_EVIL_OPENING", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                            }
                            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                        }
                    } else {
                        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_088_BE_EVIL_ALREADY_EVIL_OPENING", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                            }
                            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                        }
                    }
                    ParentClass.FadeOutFaceAndOpenDoor(Me, "DD_BeEvil");
                    Game.SetRegionExitAsActive(region_exit, true);
                    return;
                } else {
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_088_BE_EVIL_INTRO", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                        }
                        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    }
                }
            }
        }
        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    }

}

