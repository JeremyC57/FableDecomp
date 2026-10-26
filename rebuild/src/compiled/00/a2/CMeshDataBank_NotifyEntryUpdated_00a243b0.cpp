// CMeshDataBank::NotifyEntryUpdated(unsigned long a, unsigned long index, const CArray<char>& data) -- when the index is within the entry vector (+0x380) and that entry exists: the entry reloads (its slot 3) and this bank's slot 21 (+0x54) is told (a, index, data); otherwise slot 12 (+0x30) is told the same
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A243B0, length 96 bytes.
// Leaf: ?NotifyEntryUpdated@CMeshDataBank@@MAEXKKABV?$CArray@D@@@Z
// Shape: `this` in esi, the index in edi, the two arms each reload the other arguments and push them right to left, `ret 0Ch`.
struct CBankEntry_00a243b0 {
    virtual void v000();
    virtual void Release();                        // slot 1 (+0x04)
    virtual unsigned long Sizeof();                // slot 2 (+0x08)
    virtual void Reload();                         // slot 3 (+0x0c)
};
struct CRetailVector_00a243b0 {
    CBankEntry_00a243b0** m_first;
    CBankEntry_00a243b0** m_last;
    CBankEntry_00a243b0** m_end;
    unsigned long size() const { return m_last - m_first; }
};
struct CArray_char;

class CMeshDataBank { public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void OnEntryMissing(unsigned long a, unsigned long index, const CArray_char& data);    // slot 12 (+0x30)
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void OnEntryReloaded(unsigned long a, unsigned long index, const CArray_char& data);   // slot 21 (+0x54)
    unsigned char           _pad04[0x37c];
    CRetailVector_00a243b0  m_entries;             // +0x380
    virtual void NotifyEntryUpdated(unsigned long a, unsigned long index, const CArray_char& data);
};

void CMeshDataBank::NotifyEntryUpdated(unsigned long a, unsigned long index, const CArray_char& data)
{
    CBankEntry_00a243b0* entry = index < m_entries.size() ? m_entries.m_first[index] : 0;
    if (entry) {
        entry->Reload();
        OnEntryReloaded(a, index, data);
    } else {
        OnEntryMissing(a, index, data);
    }
}