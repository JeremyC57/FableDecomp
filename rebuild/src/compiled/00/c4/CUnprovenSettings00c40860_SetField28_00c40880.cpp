// CUnprovenSettings_00c40860::SetField28(long value) -- store a changed value at +0x28 and set dirty bit 2
// Leaf: ?SetField28@CUnprovenSettings_00c40860@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c40880, 19 bytes, thiscall, `ret 4`.
// Identity (descriptive): it compares and stores the dword at +0x28 and sets dirty bit 2 in the mask at +0x78; its only caller is the two-argument __stdcall API wrapper 0x00C33A00 (`(object, value)`, one of a run of such wrappers 0x00C339C0..), so the block and its setters are named by offset (libpk: no PDB, no RTTI).
// Shape (family L128): `cmp [ecx+28h],eax ; je`, store, `or dword ptr [ecx+78h],2`.
struct CUnprovenSettings_00c40860 {
    unsigned char _pad00[0x28];
    long          m_field28;                // +0x28
    unsigned char _pad28[0x78 - 0x28 - 4];
    unsigned long m_dirty;                    // +0x78
    void SetField28(long value);
};

void CUnprovenSettings_00c40860::SetField28(long value)
{
    if (m_field28 != value) {
        m_field28 = value;
        m_dirty |= 2;
    }
}