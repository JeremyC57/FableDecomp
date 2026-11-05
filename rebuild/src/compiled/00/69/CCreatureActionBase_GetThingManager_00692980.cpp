// Leaf: ?CCreatureActionBase_GetThingManager@@YIPAVCThingManager@@PAVCCreatureActionBase@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct Sub; 
class CThingManager; extern CThingManager* g_thingmgr;
CThingManager* __fastcall CCreatureActionBase_GetThingManager(CCreatureActionBase* action)
{
    ((CBaseIntelligentPointer*)(reinterpret_cast<Sub*>(reinterpret_cast<char*>(action) + 8)))->GetPItem();
    return g_thingmgr;
}