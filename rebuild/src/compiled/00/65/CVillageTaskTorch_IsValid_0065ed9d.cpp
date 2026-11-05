// Leaf: ?CVillageTaskTorch_IsValid@@YI_NPAVCVillageTaskTorch@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#include "engine/CVillageTaskTorch.h"  // retyped onto the PDB layout; byte parity re-verified
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
bool __fastcall CVillageTaskTorch_IsValid(CVillageTaskTorch* task){ return ((int)(((CBaseIntelligentPointer*)(reinterpret_cast<char*>(task)+8))->GetPItem())) != 0; }
// Stand-in (owner rule 2026-11-05): CVillageTaskTorch_IsValid for CVillageTaskTorch::IsValid; RTTI vtable slot ?IsValid@CVillageTaskTorch@@UBE_NXZ + PDB name set (egor/fablewin).
