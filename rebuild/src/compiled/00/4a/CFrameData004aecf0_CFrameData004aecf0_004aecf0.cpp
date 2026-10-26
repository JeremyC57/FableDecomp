// CFrameData_004aecf0::CFrameData_004aecf0() -- a non-polymorphic 0x20-byte record: a CCharString at +0xc, two other string objects at +0x10 and +0x14 (constructor 0x0099AED0), two flag bytes at +0x19/+0x1a and the word at +0x1c cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AECF0, length 42 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0 (+0xc), the string constructor @ 0x0099AED0 twice (+0x10, +0x14; labelled CCharString::CCharString, another
// overload). Shape: no vtable store, the three clears share eax, `this` is returned. The manifest label "CFrameData::ResetTexture" is propagated.
#pragma optimize("s", on)
class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
};
struct CNameString_0099aed0 {
    void* m_pData;
    CNameString_0099aed0();                        // 0x0099AED0
};

struct CFrameData_004aecf0 {
    unsigned char        _pad[0xc];               // +0x00 .. +0x0c (not initialised)
    CCharString          m_name;                   // +0x0c
    CNameString_0099aed0 m_textA;                  // +0x10
    CNameString_0099aed0 m_textB;                  // +0x14
    unsigned char        _pad18;                   // +0x18 (not initialised)
    bool                 m_flagA;                  // +0x19
    bool                 m_flagB;                  // +0x1a
    unsigned char        _pad1b;
    long                 m_value;                  // +0x1c
    CFrameData_004aecf0();
};

CFrameData_004aecf0::CFrameData_004aecf0()
    : m_flagA(false), m_flagB(false), m_value(0)
{
}