// CTCLightningNode::~CTCLightningNode() -- destroy the lightning node component: the LightningBranches vector, the PDef def pointer (released and cleared), the three intelligent pointers, then ~CTCBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E769F, length 64 bytes (the plain `??1`).
// Leaf: ??1CTCLightningNode
// Callees: the vector<CCountedPointer<CLightningBranch>> destructor @ 0x004E40A0 (out of line; manifest label
// "CActiveFile::OnReadFinished" propagated), the def's virtual slot 1 (Destroy, when its count drops to zero),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (x3), CTCBase::~CTCBase @ 0x00686830 (tail jump).
// Layout: as the constructor 0x004E6D17 (PParent +0x0c, PSourceOfFaction +0x14, PSourceOfHit +0x1c, PDef +0x24,
// LightningBranches +0x2c). Local model.
// Shape: no vtable store at entry -- the class declares no destructor (VC7.1 drops the vptr reset in a compiler-generated
// destructor); the members die in reverse order. The def pointer's inline destructor is `if (m_pDef) { m_pDef->Release();
// m_pDef = 0; }` with the count decrement on the def object (`dec [ecx+4] ; jne ; call [eax+4]`) and the clear surviving as
// `and dword ptr [esi+24h],0`; the intelligent-pointer destructors add nothing to the out-of-line base destructor. The
// `??1` is emitted through a `new CTCLightningNode` factory (the `??_G` calls it out of line). Size-optimised TU (the
// `and` zero store); the def pointer's destructor and Release are `__forceinline` (the size TU would call them).
#pragma optimize("s", on)
struct CThing;
struct CLightningBranchPointer;

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

struct CLightningDefPointer {                      // CDefPointer<CLightningDef const>
    CDefBase* m_pDef;
    CLightningDefPointer() : m_pDef(0) { }
    __forceinline ~CLightningDefPointer()
    {
        if (m_pDef) {
            m_pDef->Release();
            m_pDef = 0;
        }
    }
};

struct CLightningBranchVector {                    // STLport vector<CCountedPointer<CLightningBranch>>
    CLightningBranchPointer* _M_start;
    CLightningBranchPointer* _M_finish;
    CLightningBranchPointer* _M_end_of_storage;
    CLightningBranchVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLightningBranchVector();                     // 0x004E40A0
};

struct CTCLightningNode : CTCBase {
    CIntelligentPointer<CThing>       PParent;            // +0x0c
    CIntelligentPointer<const CThing> PSourceOfFaction;   // +0x14
    CIntelligentPointer<CThing>       PSourceOfHit;       // +0x1c
    CLightningDefPointer              PDef;               // +0x24
    long                              Timer;              // +0x28
    CLightningBranchVector            LightningBranches;  // +0x2c
    CTCLightningNode(CThing* owner) : CTCBase(owner), Timer(0) { }
    virtual void v01();
};

CTCLightningNode* CreateLightningNode(CThing* owner)
{
    return new CTCLightningNode(owner);
}