// CTCLightningNode::CTCLightningNode(CThing* owner) -- construct the lightning node component: CTCBase(owner), three intelligent pointers (PParent, PSourceOfFaction, PSourceOfHit), a null PDef, Timer 0 and an empty LightningBranches vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E6D17, length 90 bytes.
// Leaf: ??0CTCLightningNode
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (x3).
// Vtables: CTCLightningNode @ 0x0124337C, CIntelligentPointer<CThing> @ 0x01238C6C (kept in ebx for two members),
// CIntelligentPointer<CThing const> @ 0x01237F1C.
// Layout: PDB CTCLightningNode (0x3c; retail 0x38 with the 12-byte vector): PParent +0x0c, PSourceOfFaction +0x14,
// PSourceOfHit +0x1c (8-byte intelligent pointers: vptr + item), PDef +0x24, Timer +0x28, LightningBranches +0x2c.
// No engine header exists; local model. Shape: the derived vtable store sits between the first member's `lea` and its base
// constructor call; each CIntelligentPointer<T> constructor is inline (out-of-line base constructor, then its own vtable
// store); the five trailing zero stores are PDef's, Timer's and the vector's inline initialisers sharing `xor eax,eax`.
// Size-optimised TU (`#pragma optimize("s", on)`): the pass-through `push dword ptr [esp+10h]` and the vtable constant
// cached in ebx.
#pragma optimize("s", on)
class CThing;
class CLightningDef;
struct CLightningBranchPointer;

class CTCBase { public:
    CTCBase(CThing* owner);                        // 0x00686800
    virtual ~CTCBase();                            // 0x00686830
    CThing* m_pThing;                              // +0x04
    long    m_baseWord;                            // +0x08
};

struct CBaseIntelligentPointer {
    CBaseIntelligentPointer();                     // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    virtual void v00();
    void* m_pItem;                                 // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() { }
    virtual void v00();
};

struct CLightningDefPointer {                      // CDefPointer<CLightningDef const>
    const CLightningDef* m_pDef;
    CLightningDefPointer() : m_pDef(0) { }
};

struct CLightningBranchVector {                    // STLport vector<CCountedPointer<CLightningBranch>>
    CLightningBranchPointer* _M_start;
    CLightningBranchPointer* _M_finish;
    CLightningBranchPointer* _M_end_of_storage;
    CLightningBranchVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

class CTCLightningNode : public CTCBase { public:
    CIntelligentPointer<CThing>       PParent;            // +0x0c
    CIntelligentPointer<const CThing> PSourceOfFaction;   // +0x14
    CIntelligentPointer<CThing>       PSourceOfHit;       // +0x1c
    CLightningDefPointer              PDef;               // +0x24
    long                              Timer;              // +0x28
    CLightningBranchVector            LightningBranches;  // +0x2c
    CTCLightningNode(CThing* owner);
    virtual void v01();
};

CTCLightningNode::CTCLightningNode(CThing* owner)
    : CTCBase(owner), Timer(0)
{
}