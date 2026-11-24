// Leaf: ?IsToCauseRecoil@CCombatAction_GenericStrikeResponseKnockdown@@UBE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEKNOCKDOWN_RTTI_DECLS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatAction_GenericStrikeResponseKnockdown.h"
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
extern bool __fastcall r_h2(void*);
bool CCombatAction_GenericStrikeResponseKnockdown::IsToCauseRecoil() const
{
    void* action = (void*)this; return r_h2(((void*)(((CBaseIntelligentPointer*)(reinterpret_cast<char*>(action)+8))->GetPItem()))); }
// Flatten route (flatten_pilot v4, NF324): CCombatAction_GenericStrikeResponseKnockdown_IsToCauseRecoil -> CCombatAction_GenericStrikeResponseKnockdown::IsToCauseRecoil, the RTTI-placed PDB member; class model: engine/CCombatAction_GenericStrikeResponseKnockdown.h opt-in FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN, FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEKNOCKDOWN_RTTI_DECLS (preamble of CThingCreatureBase_ReactToGenericStrike_00667450.cpp).
