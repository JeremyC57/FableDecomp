// CDefPointeeBase_Derived_007acac0::~CDefPointeeBase_Derived_007acac0() -- destroy the four CCharString members (+0x90, +0x8c, +0x6c, +0x64; 0x0099EAE0) then the inlined CDefPointeeBase destructor (vtable 0x01230BA0, base 0x009FC550 as a tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ACAC0, length 55 bytes.
// The chunk label "CHeroMorphDef::~CHeroMorphDef" is propagated (CHeroMorphDef has no strings at these offsets); named by address.
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct CBase_009fc550 {
    virtual void v00();
    ~CBase_009fc550();                             // 0x009FC550 (labelled "~CMemoryBlock")
};
struct CDefPointeeBase : CBase_009fc550 {          // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }                         // inlined into every derived destructor: the vtable store then the base tail jump
};

struct __declspec(novtable) CDefPointeeBase_Derived_007acac0 : CDefPointeeBase {
    unsigned char _pad04[0x60];
    CCharString   m_string64;                      // +0x64
    unsigned char _pad68[4];
    CCharString   m_string6c;                      // +0x6c
    unsigned char _pad70[0x1c];
    CCharString   m_string8c;                      // +0x8c
    CCharString   m_string90;                      // +0x90
    virtual void v00();
    ~CDefPointeeBase_Derived_007acac0();
};

CDefPointeeBase_Derived_007acac0::~CDefPointeeBase_Derived_007acac0()
{
}