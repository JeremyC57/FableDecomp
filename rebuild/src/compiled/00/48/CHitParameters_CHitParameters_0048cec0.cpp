// CHitParameters::~CHitParameters() -- destroy the intelligent pointer at +0x38 (0x00A01C10), release the counted pointer at +0x30, destroy the intelligent pointer at +0x28, then the base destructor (0x00A0D3D0, a tail jump)
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048CEC0, length 51 bytes.
// The base label "~CAInputProcess" is propagated. Shape: no vtable store (the class is not polymorphic here), members last-to-first.
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CHitParametersBase_00a0d3d0 {               // 0x28 bytes
    unsigned char _body[0x28];
    ~CHitParametersBase_00a0d3d0();                // 0x00A0D3D0
};
class CHitParameters : public CHitParametersBase_00a0d3d0 { public:
    CBaseIntelligentPointer m_pointer28;           // +0x28
    CIVCountedPointer       m_counted30;           // +0x30
    unsigned char           _pad34[4];
    CBaseIntelligentPointer m_pointer38;           // +0x38
    ~CHitParameters();
};

CHitParameters::~CHitParameters()
{
}