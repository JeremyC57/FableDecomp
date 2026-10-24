// CAudioParamBlock::GetGain(float value, unsigned short mask) -- the product of value and the current values of the ramped gain parameters selected by mask (sixteen at +4), clamped to [0, 1]
// Grade: DIFFER(68v68) -- scheduling only: retail forms the array pointer between the two pushes (`push N ; add ecx,4 ; push eax`), VC7.1 here after them (`push N ; push eax ; add ecx,4`); every other byte matches
// Leaf: GetGain
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0B420, 68 bytes (`ret 8`, result in st0). Descriptive names (no PDB covers
//   this library): the manifest's "Render_ClampAlphaValue" is propagated. The block is the one the landed constructor
//   0x00C0B200 builds (sixteen ramped parameters "G0".."G15" at +4, eight factors "P0".."P7" at +0x344).
// Callees: MultiplyMaskedParams @ 0x00C0B180 (`__fastcall`: the parameters in ecx, the zero-extended mask in edx,
//   value and the count 16 on the stack).
// Constants: 0.0 @ 0x0122DEDC, 1.0 @ 0x0122DED8.
// Tried: a named pointer local for the array, /Oa, #pragma optimize("s") (70 bytes) -- the add stays after the pushes.
// Shape: two `fcom` clamps on the returned st0, each replacing it (`fstp st0 ; fld [k]`) with its own `ret 8`.
struct CRampedParam {                           // 0x34 bytes (see 0x00C0B510)
    unsigned char _body[0x34];
};

float __fastcall MultiplyMaskedParams(const CRampedParam* params, unsigned long mask, float value, unsigned long count);   // 0x00C0B180

static const float kMinGain = 0.0f;             // 0x0122DEDC
static const float kMaxGain = 1.0f;             // 0x0122DED8

struct CAudioParamBlock {
    void* m_vtable;                             // +0x000
    CRampedParam m_gains[16];                   // +0x004
    float m_pitchFactors[8];                    // +0x344
    float GetGain(float value, unsigned short mask) const;
};

float CAudioParamBlock::GetGain(float value, unsigned short mask) const
{
    float gain = MultiplyMaskedParams(m_gains, mask, value, 16);
    if (gain < kMinGain)
        return kMinGain;
    if (gain > kMaxGain)
        return kMaxGain;
    return gain;
}
