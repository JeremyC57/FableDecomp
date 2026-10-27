// CUnprovenPendingParams_00c53740::SetField38(float value) -- clamp the value into [0.0f, 1.0f], store it at +0x38 and set dirty bit 2
// Leaf: ?SetField38@CUnprovenPendingParams_00c53740@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c53980, 89 bytes, thiscall, `ret 4`.
// Identity (descriptive): it clamps into [0.0f, 1.0f] (constants 0x0122DEDC / 0x0122DED8), stores at +0x38 and sets dirty bit 2 in the block's first word; the block is C3DSoundSource+0x290 (the landed SetVelocity 0x00C53E40 names that word DirtyFlags) -- callers the C3DSoundSource setter 0x00C53F30 (tail jump after storing source+0x26C), 0x00C54190 and 0x00C54510 pass `this + 0x290`; the block and its fields are named by offset.
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
    unsigned char _pad04[0x38 - 4];
    float         m_field38;                 // +0x38
    void SetField38(float value);
};

void CUnprovenPendingParams_00c53740::SetField38(float value)
{
    m_field38 = Clamp(value, 0.0f, 1.0f);
    DirtyFlags |= 2;
}