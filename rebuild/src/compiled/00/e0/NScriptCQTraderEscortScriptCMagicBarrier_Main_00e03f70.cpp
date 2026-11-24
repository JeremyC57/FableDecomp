// NScript::CQ_TraderEscortScript::CMagicBarrier::Main() -- Maintain the two forcefield effects, then replace them with the off effects when the quest resumes.
// Leaf: ?Main@CMagicBarrier@CQ_TraderEscortScript@NScript@@EAEXXZ
// Flags: default
// Grade: RELOCATION_MATCH
// Inline-budget: expose the handle assignment at the four respawn sites, as recorded in PROBES.md.
// Inferred: parentheses on the returned angle preserve the x87 store schedule at the six creation sites.

#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_compiler.h"
#include "engine/TRetVoid.h"
#include "engine/CQ_TraderEscortScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/C3DVector.h"

inline CScriptThing::~CScriptThing() {}
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline const C3DVector& CScriptThing::GetPos() const
{
    CScriptThing* implementation = PImp.Data;
    if (!implementation) return NullCoord;
    return implementation->GetPos();
}

inline float CScriptThing::GetAngleXY() const
{
    if (PImp.IsNull()) return 0.0f;
    return (PImp->GetAngleXY());
}

inline bool CScriptThing::IsDead() const { return !IsAlive(); }

void NScript::CQ_TraderEscortScript::CMagicBarrier::Main()
{
    TRetVoid _Fail_Val;
    CScriptThing BarrierLocation = Game->GetThingWithScriptName("BarrierFX");
    CScriptThing ForceField1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", BarrierLocation.GetPos(), "", BarrierLocation.GetAngleXY(), false, false);
    CScriptThing ForceField2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", BarrierLocation.GetPos(), "", BarrierLocation.GetAngleXY(), false, false);
    while (!ParentClass.TradersShouldBeScared) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ForceField1.IsDead() && ForceField2.IsDead()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ForceField1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", BarrierLocation.GetPos(), "", BarrierLocation.GetAngleXY(), false, false);
            ForceField2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", BarrierLocation.GetPos(), "", BarrierLocation.GetAngleXY(), false, false);
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptThing RockTroll = Game->GetThingWithScriptName("DarkwoodRockTroll");
    while (ParentClass.TradersShouldBeScared) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ForceField1.IsDead() && ForceField2.IsDead()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ForceField1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", BarrierLocation.GetPos(), "", BarrierLocation.GetAngleXY(), false, false);
            ForceField2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", BarrierLocation.GetPos(), "", BarrierLocation.GetAngleXY(), false, false);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->RemoveThing(ForceField1, false, true);
    Game->RemoveThing(ForceField2, false, true);
    Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01_OFF", BarrierLocation.GetPos(), "", BarrierLocation.GetAngleXY(), false, false);
    Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02_OFF", BarrierLocation.GetPos(), "", BarrierLocation.GetAngleXY(), false, false);
    Game->RemoveThing(Me, false, true);
}