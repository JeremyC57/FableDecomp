// NScript::CV_RandomPopulationSimScript::CRandomPopulationCreature::Init() -- a randomly populated creature's entity-script Init: not yet attached to a village, pick the furthest RandomPopulationExit from Me as its exit, classify Me by script name (villager 0, guard 1, bandit 2; a villager also gets gold scaled down by the current region's population scale), and count it alive in the parent's per-type tally
// Leaf: ?Init@CRandomPopulationCreature@CV_RandomPopulationSimScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members References->RefCount are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC1CD0, 540 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the entity-script vtable 0x012F1390 [slot 0 scalar deleting dtor 0x00EC2830, slot 1
//   Main 0x00EC1EF0 (landed), slot 2 this row, slot 5 the landed OnPredicateFail 0x00EC1C20] that the landed
//   CRandomPopulationCreature::Alloc 0x00EC1B70 stores; Ego_r.pdb / FableWin.pdb name set
//   ?Init@CRandomPopulationCreature@CV_RandomPopulationSimScript@NScript@@EAEXXZ (Ego_r.pdb order Alloc, OnPredicateFail, Init,
//   Main matches the retail order 0x00EC1B70 / 0x00EC1C20 / 0x00EC1CD0 / 0x00EC1EF0). Catalog `sub_00ec1cd0`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CBasicString<char>::Compare 0x00411570 (__fastcall, the
//   inline CCharString == const char*; strcmp intrinsic on the null arm), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC,
//   rand 0x00BFEB16, _ftol 0x00BFEA70; CScriptThing::GetName slot 1 (called virtually on Me); inline CScriptThing::operator=
//   and ~CScriptThing (vtable 0x01238C8C, counted-pointer release); CGameScriptInterfaceBase slots 78 GetFurthestWithScriptName,
//   520 EntityGiveGold; pGlobalScriptDef 0x0143E90C.
// Literals: "RandomPopulationExit" 0x012F13AC, "RandomPopulationVillager" 0x012F1374, "RandomPopulationGuard" 0x012F1354,
//   "RandomPopulationBandit" 0x012F133C, "" 0x0122D70E; 1.0f 0x0122DED8.
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT: CEntityScriptBase {vptr, Game +4, Me +8}); Ego_r.pdb
//   CRandomPopulationCreature (48 bytes, no STL members): ParentClass +0x14, PMasterData +0x18, Exit +0x1c, MyType +0x28,
//   AttachedToVillage +0x2c. CV_RandomPopulationSimScript : CLevelScriptBase<CQ_SunnyvaleMasterData> (Ego_r.pdb first member
//   at 0x5c), retail = Ego_r.pdb - 0x14: CurrentPopulationScale[4] +0x48, CurrentPopulationRegion +0x5c, CreaturesAlive[3]
//   +0x60. CScriptDef retail = Ego_r.pdb - 0x5c (the landed AssassinThreatDistSq +0x720 / Ego_r +0x77c):
//   RandomPopulationTraderGoldMinimum +0x724, RandomPopulationTraderGoldVariation +0x728.
// Model: the landed CQ_TraderEscortScript::CDarkwoodTrader::Init 0x00E04BD0 (inline CCharString == const char*) and the
//   landed CTCAssassinRush::SetFinalAction 0x008CEAE0 (inline counted-pointer assignment, source fields read first).
// Shape: the gold is a named local (that keeps the `fimul` on its stack slot; inline it gives `fild`/`fmulp`) and the def
//   is read before rand() (kept in esi for the `idiv`) while the minimum is read through the global again after the call.
//   agentNF60: pGlobalScriptDef is the CDefPointer<const CScriptDef> of engine/fable_script_def_global.h (was modelled as a plain
//   pointer read into a `def` local). VC7.1 evaluates the operands of `+` / `%` right to left, so `Minimum + rand() % Variation`
//   reads Variation's def before rand() and Minimum's after it (retail 0x00EC1E4B / 0x00EC1E63); `rand() % ... + Minimum` with
//   operator-> reads both before rand() (DIFFER 537v540).
#include "fable_compiler.h"
#include <string.h>
#include <stdlib.h>
// agentNF60: CCharString / CCharStringData / CBasicString<char> are fable_string.h's (were local models); CBasicString::operator==(const char*)
//   expands in place (FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE) over the protected static Compare 0x00411570.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#include "fable_string.h"
#pragma pack(push, 1)
#pragma pack(pop)
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"

FABLE_FORCEINLINE CScriptThing::~CScriptThing() { }         // out of line 0x004AA840; inline here
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other)   // out of line 0x008AB980; inline here
{
    PImp = other.PImp;
    return *this;
}

#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_RandomPopulationSimScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    float CurrentPopulationScale[4];                    // +0x48
    bool  HighDetailSimulationActive;                   // +0x58
    long  CurrentPopulationRegion;                      // +0x5c
    long  CreaturesAlive[3];                            // +0x60
    class CRandomPopulationCreature;
};
class CV_RandomPopulationSimScript::CRandomPopulationCreature : public CEntityScriptBase {
public:
    CV_RandomPopulationSimScript* ParentClass;          // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;          // +0x18
    CScriptThing                  Exit;                 // +0x1c
    long                          MyType;               // +0x28
    bool                          AttachedToVillage;    // +0x2c
private:
    virtual void Main();                                // slot 1 0x00EC1EF0
    virtual void Init();                                // slot 2
};

void CV_RandomPopulationSimScript::CRandomPopulationCreature::Init()
{
    AttachedToVillage = false;
    Exit = Game->GetFurthestWithScriptName(Me, "RandomPopulationExit");
    if (Me.GetName() == "RandomPopulationVillager") {
        MyType = 0;
        long gold = pGlobalScriptDef->RandomPopulationTraderGoldMinimum + rand() % pGlobalScriptDef->RandomPopulationTraderGoldVariation;
        Game->EntityGiveGold(Me, (long)((1.0f - ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion]) * gold));
    } else if (Me.GetName() == "RandomPopulationGuard")
        MyType = 1;
    else if (Me.GetName() == "RandomPopulationBandit")
        MyType = 2;
    else
        MyType = 0;
    ParentClass->CreaturesAlive[MyType]++;
}
}   // namespace NScript