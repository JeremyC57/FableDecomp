// CUnprovenSettings_00c40860::SetField2C(long value) -- store a changed value at +0x2C and set dirty bit 4
// Leaf: ?SetField2C@CUnprovenSettings_00c40860@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c408a0, 19 bytes, thiscall, `ret 4`.
// Identity (descriptive): it compares and stores the dword at +0x2C and sets dirty bit 4 in the mask at +0x78; its only caller is the two-argument __stdcall API wrapper 0x00C33A20 (`(object, value)`, one of a run of such wrappers 0x00C339C0..), so the block and its setters are named by offset (libpk: no PDB, no RTTI).
// Shape (family L128): `cmp [ecx+2Ch],eax ; je`, store, `or dword ptr [ecx+78h],4`.
struct CUnprovenSettings_00c40860 {
    unsigned char _pad00[0x2C];
    long          m_field2C;                // +0x2C
    unsigned char _pad2C[0x78 - 0x2C - 4];
    unsigned long m_dirty;                    // +0x78
    void SetField2C(long value);
};

void CUnprovenSettings_00c40860::SetField2C(long value)
{
    if (m_field2C != value) {
        m_field2C = value;
        m_dirty |= 4;
    }
}