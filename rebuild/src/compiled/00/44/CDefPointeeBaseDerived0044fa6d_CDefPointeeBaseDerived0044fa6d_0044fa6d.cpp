// CDefPointeeBase_Derived_0044fa6d::~CDefPointeeBase_Derived_0044fa6d() -- destroy the CCharString members at +0x50, +0x4c, +0x28, then the inlined CDefPointeeBase destructor stores its vtable and tail-calls the CMemoryBlock base destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044FA6D, length 41 bytes.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (x3), CBase_009fc550::~CBase_009fc550 @ 0x009FC550
// (manifest: ~CMemoryBlock) tail-called. Vtable 0x01230BA0 = CDefPointeeBase (rtti-vtables.tsv).
// The manifest label "~CBossDef" is propagated (the PDB CBossDef carries vectors, not strings, at
// +0x28); the derived class cannot be named from the body. Same shape as the landed 0x004D9861:
// the derived class is `__declspec(novtable)` (abstract) so only the inlined CDefPointeeBase
// destructor's vtable store appears; members are destroyed in reverse declaration order.
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                             // 0x0099EAE0
};

class CResource { public:
    virtual void v00();
    virtual ~CResource();                          // 0x009FC550
};

class CDefPointeeBase : public CResource { public:       // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }                      // inline: inlined into every derived destructor
};

struct __declspec(novtable) CDefPointeeBase_Derived_0044fa6d : CDefPointeeBase {
    unsigned char _pad0[0x24];                  // +0x04..+0x27
    CCharString   m_string0;                    // +0x28
    unsigned char _pad1[0x20];                  // +0x2c..+0x4b
    CCharString   m_string1;                    // +0x4c
    CCharString   m_string2;                    // +0x50
    virtual void v00();
    ~CDefPointeeBase_Derived_0044fa6d();
};

CDefPointeeBase_Derived_0044fa6d::~CDefPointeeBase_Derived_0044fa6d()
{
}