// CTCTargetedCountedComponent_006ca760::~CTCTargetedCountedComponent_006ca760() -- release the two intelligent pointers (+0x30, +0x1d) and the counted pointer (+0x18), then ~CTCBase
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA760, length 51 bytes.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (twice: +0x30 then the unaligned one at +0x1d), the counted object's virtual slot 1
// (`call [eax+4]`, Delete) when the release at +0x18 drops the count to zero, CTCBase::~CTCBase @ 0x00686830 (tail jump).
// The chunk label (~CCreatureAction_PerformExpressionExtended) is propagated; the base is CTCBase (0xc bytes) and no vtable of this class is stored before the
// members go, so this is an abstract (novtable) thing-component class, named by what it owns. The layout is byte-packed (a pointer object at +0x1d), as the
// PDB-derived engine headers declare for this hierarchy. Members are destroyed in reverse order.
struct CCountedObject {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1
    long m_refCount;                            // +4
};
struct CCountedPointer {
    CCountedObject* m_pObject;
    ~CCountedPointer()
    {
        CCountedObject* object = m_pObject;     // named: retail never re-reads the member
        if (object != 0) {
            if (--object->m_refCount == 0)
                object->Delete();
            m_pObject = 0;
        }
    }
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CTCBase { public:
    virtual void v00();
    unsigned char _pad[8];                      // +4..+0xb
    virtual ~CTCBase();                                 // 0x00686830
};

#pragma pack(push, 1)
struct __declspec(novtable) CTCTargetedCountedComponent_006ca760 : CTCBase {
    unsigned char _pad0c[0xc];                  // +0xc..+0x17
    CCountedPointer m_pAction;                  // +0x18
    unsigned char _pad1c;                       // +0x1c
    CBaseIntelligentPointer m_pTarget;          // +0x1d (unaligned)
    unsigned char _pad25[0xb];                  // +0x25..+0x2f
    CBaseIntelligentPointer m_pSource;          // +0x30
    virtual void v01();
    ~CTCTargetedCountedComponent_006ca760();
};
#pragma pack(pop)

CTCTargetedCountedComponent_006ca760::~CTCTargetedCountedComponent_006ca760()
{
}