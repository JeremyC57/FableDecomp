// 00e16e00 NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::Main
// Leaf: ?Main@CWB_Guard1@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ
// Flags:
#include "fable_compiler.h"
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
#include "engine/EScriptAIPriority.h"
#include "engine/ECutsceneBehaviour.h"
#include "engine/C3DVector.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/TRetVoid.h"
#include "engine/CQ_WhiteBalverineKnotholeGladeScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CIsThingConscious.h"
#include "engine/fable_script_distance.h"
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
inline CScriptThing::~CScriptThing() {}
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