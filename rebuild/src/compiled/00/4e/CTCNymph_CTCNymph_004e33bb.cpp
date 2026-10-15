// CTCNymph::~CTCNymph() -- destroy the nymph component: the PTarget, POldEffect and PEffect intelligent pointers, the FlightSpline's point storage (freed), the PDef def pointer (released and cleared), then ~CTCBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E33BB, length 70 bytes (the plain `??1`).
// Leaf: ??1CTCNymph
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (x3), free @ 0x00BFEA14, the def's virtual slot 1
// (Destroy, when its count drops to zero), CTCBase::~CTCBase @ 0x00686830 (tail jump).
// The chunk label "CTCLightningOrb::~CTCLightningOrb" is propagated: the only caller is the scalar deleting destructor
// 0x004E339F, slot 0 of CTCNymph's vtable 0x01242AD4 (RTTI). Layout: PDB CTCNymph (0x74; retail 0x70: the CCardinalSpline
// at +0x2c holds a 12-byte retail vector, so everything after it sits 4 bytes low): PDef +0x0c, FlightSpline +0x2c,
// FlyingAround +0x40, PEffect +0x41, POldEffect +0x49 (the PDB already packs them unaligned behind the bool), PTarget +0x68.
// No engine header exists; local packed model.
// Shape: no vptr reset (the class declares no destructor), members destroyed in reverse order; the spline's vector frees its
// storage with a plain null test (`push eax ; call free ; pop ecx`) and the def pointer releases through the def object and
// clears with `and dword ptr [esi+0Ch],0` -- a size-optimised TU, so the inline destructors are `__forceinline`.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);       // 0x00BFEA14

struct CThing;
struct CThingCreatureBase;

struct CTCBase {
    CTCBase(CThing* owner);                        // 0x00686800
    virtual ~CTCBase();                            // 0x00686830
    CThing* m_pThing;                              // +0x04
    long    m_baseWord;                            // +0x08
};

struct CBaseIntelligentPointer {
    CBaseIntelligentPointer();                     // 0x00A01B10
    ~CBaseIntelligentPointer();                    // 0x00A01C10
    virtual void v00();
    void* m_pItem;                                 // +0x04
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    virtual void v00();
};

struct CDefBase {                                  // counted def: vptr, count at +4
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_count;                                  // +0x04
    __forceinline void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};

struct CNymphDefPointer {                          // CDefPointer<CNymphDef const>
    CDefBase* m_pDef;
    CNymphDefPointer() : m_pDef(0) { }
    __forceinline ~CNymphDefPointer()
    {
        if (m_pDef) {
            m_pDef->Release();
            m_pDef = 0;
        }
    }
};

struct C3DVector { float x, y, z; };

struct CSplinePointVector {                        // STLport vector<C3DVector> (malloc allocator)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CSplinePointVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~CSplinePointVector()
    {
        if (_M_start)
            free(_M_start);
    }
};

struct CCardinalSpline {                           // 0x14 bytes in retail
    CSplinePointVector m_points;                   // +0x00
    float              m_tension;                  // +0x0c
    long               m_segmentCount;             // +0x10
    CCardinalSpline() : m_tension(0), m_segmentCount(0) { }
};

#pragma pack(push, 1)
struct CTCNymph : CTCBase {
    CNymphDefPointer                        PDef;               // +0x0c
    C3DVector                               PointToFlyAround;   // +0x10
    float                                   EffectHeightOffset; // +0x1c
    C3DVector                               InitialPos;         // +0x20
    CCardinalSpline                         FlightSpline;       // +0x2c
    bool                                    FlyingAround;       // +0x40
    CIntelligentPointer<CThing>             PEffect;            // +0x41
    CIntelligentPointer<CThing>             POldEffect;         // +0x49
    unsigned char                           _pad_0x51[3];
    unsigned long                           LastTargetTick;     // +0x54
    bool                                    CanAttack;          // +0x58
    bool                                    ReadyToAttack;      // +0x59
    bool                                    ReadyToSummon;      // +0x5a
    bool                                    HasBeenInLimbo;     // +0x5b
    bool                                    HasSpline;          // +0x5c
    bool                                    BeenHit;            // +0x5d
    unsigned char                           _pad_0x5e[2];
    long                                    FadeCounter;        // +0x60
    long                                    TotalFadeTime;      // +0x64
    CIntelligentPointer<CThingCreatureBase> PTarget;            // +0x68
    CTCNymph(CThing* owner) : CTCBase(owner) { }
    virtual void v01();
};
#pragma pack(pop)

CTCNymph* CreateNymph(CThing* owner)
{
    return new CTCNymph(owner);
}