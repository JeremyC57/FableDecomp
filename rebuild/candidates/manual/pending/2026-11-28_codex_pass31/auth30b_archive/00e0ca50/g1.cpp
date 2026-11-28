// Leaf: ?Main@CUndeadRisingFirefly@CQ_UndeadRisingScript@NScript@@EAEXXZ
// Flags: /O2 /Oy
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_UNDEAD_RISING_FIREFLY_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#include "fable_compiler.h"
#include "engine/TRetVoid.h"
#include "engine/C3DVector.h"
#include "engine/ECutsceneBehaviour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_UndeadRisingScript.h"
#include "engine/CTimer.h"
// Both original public tables list ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A; retail 0143E8F8.
extern CGameScriptInterfaceBase* pDSTGame;
template<> FABLE_FORCEINLINE void CCountedPointer<CScriptThing>::ShareData(CScriptThing* data,CCPPointerInfo* info) { if(Info!=info) { ReleaseData(); Data=data; Info=info; if(Info) ++Info->RefCount; } }
template<> FABLE_FORCEINLINE CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>& other) { ShareData(other.Data,other.Info); return *this; }
// Inline-budget: retail calls this assignment out of line (0x008AB980) from Main; its copy expands ReleaseData (74 bytes).
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { CBaseClass::operator=(other); PImp = other.PImp; return *this; }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
// Inline-budget: both timer assignments expand into the original interface SetTimer slot.
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
void NScript::CQ_UndeadRisingScript::CUndeadRisingFirefly::Main()
{
    TRetVoid _Fail_Val;
    while (ParentClass.quest_status != CQ_UndeadRisingScript::GRUNT_BATTLE) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptThing grave = Game.GetRandomThingWithScriptName("UndeadRisingGrave");
    CScriptThing creature;
    CTimer SpawnTimer;
    SpawnTimer = 0;
    if (ParentClass.quest_status == CQ_UndeadRisingScript::GRUNT_BATTLE) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        creature = Game.CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD", grave.GetPos(), "UndeadRisingZombieAttacker", false);
        Game.EntitySetCutsceneBehaviour(creature, CUTSCENE_BEHAVIOUR_NOT_PAUSED);
        while (creature.IsAlive()) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        SpawnTimer = 7;
    }
    while (ParentClass.quest_status == CQ_UndeadRisingScript::GRUNT_BATTLE) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        grave = Game.GetRandomThingWithScriptName("UndeadRisingGrave");
        while (SpawnTimer.GetTime() > 0) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (ParentClass.quest_status == CQ_UndeadRisingScript::GRUNT_BATTLE) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        creature = Game.CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD", grave.GetPos(), "UndeadRisingZombieAttacker", false);
        Game.EntitySetCutsceneBehaviour(creature, CUTSCENE_BEHAVIOUR_NOT_PAUSED);
        while (creature.IsAlive()) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        SpawnTimer = 7;
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
}
