// CUnprovenSettings_00c40860::SetField3C(long value) -- store a changed value at +0x3C and set dirty bit 0x40
// Leaf: ?SetField3C@CUnprovenSettings_00c40860@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c40920, 19 bytes, thiscall, `ret 4`.
// Identity (descriptive): it compares and stores the dword at +0x3C and sets dirty bit 0x40 in the mask at +0x78; its only caller is the two-argument __stdcall API wrapper 0x00C33AA0 (`(object, value)`, one of a run of such wrappers 0x00C339C0..), so the block and its setters are named by offset (libpk: no PDB, no RTTI).
// Shape (family L128): `cmp [ecx+3Ch],eax ; je`, store, `or dword ptr [ecx+78h],0x40`.
struct CUnprovenSettings_00c40860 {
    unsigned char _pad00[0x3C];
    long          m_field3C;                // +0x3C
    unsigned char _pad3C[0x78 - 0x3C - 4];
    unsigned long m_dirty;                    // +0x78
    void SetField3C(long value);
};

void CUnprovenSettings_00c40860::SetField3C(long value)
{
    if (m_field3C != value) {
        m_field3C = value;
        m_dirty |= 0x40;
    }
}