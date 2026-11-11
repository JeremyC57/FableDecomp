// Leaf: ?CCreatureActionBase_GetThingManager@@YIPAVCThingManager@@PAVCCreatureActionBase@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct Sub; 
class CThingManager;
// agentNF15: the global stand-in `g_thingmgr` at 0x013B8A1C is the private static CThing::PCurrentThingManager
//   (?PCurrentThingManager@CThing@@0PAVCThingManager@@A, engine/CThing.h, 65 landed rows), read through the inline public
//   accessor as retail code outside CThing does. Partial view of CThing (only the static and its accessor).
class CThing {
private:
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C (private static)
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (inline)
};
CThingManager* __fastcall CCreatureActionBase_GetThingManager(CCreatureActionBase* action)
{
    ((CBaseIntelligentPointer*)(reinterpret_cast<Sub*>(reinterpret_cast<char*>(action) + 8)))->GetPItem();
    return CThing::GetPCurrentThingManager();
}