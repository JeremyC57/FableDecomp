// CDefPointeeBase_Derived_004e26f1::~CDefPointeeBase_Derived_004e26f1() -- destroy the two 16-byte members at +0x74 and +0x64 (0x004DD3EE), the two 12-byte members at +0x4c and +0x40 (0x004DF907) and the CCharString at +0x3c, then the inlined CDefPointeeBase destructor (vtable 0x01230BA0, base 0x009FC550 as a tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E26F1, length 57 bytes.
// The chunk label "CHeroMarriageDef::~CHeroMarriageDef" is propagated (a different layout); the member destructor labels ("CActiveFile::OnReadFinished") are propagated too. Shape: members last-to-first, `this` in esi.
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
class CResource { public:
    virtual void v00();
    virtual ~CResource();                             // 0x009FC550 (labelled "~CMemoryBlock")
};
class CDefPointeeBase : public CResource { public:          // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }                         // inlined into every derived destructor: the vtable store then the base tail jump
};
struct CMember16_004dd3ee {
    unsigned char _body[0x10];
    ~CMember16_004dd3ee();                         // 0x004DD3EE
};
struct CMember12_004df907 {
    unsigned char _body[0xc];
    ~CMember12_004df907();                         // 0x004DF907
};

struct __declspec(novtable) CDefPointeeBase_Derived_004e26f1 : CDefPointeeBase {
    unsigned char       _pad04[0x38];
    CCharString         m_string3c;                // +0x3c
    CMember12_004df907  m_member40;                // +0x40
    CMember12_004df907  m_member4c;                // +0x4c
    unsigned char       _pad58[0xc];
    CMember16_004dd3ee  m_member64;                // +0x64
    CMember16_004dd3ee  m_member74;                // +0x74
    virtual void v00();
    ~CDefPointeeBase_Derived_004e26f1();
};

CDefPointeeBase_Derived_004e26f1::~CDefPointeeBase_Derived_004e26f1()
{
}