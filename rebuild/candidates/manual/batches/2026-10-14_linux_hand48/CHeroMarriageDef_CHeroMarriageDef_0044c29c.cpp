// CHeroMarriageDef::CHeroMarriageDef() -- the def base (0x0044C0C0), this class's vtable (0x01232074), then the five CCharString members at +0x7c, +0xa4, +0xa8, +0xb8, +0xc4 constructed in place (0x0099E4B0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C29C, length 70 bytes.
// Shape: `this` in esi, the vtable store between the base call and the first string, `mov eax,esi`.
struct CDefBase_0044c0c0 {                        // the def base (its constructor @ 0x0044C0C0 is labelled "CShoveHaPennyRegionDef", propagated)
    virtual void v00();
    CDefBase_0044c0c0();
};
struct CCharString {
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};

struct CHeroMarriageDef : CDefBase_0044c0c0 {      // vtable 0x01232074
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
    CHeroMarriageDef();
};

CHeroMarriageDef::CHeroMarriageDef()
    : CDefBase_0044c0c0()
{
}
