// CFontDataBankEntry::CFontDataBankEntry(long a, long b, long c) -- the CBase base constructor, this class's vtable, the counted font pointer at +4 cleared, then c, b, a stored at +0xc, +0x10, +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A63440, length 64 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. Vtable 0x0129DA70 = CFontDataBankEntry (rtti-vtables.tsv). Shape: b and c loaded before the vtable store, a after the
// counted pointer's two zero words; `this` is returned, `ret 0Ch`.
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

struct CCountedPointer_00a63440 {                        // 8 bytes: object + count block
    void* m_pObject;
    void* m_pCount;
    CCountedPointer_00a63440() : m_pObject(0), m_pCount(0) { }
};

struct CFontDataBankEntry : CBase {                        // vtable 0x0129DA70
    CCountedPointer_00a63440 m_font;                     // +0x04
    long m_c;                                      // +0x0c
    long m_b;                                      // +0x10
    long m_a;                                      // +0x14
    virtual void v00();
    CFontDataBankEntry(long a, long b, long c);
};

CFontDataBankEntry::CFontDataBankEntry(long a, long b, long c)
    : CBase()
    , m_c(c), m_b(b), m_a(a)
{
}