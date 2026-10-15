// CTCLightningOrb::CTCLightningOrb(CThing* owner) -- construct the lightning orb component: CTCBase(owner), the PParent and PTarget intelligent pointers, a null PDef, Timer and LifeTime -1, NumFramesForSplineTraversal 1 and a null PSpline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB4D7, length 83 bytes.
// Leaf: ??0CTCLightningOrb
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (x2).
// Vtables: CTCLightningOrb @ 0x01240A2C, CIntelligentPointer<CThing> @ 0x01238C6C (cached in ebx for both members).
// Layout: PDB CTCLightningOrb (0x34): PParent +0x0c, PTarget +0x14 (8-byte intelligent pointers), PDef +0x1c, Timer +0x20,
// LifeTime +0x24, NumFramesForSplineTraversal +0x28, PSpline +0x2c (CCountedPointer<NBBBMaths::CLinearSpline>: object + block).
// No engine header exists; local model. Shape: as the CTCLightningNode constructor (0x004E6D17): a size-optimised TU
// (pass-through argument push, `or dword ptr [..],0FFFFFFFFh` for -1, the vtable constant in ebx), inline CIntelligentPointer
// constructors, and initialisers in declaration order sharing the `xor eax,eax` zero.
#pragma optimize("s", on)
struct CThing;
struct CLightningOrbDef;
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

struct CLightningOrbDefPointer {                   // CDefPointer<CLightningOrbDef const>
    const CLightningOrbDef* m_pDef;
    CLightningOrbDefPointer() : m_pDef(0) { }
};

struct CSplinePointer {                            // CCountedPointer<NBBBMaths::CLinearSpline>
    CLinearSpline* m_pObject;
    void*          m_pBlock;
    CSplinePointer() : m_pObject(0), m_pBlock(0) { }
};

struct CTCLightningOrb : CTCBase {
    CIntelligentPointer<CThing> PParent;                       // +0x0c
    CIntelligentPointer<CThing> PTarget;                       // +0x14
    CLightningOrbDefPointer     PDef;                          // +0x1c
    long                        Timer;                         // +0x20
    long                        LifeTime;                      // +0x24
    long                        NumFramesForSplineTraversal;   // +0x28
    CSplinePointer              PSpline;                       // +0x2c
    CTCLightningOrb(CThing* owner);
    virtual void v01();
};

CTCLightningOrb::CTCLightningOrb(CThing* owner)
    : CTCBase(owner), Timer(-1), LifeTime(-1), NumFramesForSplineTraversal(1)
{
}
