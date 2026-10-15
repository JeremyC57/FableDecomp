// CCombatant::ComputeDamageResistance() const -- 0 when the bit-19 flag is set and the value has reached its maximum, else pow(m_184 / clamp(value, min, max), g_scale * m_188)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C41990, 132 bytes (144 with padding).
// Callees: _CIpow 0x00BFEB40 (the `pow` intrinsic). Globals: 0.0f pool 0x0122DEDC, scale float 0x0143BBF8.
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

extern float g_damageResistanceScale;       // 0x0143BBF8

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
    float ComputeDamageResistance() const;
};

float CCombatant::ComputeDamageResistance() const
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
    float result = (float)pow(m_base / clamped, g_damageResistanceScale * m_exponent);
    return result;
}