// CGraphicDataBank::NotifyEntryUpdated(unsigned long a, unsigned long entry, const CArray<char>& data) -- when `entry` indexes the 44-byte entry vector at +0x1e0 and that entry's word at +0x24 is not -1, tell virtual slot 14 about it; then forward all three arguments to virtual slot 12
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD910, length 96 bytes.
// Callees: this bank's virtual slots 14 (+0x38: the entry) and 12 (+0x30: a, entry, data). Shape: the element count through the reciprocal multiply, the unsigned bounds
// test (`jae`), the entry address by `imul 2Ch`, `ret 0Ch`. Protected virtual (the mangled name).
// Leaf: ?NotifyEntryUpdated@CGraphicDataBank@@MAEXKKABV?$CArray@D@@@Z
struct CArrayChar;
struct SEntry44_009fd910 {
    unsigned char _pad[0x24];
    long          m_slot;                          // +0x24 (-1 = none)
    unsigned char _pad28[4];
};

struct CEntryVector_009fd910 {                     // the 44-byte entry vector (first, last)
    SEntry44_009fd910* m_pFirst;
    SEntry44_009fd910* m_pLast;
    unsigned long size() const { return m_pLast - m_pFirst; }
    SEntry44_009fd910& operator[](unsigned long i) { return m_pFirst[i]; }
};

class CGraphicDataBank { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void OnEntryUpdated(unsigned long a, unsigned long entry, const CArrayChar& data);   // slot 12 (+0x30)
    virtual void v013();
    virtual void OnEntrySlotUpdated(unsigned long entry);                                        // slot 14 (+0x38)
    unsigned char      _pad04[0x1dc];
    CEntryVector_009fd910 m_entries;               // +0x1e0
protected:
    virtual void NotifyEntryUpdated(unsigned long a, unsigned long entry, const CArrayChar& data);
};

void CGraphicDataBank::NotifyEntryUpdated(unsigned long a, unsigned long entry, const CArrayChar& data)
{
    if (entry < m_entries.size() && m_entries[entry].m_slot != -1)
        OnEntrySlotUpdated(entry);
    OnEntryUpdated(a, entry, data);
}
