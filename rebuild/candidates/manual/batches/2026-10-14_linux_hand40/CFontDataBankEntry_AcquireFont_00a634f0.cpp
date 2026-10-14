// CFontDataBankEntry::AcquireFont() -- create the entry's CStaticFontBank (0x2034 bytes, over the words at +0xc and +0x10) into the counted pointer at +4 when it is still empty, then return a copy of that counted pointer (add-reference on its count block)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A634F0, length 96 bytes.
// Callees: operator new @ 0x00BFEA1A (0x2034), CStaticFontBank::CStaticFontBank @ 0x00AB8E10 (on the block, +0xc, +0x10; the null arm yields 0), CCountedPointer::Reset @ 0x0041947A
// (labelled "CCountedPointer<std::vector<CCharString>>::Reset", propagated), the inlined copy constructor into the hidden return slot (`inc dword ptr [count]`).
// Shape: the counted pointer's address in esi, ebx/edi saved only on the create arm (the two constructor arguments are loaded BEFORE operator new: the bank is
// built by an inline helper whose arguments are evaluated first), `ret 4`.
struct CStaticFontBank {
    unsigned char _body[0x2034];
    CStaticFontBank(long a, long b);                            // 0x00AB8E10
};

struct CCountedPointer_00a634f0 {                        // 8 bytes: object + count block (references first)
    CStaticFontBank* m_pObject;
    long*  m_pCount;
    CCountedPointer_00a634f0(const CCountedPointer_00a634f0& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++*m_pCount;
    }
    void Reset(CStaticFontBank* object);                        // 0x0041947A
};

struct CFontDataBankEntry {
    virtual void v00();
    CCountedPointer_00a634f0 m_font;                     // +0x04
    long m_c;                                      // +0x0c
    long m_b;                                      // +0x10
    static CStaticFontBank* CreateBank(long a, long b) { return new CStaticFontBank(a, b); }
    CCountedPointer_00a634f0 AcquireFont();
};

CCountedPointer_00a634f0 CFontDataBankEntry::AcquireFont()
{
    if (!m_font.m_pObject)
        m_font.Reset(CreateBank(m_c, m_b));
    return m_font;
}
