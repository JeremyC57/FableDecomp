// CFractal::NewFractalData(CFractalSource* source) -- fill the eight 0x1c-byte entries at +0x18 with a fresh value from the source (0x00C1BD50, a float) each
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C457E0, length 48 bytes.
// Callees: the source's value getter @ 0x00C1BD50 (labelled "CMap::DrawGetSeaLevel", propagated; a float in st0 stored into each entry's first word). Shape: a
// running entry address in esi and a count-down in edi (eight iterations, `nop`-aligned top), the source kept in ebx, `ret 4`.
struct CFractalSource_00c457e0 {
    float GetValue();                              // 0x00C1BD50
};
struct SFractalEntry_00c457e0 {
    float         m_value;                         // +0x00
    unsigned char _pad[0x18];                      // 0x1c bytes
};

struct CFractal {
    unsigned char            _pad[0x18];
    SFractalEntry_00c457e0   m_entries[8];         // +0x18
    void NewFractalData(CFractalSource_00c457e0* source);
};

void CFractal::NewFractalData(CFractalSource_00c457e0* source)
{
    for (int i = 0; i < 8; ++i)
        m_entries[i].m_value = source->GetValue();
}
