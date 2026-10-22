// CStreamingFontDataBankEntry::AcquireFont() -- create the entry's CStreamingFontBank (0x3034 bytes, over the words at +0xc and +0x10) into the counted pointer at +4 when it is still empty, then return a copy of that counted pointer (add-reference on its count block)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5F1C0, length 96 bytes.
// Callees: operator new @ 0x00BFEA1A (0x3034), CStreamingFontBank::CStreamingFontBank @ 0x009A3C20 (on the block, +0xc, +0x10; the null arm yields 0), CCountedPointer::Reset @ 0x0041947A
// (labelled "CCountedPointer<std::vector<CCharString>>::Reset", propagated), the inlined copy constructor into the hidden return slot (`inc dword ptr [count]`).
// Shape: the counted pointer's address in esi, ebx/edi saved only on the create arm (the two constructor arguments are loaded BEFORE operator new: the bank is
// built by an inline helper whose arguments are evaluated first), `ret 4`.
struct CStreamingFontBank {
    unsigned char _body[0x3034];
    CStreamingFontBank(long a, long b);                            // 0x009A3C20
};

struct CCountedPointer_00a5f1c0 {                        // 8 bytes: object + count block (references first)
    CStreamingFontBank* m_pObject;
    long*  m_pCount;
    CCountedPointer_00a5f1c0(const CCountedPointer_00a5f1c0& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++*m_pCount;
    }
    void Reset(CStreamingFontBank* object);                        // 0x0041947A
};

struct CStreamingFontDataBankEntry {
    virtual void v00();
    CCountedPointer_00a5f1c0 m_font;                     // +0x04
    long Bank;                                      // +0x0c
    long m_b;                                      // +0x10
    static CStreamingFontBank* CreateBank(long a, long b) { return new CStreamingFontBank(a, b); }
    CCountedPointer_00a5f1c0 AcquireFont();
};

CCountedPointer_00a5f1c0 CStreamingFontDataBankEntry::AcquireFont()
{
    if (!m_font.m_pObject)
        m_font.Reset(CreateBank(Bank, m_b));
    return m_font;
}