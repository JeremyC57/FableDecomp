// CModeStandNamed_008cc280::~CModeStandNamed_008cc280() -- three pointer members, a named anim filter, then the stand base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CC280, length 53 bytes.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 on +0x84, +0x7c,
// +0x74; CCharString::~CCharString @ 0x0099EAE0 on +0x6c followed by the CAnimFilterBase
// vtable store 0x0122FFCC at +0x68 (the anim-filter member's inline destructor: its own
// vtable store is dead, the string is destroyed, then the empty inline base destructor stores
// the base vtable); then ~CModeStandBase @ 0x0073D860 tail-called. No vtable store at entry:
// a `__declspec(novtable)` most-derived class. The manifest label (CarryCrateAndFollow dtor)
// is propagated garbage; names are descriptive.
struct CBaseIntelligentPointer {         // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                          // +4
    ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {
};

class CCharString { public:
    char* m_pData;
    ~CCharString();                         // 0x0099EAE0
};

class CAnimFilterBase { public:                    // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }
};

class CAnimFilterOfGroup : public CAnimFilterBase { public:   // 12 bytes
    CCharString m_group;                    // +4
    long        m_groupIndex;               // +8
};

struct CModeStandBase {
    virtual void v00();
    ~CModeStandBase();                      // 0x0073D860
    unsigned char _pad[0x64];               // +0x4..+0x67
};

struct __declspec(novtable) CModeStandNamed_008cc280 : CModeStandBase {
    CAnimFilterOfGroup  m_filter;           // +0x68
    CIntelligentPointer m_pThing0;          // +0x74
    CIntelligentPointer m_pThing1;          // +0x7c
    CIntelligentPointer m_pThing2;          // +0x84
    ~CModeStandNamed_008cc280();
};

CModeStandNamed_008cc280::~CModeStandNamed_008cc280()
{
}