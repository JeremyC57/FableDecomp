// 00ea2080 NScript::CV_KnotholeGladeGatesScript::ControlMainBalverines
// Leaf: ?ControlMainBalverines@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ
// Flags: /Gr
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_KnotholeGladeGatesScript.h"
#include "engine/TRetVoid.h"
#include "engine/CLevelScriptBase.h"
#include "stlport/fable_containers.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
// Inline-budget: the ordinary inline form calls operator= at GetHero (52-byte reduction); retail expands both assignments.
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
// Inline-budget: retail expands PImp null handling and the GetPos virtual dispatch at each camera or creature position use.
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { if(PImp.IsNull())return NullCoord; return PImp->GetPos(); }
bool FABLE_FASTCALL AreAllThingsInVectorDead(const std::vector<CScriptThing>& things);   // 0x00CBED00
void NScript::CV_KnotholeGladeGatesScript::ControlMainBalverines()
{
    TRetVoid _Fail_Val;
    std::vector<CScriptThing> KGG_Balverines;
    CScriptThing b2m = Game.GetThingWithScriptName("KGG_Balv1");
    CScriptThing Target;
    if (false) {} else for (long i = 0; i < 1; ++i) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Target = Game.GetRandomThingWithScriptName("KGG_VillagerOutside");
        if (Target.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (Game.IsCameraPosOnScreen(b2m.GetPos())) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing BalverineToAdd = Game.CreateCreatureNearby("CREATURE_BALVERINE_01", b2m.GetPos(), 6.0f, "KGG_Balverine", false);
            Game.PlayCriteriaSoundOnThing(TheGate, "KNG_HORN");
            Game.SetThingPersistent(BalverineToAdd, true);
            Game.GiveThingBestEnemyTarget(BalverineToAdd, Target);
            Game.EntitySetAsOpinionSource(BalverineToAdd, "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
            KGG_Balverines.push_back(BalverineToAdd);
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Target = Game.GetHero();
            while (Game.IsCameraPosOnScreen(b2m.GetPos())) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing BalverineToAdd = Game.CreateCreatureNearby("CREATURE_BALVERINE_01", b2m.GetPos(), 6.0f, "KGG_Balverine", false);
            Game.PlayCriteriaSoundOnThing(TheGate, "KNG_HORN");
            Game.SetThingPersistent(BalverineToAdd, true);
            Game.GiveThingBestEnemyTarget(BalverineToAdd, Target);
            KGG_Balverines.push_back(BalverineToAdd);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!AreAllThingsInVectorDead(KGG_Balverines)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}