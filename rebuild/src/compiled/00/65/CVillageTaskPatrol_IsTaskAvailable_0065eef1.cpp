// Leaf: ?CVillageTaskPatrol_IsTaskAvailable@@YI_NPAUS@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct S { char pad[0x10]; char sub[4]; };
bool __fastcall CVillageTaskPatrol_IsTaskAvailable(S* task)
{
    return ((int)(((CBaseIntelligentPointer*)(reinterpret_cast<char*>(task) + 0x10))->GetPItem())) == 0;
}