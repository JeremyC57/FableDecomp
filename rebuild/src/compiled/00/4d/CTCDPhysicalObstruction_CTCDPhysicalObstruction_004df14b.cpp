// CTCDPhysicalObstruction::CTCDPhysicalObstruction(CThing* parent) -- CTCDriverBase(parent), own vtable, empty ignore list, three null primitive handles
// Retry 10: RELOCATION_MATCH -- the vector modelled as STLport _Vector_base (m_pEnd an _STLP_alloc_proxy with its own ctor); three plain pointers put `mov ecx,edi` one store too early.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF14B, length 91 bytes.
// Callees: CTCDriverBase::CTCDriverBase(CThing*) @ 0x00771200; CBaseClass::CBaseClass @ 0x0099A2D0 (x3, the
// base of each CEnginePrimitiveHandle, vtable 0x01231710).
// Vtable 0x01241C0C = CTCDPhysicalObstruction, 0x0122F598 = CEnginePrimitiveHandle (rtti-vtables.tsv).
// PDB CTCDPhysicalObstruction: StartPos +0x0c, EndPos +0x18 (C3DVector, trivial constructors: no stores),
// LifeTimer +0x24 and Radius +0x28 (left uninitialised), ThingsToIgnoreObstruction (vector) +0x2c,
// line1/line2/line3 (CEnginePrimitiveHandle) +0x3c/+0x44/+0x4c in the PDB; the retail vector is 12 bytes so
// the handles sit at +0x38/+0x40/+0x48 (the `lea edi` offsets).
// Shape: /Os pass-through base argument; the derived vtable store first, then the vector's inline
// constructor (three zero stores through ebp), then each handle's inline default constructor: the
// CBaseClass call, the handle vtable (kept in ebx across the three) and the null CIVCountedPointer.
#pragma optimize("s", on)
class CThing;
struct CThingConst;
class CEnginePersistentPrimitiveBase;

struct CTCDriverBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCDriverBase_Calls(CThing* parent);                    // 0x00771200
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};

template <class P> struct CAllocProxy {      // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    P p;
    CAllocProxy(P v) : p(v) { }
    operator P() const { return p; }
    CAllocProxy& operator=(P v) { p = v; return *this; }
};
template<class T> struct CRetailVector {                    // retail Dinkumware vector: 3 pointers
    T* m_pFirst;
    T* m_pLast;
    CAllocProxy<T*> m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CIntelligentPointerThing {                           // CIntelligentPointer<CThing>, 8 bytes
    void* __vftable;
    void* PData;
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

struct CIVCountedPointer {                                  // CIVCountedPointer<CEnginePersistentPrimitiveBase>
    CEnginePersistentPrimitiveBase* Object;
    __forceinline CIVCountedPointer() : Object(0) { }
};

class CEnginePrimitiveHandle : public CBaseClass { public:                // vtable 0x0122F598
    CIVCountedPointer Data;                                 // +4
    __forceinline CEnginePrimitiveHandle() { }             // inline under /Os only when forced
};

class CTCDPhysicalObstruction : public CTCDriverBase_Calls { public:
    C3DVector                               StartPos;                   // +0x0c
    C3DVector                               EndPos;                     // +0x18
    long                                    LifeTimer;                  // +0x24
    float                                   Radius;                     // +0x28
    CRetailVector<CIntelligentPointerThing> ThingsToIgnoreObstruction;  // +0x2c
    CEnginePrimitiveHandle                  line1;                      // +0x38
    CEnginePrimitiveHandle                  line2;                      // +0x40
    CEnginePrimitiveHandle                  line3;                      // +0x48
    virtual void v00();                                     // own vtable 0x01241C0C
    CTCDPhysicalObstruction(CThing* parent);
};

CTCDPhysicalObstruction::CTCDPhysicalObstruction(CThing* parent)
    : CTCDriverBase_Calls(parent)
{
}