// CModeWithTwoFilters_0073a5d0::~CModeWithTwoFilters_0073a5d0() -- destructor of a CModeBase-derived creature mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073A5D0, length 49 bytes.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 on +0x28,
// CCharString::~CCharString @ 0x0099EAE0 on +0x20 followed by the CAnimFilterBase vtable store
// 0x0122FFCC at +0x1c (a named anim-filter member's inline destructor), then the inlined
// CTCCreatureModeManager::CModeBase destructor: its vtable 0x012673CC stored at +0, its own
// filter member at +0x8 destroyed the same way (string +0xc, base vtable at +0x8). No store of
// the derived vtable at entry: a `__declspec(novtable)` most-derived class. The manifest label
// (BalverineCornered ctor) is propagated garbage; names are descriptive.
struct CBaseIntelligentPointer {         // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                          // +4
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
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

struct CModeBase {                          // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual void v00();
    long m_type;                            // +0x4
    CAnimFilterOfGroup m_idleFilter;        // +0x8 (12 bytes)
    long m_a;                               // +0x14
    long m_b;                               // +0x18
    ~CModeBase() { }
};

struct __declspec(novtable) CModeWithTwoFilters_0073a5d0 : CModeBase {
    CAnimFilterOfGroup  m_actionFilter;     // +0x1c
    CIntelligentPointer m_pTarget;          // +0x28
    ~CModeWithTwoFilters_0073a5d0();
};

CModeWithTwoFilters_0073a5d0::~CModeWithTwoFilters_0073a5d0()
{
}