// CThingFilter_IsCreatureThreatTo::operator()(const CThingCreatureBase&) const -- is the tested creature an enemy of the filter's creature
// Leaf: ??RCThingFilter_IsCreatureThreatTo@@QBE_NABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A7600, 14 bytes: `mov edx,[esp+4] ; mov ecx,[ecx] ; call 0x0076D2F0 ; ret 4`.
// NF28 (2026-11-12, queue land1011ed): was landed as std::_Cons_val<...> (a propagated manifest label over the same byte shape).
// Identity: Ego_r.pdb 0x005A2FB0 and FableWin.pdb 0x01F94540 ??RCThingFilter_IsCreatureThreatTo@@QBE_NABVCThingCreatureBase@@@Z
//   (FableWin also the ctor ??0CThingFilter_IsCreatureThreatTo@@QAE@ABVCThingCreatureBase@@@Z); Ego_r filter order ... IsCreatureThreatTo
//   0x005A2FB0, IsAICreatureAwareOf 0x005A2FC0, IsAwareOfThingIndirectly 0x005A2FD0, IsNavLineOfSightToThing 0x005A2FF0 = retail
//   0x006A7600 / 0x006A7610 / 0x006A7620 / 0x006A7640 (catalog: IsNavLineOfSightToThing 0x006A7640); the callee 0x0076D2F0 is
//   CTCEnemy::AreThingsEnemies (static __fastcall, both PDBs), called with the filter's creature (+0x00) and the tested creature;
//   the landed GetAllThingsInRoughArea<..., CFilter_Bind2<CThingFilter_IsCreatureThreatTo, CThingFilter_IsAwareOfThing>> 0x0084F850
//   (Ego_r 0x008469C0 instance) calls it.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCTARGETINGAI_ATTACK
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCLASS
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCEnemy.h"
class CThingFilter_IsCreatureThreatTo {           // no shared header: the one member the operator reads
public:
    const CThingCreatureBase* Creature;           // +0x00
    bool operator()(const CThingCreatureBase& thing) const;
};
bool CThingFilter_IsCreatureThreatTo::operator()(const CThingCreatureBase& thing) const
{
    return CTCEnemy::AreThingsEnemies(*Creature, thing);
}
