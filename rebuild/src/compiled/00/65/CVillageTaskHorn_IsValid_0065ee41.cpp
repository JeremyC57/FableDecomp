// Leaf: ?IsValid@CVillageTaskHorn@@UBE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CVILLAGETASKHORN
#include "engine/CVillageTaskHorn.h"
#include "engine/CVillageTaskHorn.h"  // retyped onto the PDB layout; byte parity re-verified
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
bool CVillageTaskHorn::IsValid() const
{
    CVillageTaskHorn* task = const_cast<CVillageTaskHorn*>(this); return ((int)(((CBaseIntelligentPointer*)(reinterpret_cast<char*>(task)+8))->GetPItem())) != 0; }
// Flatten route (flatten_pilot v4, NF324): CVillageTaskHorn_IsValid -> CVillageTaskHorn::IsValid, the RTTI-placed PDB member; class model: engine/CVillageTaskHorn.h under FABLE_RETAIL_CVILLAGETASKHORN (preamble of CCountedPointerCVillageTaskBase_CCountedPointerCVillageTaskHorn_0065db2a.cpp).
