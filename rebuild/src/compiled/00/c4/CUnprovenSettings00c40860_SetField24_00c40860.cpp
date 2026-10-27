// CUnprovenSettings_00c40860::SetField24(long value) -- store a changed value at +0x24 and set dirty bit 1
// Leaf: ?SetField24@CUnprovenSettings_00c40860@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c40860, 19 bytes, thiscall, `ret 4`.
// Identity (descriptive): it compares and stores the dword at +0x24 and sets dirty bit 1 in the mask at +0x78; its only caller is the two-argument __stdcall API wrapper 0x00C339E0 (`(object, value)`, one of a run of such wrappers 0x00C339C0..), so the block and its setters are named by offset (libpk: no PDB, no RTTI).
// Shape (family L128): `cmp [ecx+24h],eax ; je`, store, `or dword ptr [ecx+78h],1`.
struct CUnprovenSettings_00c40860 {
    unsigned char _pad00[0x24];
    long          m_field24;                // +0x24
    unsigned char _pad24[0x78 - 0x24 - 4];
    unsigned long m_dirty;                    // +0x78
    void SetField24(long value);
};

void CUnprovenSettings_00c40860::SetField24(long value)
{
    if (m_field24 != value) {
        m_field24 = value;
        m_dirty |= 1;
    }
}