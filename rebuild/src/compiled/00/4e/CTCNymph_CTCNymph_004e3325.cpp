// CTCNymph::CTCNymph(CThing* thing) -- CTCBase(thing), own vtable, null def, an empty flight spline (tension 0.5), null effect/old-effect/target pointers
// Leaf: ??0CTCNymph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3325, length 97 bytes (`ret 4`; size TU, odd start). Vtable 0x01242AD4 (CTCNymph).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (catalog "CEngineSubPrimitive2DClockSprite" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (x3: CIntelligentPointer<CThing> vtable 0x01238C6C twice,
// CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8).
// Layout (retail, local model; PDB CTCNymph is 0x74, retail 0x70 because the retail CArray inside CCardinalSpline is 0x0C):
// PDef +0x0C, PointToFlyAround +0x10, EffectHeightOffset +0x1C, InitialPos +0x20, FlightSpline +0x2C (ControlPoints +0x2C,
// Tension +0x38, EndPointsConnected +0x3C), FlyingAround +0x40, PEffect +0x41 and POldEffect +0x49 (intelligent pointers at odd
// offsets: the pointer class is byte-aligned, so it is declared under pack(1)), LastTargetTick +0x54, CanAttack.. +0x58, FadeCounter
// +0x60, TotalFadeTime +0x64, PTarget +0x68.
// Shape: `#pragma optimize("s", on)` (`push [esp+10h]`, the CThing vtable held in ebx); the spline's inline ctor zeroes its
// array (ControlPoints is an STLport vector<C3DVector>, as in the landed CTCCreatureFlyInCircles ctor 0x007DDF90) and sets Tension 0.5f
// and EndPointsConnected false; the plain scalars are left uninitialised. The end-of-storage word is modelled
// like STLport's _STLP_alloc_proxy (an allocator base + the word): that places `lea edi,[esi+41h] ; mov ecx,edi` after the third
// array zero as retail (plain three-word init puts it right after the PDef zero).
#pragma optimize("s", on)
class CThing;
class CThingCreatureBase;
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                      // 0x00A01B10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature() { }
};
#pragma pack(pop)
class C3DVector { public: float X, Y, Z; };
template <class T> struct CAllocator { CAllocator() { } };
template <class T> struct CAllocProxy : CAllocator<T> {
    long m_capacity;
    CAllocProxy(const CAllocator<T>& a, long c) : CAllocator<T>(a), m_capacity(c) { }
};
template <class T> class CArray { public:                  // retail CArray is 0x0c: data, size, capacity
    T*   m_pData;
    long m_size;
    CAllocProxy<T> m_capacity;
    CArray() : m_pData(0), m_size(0), m_capacity(CAllocator<T>(), 0) { }
};
class CCardinalSpline { public:                            // retail 0x14
    CArray<C3DVector> ControlPoints;                // +0
    float             Tension;                      // +0xc
    bool              EndPointsConnected;           // +0x10
    CCardinalSpline() : Tension(0.5f), EndPointsConnected(false) { }
};
class CNymphDef;
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    CTCBase(CThing* thing);                         // 0x00686800
};
class CTCNymph : public CTCBase { public:                         // vtable 0x01242AD4
    CDefPointer<const CNymphDef> PDef;              // +0x0c
    C3DVector PointToFlyAround;                     // +0x10
    float EffectHeightOffset;                       // +0x1c
    C3DVector InitialPos;                           // +0x20
    CCardinalSpline FlightSpline;                   // +0x2c
    bool FlyingAround;                              // +0x40
    CIntelligentPointerThing PEffect;               // +0x41
    CIntelligentPointerThing POldEffect;            // +0x49
    unsigned long LastTargetTick;                   // +0x54
    bool CanAttack;                                 // +0x58
    bool ReadyToAttack;                             // +0x59
    bool ReadyToSummon;                             // +0x5a
    bool HasBeenInLimbo;                            // +0x5b
    bool HasSpline;                                 // +0x5c
    bool BeenHit;                                   // +0x5d
    long FadeCounter;                               // +0x60
    long TotalFadeTime;                             // +0x64
    CIntelligentPointerCreature PTarget;            // +0x68
    virtual void v00();
    CTCNymph(CThing* thing);
};

CTCNymph::CTCNymph(CThing* thing)
    : CTCBase(thing)
{
}