// CAIStateGroup_CarryCrateAndFollow::~CAIStateGroup_CarryCrateAndFollow() -- four pointer members, then the base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009017F2, length 43 bytes.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 on +0x2c, +0x24,
// +0x1c, +0x14 (reverse declaration order), then CAIStateGroupBase::~CAIStateGroupBase
// @ 0x008FD288 tail-called. No vtable store at entry: the class is `__declspec(novtable)`
// (its own store is dropped). The PDB header puts PCrate/PPreviouslyCarriedItem/PThingToFollow
// at +0x14/+0x1c/+0x24 and a bool at +0x2c; retail destroys a fourth pointer at +0x2c, so the
// layout is modelled locally (Ego_r differs from retail here). The base destructor is modelled
// non-virtual so the plain `??1` body (not the deleting `??_G`) is the TU's largest symbol.
struct CBaseIntelligentPointer {         // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                          // +4
    ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {
};

class CAIStateGroupBase { public:
    virtual void v00();
    virtual ~CAIStateGroupBase();                   // 0x008FD288
    unsigned char _pad[0x10];               // +0x4..+0x13
};

struct __declspec(novtable) CAIStateGroup_CarryCrateAndFollow : CAIStateGroupBase {
    CIntelligentPointer PCrate;                 // +0x14
    CIntelligentPointer PPreviouslyCarriedItem; // +0x1c
    CIntelligentPointer PThingToFollow;         // +0x24
    CIntelligentPointer PCarrier;               // +0x2c (retail-only fourth pointer)
    ~CAIStateGroup_CarryCrateAndFollow();
};

CAIStateGroup_CarryCrateAndFollow::~CAIStateGroup_CarryCrateAndFollow()
{
}