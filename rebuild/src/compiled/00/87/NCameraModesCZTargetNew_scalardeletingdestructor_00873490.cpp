// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#pragma optimize("t", on)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
namespace NCameraModes {
struct CBase {                                         // RTTI base of every camera mode
    virtual ~CBase();                                  // 0x0056E0E0 (Ego_r ??1CBase@NCameraModes@@UAE@XZ)
};
}
void* __fastcall CZTargetNew_CompositeScalarDeletingDestructor(void* targetNew, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(targetNew) + 164))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((NCameraModes::CBase*)targetNew)->NCameraModes::CBase::~CBase();   // the base destructor (namepass CAM2)
    if (flags & 1)
        operator delete(targetNew);
    return targetNew;
}