// CCombatant::ComputeSpellPower(float power) const -- 0 when the bit-19 flag is set and the value has reached its maximum, else pow(m_184 / clamp(value, min, max), power * m_1c4)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C41BB0, 134 bytes (144 with padding).
// Callees: _CIpow 0x00BFEB40 (the `pow` intrinsic). Globals: 0.0f pool 0x0122DEDC.
// The twin of ComputeDamageResistance (0x00C41990): the same capped test and clamp, but the pow
// exponent is the float argument times the member at +0x1C4 (`fld [esp+8] ; fmul [1C4h]`, `ret 4`).
//
// Shape: no PDB layout exists for CCombatant (local model, as the landed ComputeLightingAdjustment
// 0x00C41A20). `mov eax,[ecx+10h] ; shr eax,13h ; test al,1` is a one-bit test of the flag word
// through an unsigned char; `fcomp ; test ah,1 ; jne` is `value >= max`. The clamp
// (`test ah,5 ; jp` = `value < min`, `test ah,41h ; jne` = `value > max`) is an if/else chain into a
// float local that VC moves with integer registers into its frame slot (`push ecx` frame), then
// `fld [184h] ; fdiv [esp]` and the pow arguments go to _CIpow on the x87 stack. The pow result passes
// through a named float local so the call is not tail-jumped.
extern "C" double __cdecl pow(double, double);
#pragma intrinsic(pow)

struct CCombatant {
    unsigned char _pad[0x10];
    unsigned long m_flags;                  // +0x10 (bit 19: resistance capped)
    unsigned char _pad14[0x12c - 0x14];
    float m_value;                          // +0x12c
    unsigned char _pad130[0x17c - 0x130];
    float m_max;                            // +0x17c
    float m_min;                            // +0x180
    float m_base;                           // +0x184
    float m_exponent;                       // +0x188
    unsigned char _pad18c[0x1c4 - 0x18c];
    float m_spellScale;                     // +0x1c4
    float ComputeSpellPower(float power) const;
};

float CCombatant::ComputeSpellPower(float power) const
{
    if (((unsigned char)(m_flags >> 19) & 1) && m_value >= m_max)
        return 0.0f;
    float clamped;
    if (m_value < m_min)
        clamped = m_min;
    else if (m_value > m_max)
        clamped = m_max;
    else
        clamped = m_value;
    float result = (float)pow(m_base / clamped, power * m_spellScale);
    return result;
}
