// CTCForcePushPower::`scalar deleting destructor'(unsigned int) -- destroy the intelligent pointer member, release the def pointer PDef (+0xC), run CTCBase's destructor, free when bit 0 of the flags is set
// Leaf: ??_GCTCForcePushPower
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A62D0, length 70 bytes (+ int3 padding). CTCForcePushPower vtable 0x0126C9BC slot 0.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10, the def's virtual slot 1 (`call [eax+4]`),
// CTCBase::~CTCBase @ 0x00686830 (stores the CTCBase vtable 0x0125BE0C), operator delete @ 0x00BFE9BC.
// Label correction: the manifest's "CThingHolySite::`vector_deleting_destructor'" is propagated; the vtable names the class.
// Layout: PDB CTCForcePushPower (0x28 bytes) { PDef +0xC, SpellLevel +0x10, PItemRemoved +0x14, Casted +0x1C,
// CurTimeDelayBetweenCastsFrames +0x20, IgnoreFriendlies +0x24 }.
// engine/CTCBase.h models the vptr as a data word, which cannot carry the virtual destructor, so CTCBase is restated.
// Shape: CTCForcePushPower declares a `__forceinline` virtual destructor (its vtable store follows the first `lea`); members die in
// reverse order: the CIntelligentPointer (compiler-generated destructor: no vtable reset, the base destructor out of
// line), then the def pointer's `__forceinline` release (`dec dword ptr [ecx+4] ; jne`, the clear inside the null
// test), then the out-of-line CTCBase destructor; a `new` factory pulls the vtable and the ??_G into the TU.
class CThing;

struct CRefCountedDef {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +0x04
};

template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) {}
    __forceinline ~CDefPointer()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_p = 0;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* PItem;                                    // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
};

class CTCBase { public:
    CTCBase();
    virtual ~CTCBase();                             // 0x00686830
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};

class CTCForcePushPower : public CTCBase { public:
    CDefPointer<CRefCountedDef> PDef;               // +0x0c CDefPointer<CSpecialAbilitiesForcePushDef const>
    long                        SpellLevel;         // +0x10
    CIntelligentPointer         PItemRemoved;       // +0x14 CIntelligentPointer<CThing>
    bool                        Casted;             // +0x1c
    long                        CurTimeDelayBetweenCastsFrames;   // +0x20
    bool                        IgnoreFriendlies;   // +0x24
    __forceinline virtual ~CTCForcePushPower() {}
};

CTCForcePushPower* NewCTCForcePushPower()
{
    return new CTCForcePushPower;
}