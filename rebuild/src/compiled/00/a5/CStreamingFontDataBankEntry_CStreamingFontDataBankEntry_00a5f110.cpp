// CStreamingFontDataBankEntry::CStreamingFontDataBankEntry(long a, long b, long c) -- the CBase base constructor, this class's vtable, the counted font pointer at +4 cleared, then c, b, a stored at +0xc, +0x10, +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5F110, length 64 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. Vtable 0x0129D5B4 = CStreamingFontDataBankEntry (rtti-vtables.tsv). Shape: b and c loaded before the vtable store, a after the
// counted pointer's two zero words; `this` is returned, `ret 0Ch`.
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

struct CCountedPointer_00a5f110 {                        // 8 bytes: object + count block
    void* m_pObject;
    void* m_pCount;
    CCountedPointer_00a5f110() : m_pObject(0), m_pCount(0) { }
};

class CStreamingFontDataBankEntry : public CBase { public:                        // vtable 0x0129D5B4
    CCountedPointer_00a5f110 m_font;                     // +0x04
    long Bank;                                      // +0x0c
    long m_b;                                      // +0x10
    long m_a;                                      // +0x14
    virtual void v00();
    CStreamingFontDataBankEntry(long a, long b, long c);
};

CStreamingFontDataBankEntry::CStreamingFontDataBankEntry(long a, long b, long c)
    : CBase()
    , Bank(c), m_b(b), m_a(a)
{
}