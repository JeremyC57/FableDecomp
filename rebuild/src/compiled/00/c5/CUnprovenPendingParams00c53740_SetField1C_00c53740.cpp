// CUnprovenPendingParams_00c53740::SetField1C(float value) -- clamp the value into [20.0f, 2000.0f], store it at +0x1C and set dirty bit 8
// Leaf: ?SetField1C@CUnprovenPendingParams_00c53740@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c53740, 89 bytes, thiscall, `ret 4`.
// Identity (descriptive): it clamps into [20.0f, 2000.0f] (constants 0x0122F160 / 0x012A2AD4), stores at +0x1C and sets dirty bit 8 in the block's first word; the block is C3DSoundSource+0x290 (the landed SetVelocity 0x00C53E40 names that word DirtyFlags) -- callers the C3DSoundSource setter around 0x00C54040 (which keeps an integer at source+0x284 and passes it as a float), 0x00C54190 and 0x00C54510 pass `this + 0x290`; the block and its fields are named by offset.
// Shape (family L132): an inline Clamp returning by value; VC7.1 duplicates the store and the flag update into each of the
//   three exits (`v < lo` -> `test ah,5 ; jp`, `v > hi` -> `test ah,41h ; jne`).
inline float Clamp(float value, float lo, float hi)
{
    if (value < lo)
        return lo;
    if (value > hi)
        return hi;
    return value;
}

struct CUnprovenPendingParams_00c53740 {      // C3DSoundSource +0x290
    unsigned long DirtyFlags;                    // +0x00 (C3DSoundSource +0x290)
    unsigned char _pad04[0x1C - 4];
    float         m_field1C;                 // +0x1C
    void SetField1C(float value);
};

void CUnprovenPendingParams_00c53740::SetField1C(float value)
{
    m_field1C = Clamp(value, 20.0f, 2000.0f);
    DirtyFlags |= 8;
}