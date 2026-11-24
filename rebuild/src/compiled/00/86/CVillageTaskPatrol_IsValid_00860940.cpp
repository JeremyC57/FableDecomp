// Leaf: ?IsValid@CVillageTaskPatrol@@UBE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CVILLAGETASKPATROL
#include "engine/CVillageTaskPatrol.h"
#include "engine/CVillageTaskPatrol.h"  // retyped onto the PDB layout; byte parity re-verified
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
bool CVillageTaskPatrol::IsValid() const
{
    CVillageTaskPatrol* task = const_cast<CVillageTaskPatrol*>(this); return ((int)(((CBaseIntelligentPointer*)(reinterpret_cast<char*>(task)+8))->GetPItem())) != 0; }
// Flatten route (flatten_pilot v4, NF324): CVillageTaskPatrol_IsValid -> CVillageTaskPatrol::IsValid, the RTTI-placed PDB member; class model: engine/CVillageTaskPatrol.h under FABLE_RETAIL_CVILLAGETASKPATROL (preamble of CCountedPointer_DeleteData_0065d258.cpp).
