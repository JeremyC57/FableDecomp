// Leaf: ?CVillageTaskTorch_IsValid@@YI_NPAVCVillageTaskTorch@@@Z
#include "engine/CVillageTaskTorch.h"  // retyped onto the PDB layout; byte parity re-verified
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
bool __fastcall CVillageTaskTorch_IsValid(CVillageTaskTorch* task){ return ((int)(((CBaseIntelligentPointer*)(reinterpret_cast<char*>(task)+8))->GetPItem())) != 0; }
// Stand-in (owner rule 2026-11-05): CVillageTaskTorch_IsValid for CVillageTaskTorch::IsValid; RTTI vtable slot ?IsValid@CVillageTaskTorch@@UBE_NXZ + PDB name set (egor/fablewin).
