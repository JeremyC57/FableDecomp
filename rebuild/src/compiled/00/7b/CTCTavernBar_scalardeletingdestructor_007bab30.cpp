// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#pragma optimize("t", on)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructSecondMember_007b9830(void* member);
void* __fastcall CTCTavernBar_CompositeScalarDeletingDestructor(void* bar, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(bar) + 24))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    DestructSecondMember_007b9830(static_cast<unsigned char*>(bar) + 12);
    ((CTCBase*)(bar))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(bar);
    return bar;
}