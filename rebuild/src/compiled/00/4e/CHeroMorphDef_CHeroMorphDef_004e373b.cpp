// CHeroMorphDef::CHeroMorphDef() -- the def base (0x0044C0C0), this class's vtable (0x01242BCC), then the four 16-byte morph records at +0x28, +0x38, +0x48, +0x58 cleared (three words and the flag byte at +0xd of each)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E373B, length 68 bytes.
// Shape: one `xor eax,eax` feeding every store after the vtable, `mov eax,esi`.
struct CDefBase_0044c0c0 {                        // the def base (its constructor @ 0x0044C0C0 is labelled "CShoveHaPennyRegionDef", propagated)
    virtual void v00();
    CDefBase_0044c0c0();
};
struct SMorphRecord_004e373b {                     // 16 bytes
    long          m_a;                             // +0x00
    long          m_b;                             // +0x04
    long          m_c;                             // +0x08
    unsigned char _padc;
    bool          m_flag;                          // +0x0d
    unsigned char _pade[2];
    SMorphRecord_004e373b() : m_a(0), m_b(0), m_c(0), m_flag(false) { }
};
class CHeroMorphDef : public CDefBase_0044c0c0 { public:         // vtable 0x01242BCC
    unsigned char          _pad04[0x24];
    SMorphRecord_004e373b  m_morph28;              // +0x28
    SMorphRecord_004e373b  m_morph38;              // +0x38
    SMorphRecord_004e373b  m_morph48;              // +0x48
    SMorphRecord_004e373b  m_morph58;              // +0x58
    virtual void v00();
    CHeroMorphDef();
};

CHeroMorphDef::CHeroMorphDef()
    : CDefBase_0044c0c0()
{
}