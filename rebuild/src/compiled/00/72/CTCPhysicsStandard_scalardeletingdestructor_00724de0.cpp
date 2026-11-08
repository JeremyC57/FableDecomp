// CTCPhysicsStandard::`scalar deleting destructor'(unsigned int) -- destructor body frees PPhysicsMeshInfo (+0x40); then the CTCPhysicsBase PhysicsDef counted pointer (+0x38) is released and CTCBase's destructor runs; free when bit 0 of the flags is set
// Leaf: ??_GCTCPhysicsStandard
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724DE0, length 78 bytes (+ int3 padding). CTCPhysicsStandard vtable
// 0x01265F84 slot 0.
// Callees: operator delete @ 0x00BFE9BC (the mesh info, then the object), the def's virtual slot 1 (`call [eax+4]`),
// CTCBase::~CTCBase @ 0x00686830 (stores the CTCBase vtable 0x0125BE0C; the manifest's "~CChunkedFileChunk" label is
// propagated).
// Label correction: the manifest's "CThingHolySite::`vector_deleting_destructor'" is propagated; the vtable names the class.
// Layout: local model. PDB CTCPhysicsBase { Position +0xC, OldPosition +0x18, OldPositionLastFrameSet +0x24, Velocity
// +0x28, Radius +0x34, CDefPointer<CPhysicsDef> PhysicsDef +0x38, flag bits +0x3C } and CTCPhysicsStandard
// PPhysicsMeshInfo +0x40 match the retail offsets; engine/CTCPhysicsStandard.h / CTCBase.h model the vptr as a data
// word, which cannot carry the virtual destructor, so the hierarchy is restated here.
// Shape: CTCPhysicsStandard declares the destructor (its vtable store comes first, scheduled after the member load);
// CTCPhysicsBase is an abstract `__declspec(novtable)` class with a `__forceinline` destructor (no CTCPhysicsBase vtable
// reset; a generated one was called out of line); the mesh info's delete is guarded explicitly (`delete p` of a
// trivially destructible type emits no null test); the def pointer's release and the destructors are `__forceinline`;
// a `new` factory pulls the vtable and the ??_G into the TU.
#include "fable_compiler.h"
class CThing;
class CThingPhysicsMeshInfo { public: long m_data; };

#include "engine/C3DVector.h"

class CPhysicsDef { public:
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +0x04
};

template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) {}
    FABLE_FORCEINLINE ~CDefPointer()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_p = 0;
        }
    }
};

class CTCBase { public:
    CTCBase();
    virtual ~CTCBase();                             // 0x00686830
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};

struct __declspec(novtable) CTCPhysicsBase : CTCBase {
    C3DVector                Position;              // +0x0c
    C3DVector                OldPosition;           // +0x18
    long                     OldPositionLastFrameSet;   // +0x24
    C3DVector                Velocity;              // +0x28
    float                    Radius;                // +0x34
    CDefPointer<CPhysicsDef> PhysicsDef;            // +0x38
    unsigned long            m_flags;               // +0x3c Moving / OldMoving bits
    FABLE_FORCEINLINE virtual ~CTCPhysicsBase() {}
};

struct CTCPhysicsStandard : CTCPhysicsBase {
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;        // +0x40
    FABLE_FORCEINLINE virtual ~CTCPhysicsStandard()
    {
        if (PPhysicsMeshInfo)
            delete PPhysicsMeshInfo;
    }
};

CTCPhysicsStandard* NewCTCPhysicsStandard()
{
    return new CTCPhysicsStandard;
}