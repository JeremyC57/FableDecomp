// CMeshDataBank::FreeDeviceSpecificResources() -- release every entry of the entry vector at +0x380 (virtual slot 1 of each non-null entry)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24000, length 80 bytes.
// Leaf: FreeDeviceSpecificResources
// Shape: `this` in esi, the index in edi compared unsigned against `size()` (re-read each iteration), the loop head aligned, `ret`.
struct CBankEntry_00a24000 {
    virtual void v000();
    virtual void Release();                        // slot 1 (+0x04)
    virtual unsigned long Sizeof();                // slot 2 (+0x08)
    virtual void Reload();                         // slot 3 (+0x0c)
};
struct CRetailVector_00a24000 {
    CBankEntry_00a24000** m_first;
    CBankEntry_00a24000** m_last;
    CBankEntry_00a24000** m_end;
    unsigned long size() const { return m_last - m_first; }
};

struct CMeshDataBank {
    unsigned char           _pad00[0x380];
    CRetailVector_00a24000  m_entries;             // +0x380
    void FreeDeviceSpecificResources();
};

void CMeshDataBank::FreeDeviceSpecificResources()
{
    for (unsigned long i = 0; i < m_entries.size(); ++i) {
        CBankEntry_00a24000* entry = m_entries.m_first[i];
        if (entry)
            entry->Release();
    }
}
