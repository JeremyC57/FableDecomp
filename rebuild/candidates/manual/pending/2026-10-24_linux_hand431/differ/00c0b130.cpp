// CAudioParamBlock::GetPitch(float value, unsigned short mask) -- the product of value and the pitch factors selected by mask (eight factors at +0x344), clamped to [0.25, 4] (two octaves either way)
// Grade: DIFFER(71v71) -- scheduling only: retail forms the array pointer between the two pushes (`push N ; add ecx,0x344 ; push eax`), VC7.1 here after them (`push N ; push eax ; add ecx,0x344`); every other byte matches
// Leaf: GetPitch
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0B130, 71 bytes (`ret 8`, result in st0). Descriptive names (no PDB covers
//   this library): the manifest's "Graphics_ClampFloatParam" is propagated. The block is the one the landed constructor
//   0x00C0B200 builds (sixteen ramped parameters "G0".."G15" at +4, eight factors "P0".."P7" at +0x344 set to 1.0).
// Callees: MultiplyMaskedFactors @ 0x00C0ACC0 (`__fastcall`: factors in ecx, the zero-extended mask in edx, value and
//   the count 8 on the stack).
// Constants: 0.25 @ 0x0123078C, 4.0 @ 0x0123088C.
// Tried (on the identical-shape twin 0x00C0B420): a named pointer local for the array, /Oa, #pragma optimize("s")
//   -- the add stays after the pushes.
// Shape: two `fcom` clamps on the returned st0, each replacing it (`fstp st0 ; fld [k]`) with its own `ret 8`.
float __fastcall MultiplyMaskedFactors(const float* factors, unsigned long mask, float value, unsigned long count);   // 0x00C0ACC0

struct CRampedParam {                           // 0x34 bytes (see 0x00C0B510)
    unsigned char _body[0x34];
};

static const float kMinPitch = 0.25f;           // 0x0123078C
static const float kMaxPitch = 4.0f;            // 0x0123088C

struct CAudioParamBlock {
    void* m_vtable;                             // +0x000
    CRampedParam m_gains[16];                   // +0x004
    float m_pitchFactors[8];                    // +0x344
    float GetPitch(float value, unsigned short mask) const;
};

float CAudioParamBlock::GetPitch(float value, unsigned short mask) const
{
    float pitch = MultiplyMaskedFactors(m_pitchFactors, mask, value, 8);
    if (pitch < kMinPitch)
        return kMinPitch;
    if (pitch > kMaxPitch)
        return kMaxPitch;
    return pitch;
}
