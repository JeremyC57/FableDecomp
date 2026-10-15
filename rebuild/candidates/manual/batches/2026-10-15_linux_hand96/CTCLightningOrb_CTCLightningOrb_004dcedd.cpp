// CTCLightningOrb::~CTCLightningOrb() -- destroy the lightning orb component: the PSpline counted pointer, the PDef def pointer (released and cleared), the PTarget and PParent intelligent pointers, then ~CTCBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCEDD, length 56 bytes (the plain `??1`).
// Leaf: ??1CTCLightningOrb
// Callees: CCountedPointer<NBBBMaths::CLinearSpline>::~CCountedPointer @ 0x004D76B8 (out of line, `this` in ecx; manifest
// label "_Dest_val<...>" propagated), the def's virtual slot 1 (Destroy, when its count drops to zero),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (x2), CTCBase::~CTCBase @ 0x00686830 (tail jump).
// Layout: as the constructor 0x004DB4D7 (PParent +0x0c, PTarget +0x14, PDef +0x1c, Timer +0x20, LifeTime +0x24,
// NumFramesForSplineTraversal +0x28, PSpline +0x2c). Local model.
// Shape: the CTCLightningNode destructor's (0x004E769F): no vptr reset (the class declares no destructor), members destroyed
// in reverse order, the def pointer's `__forceinline` destructor releasing through the def object and clearing with
// `and dword ptr [esi+1Ch],0` (size-optimised TU); the `??1` is emitted through a `new CTCLightningOrb` factory.
#pragma optimize("s", on)
struct CThing;
struct CLinearSpline;

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

struct CLightningOrbDefPointer {                      // CDefPointer<CLightningOrbDef const>
    CDefBase* m_pDef;
    CLightningOrbDefPointer() : m_pDef(0) { }
    __forceinline ~CLightningOrbDefPointer()
    {
        if (m_pDef) {
            m_pDef->Release();
            m_pDef = 0;
        }
    }
};

struct CSplinePointer {                            // CCountedPointer<NBBBMaths::CLinearSpline>
    CLinearSpline* m_pObject;
    void*          m_pBlock;
    CSplinePointer() : m_pObject(0), m_pBlock(0) { }
    ~CSplinePointer();                             // 0x004D76B8
};

struct CTCLightningOrb : CTCBase {
    CIntelligentPointer<CThing> PParent;                       // +0x0c
    CIntelligentPointer<CThing> PTarget;                       // +0x14
    CLightningOrbDefPointer     PDef;                          // +0x1c
    long                        Timer;                         // +0x20
    long                        LifeTime;                      // +0x24
    long                        NumFramesForSplineTraversal;   // +0x28
    CSplinePointer              PSpline;                       // +0x2c
    CTCLightningOrb(CThing* owner) : CTCBase(owner), Timer(-1), LifeTime(-1), NumFramesForSplineTraversal(1) { }
    virtual void v01();
};

CTCLightningOrb* CreateLightningOrb(CThing* owner)
{
    return new CTCLightningOrb(owner);
}
