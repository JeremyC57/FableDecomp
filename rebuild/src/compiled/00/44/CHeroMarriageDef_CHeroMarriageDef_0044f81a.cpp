// CHeroMarriageDef::~CHeroMarriageDef() -- destroy the five CCharString members (+0xc4, +0xb8, +0xa8, +0xa4, +0x7c; 0x0099EAE0) then the inlined CDefPointeeBase destructor: its vtable (0x01230BA0) and the base destructor (0x009FC550, a tail jump)
// Leaf: ??1CHeroMarriageDef@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044F81A, length 69 bytes.
// Shape: `this` in esi, the strings last-to-first, `mov dword ptr [esi],vtable ; mov ecx,esi ; pop esi ; jmp`.
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

struct __declspec(novtable) CHeroMarriageDef : CDefPointeeBase {
    unsigned char _pad04[0x78];
    CCharString   m_string7c;                      // +0x7c
    unsigned char _pad80[0x24];
    CCharString   m_stringa4;                      // +0xa4
    CCharString   m_stringa8;                      // +0xa8
    unsigned char _padac[0xc];
    CCharString   m_stringb8;                      // +0xb8
    unsigned char _padbc[8];
    CCharString   m_stringc4;                      // +0xc4
    virtual void v00();
    ~CHeroMarriageDef();
};

CHeroMarriageDef::~CHeroMarriageDef()
{
}