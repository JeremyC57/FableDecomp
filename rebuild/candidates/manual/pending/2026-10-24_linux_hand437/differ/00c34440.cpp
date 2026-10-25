// CVoiceParams::GetGain(const CVoiceContext* context) const -- the voice's gain through the channel's parameter block, then (for a positioned voice with a non-zero gain) scaled by the distance falloff and reduced by the second falloff times its amount
// Grade: DIFFER(145v138) -- x87 operand order: retail keeps the parameter-block result in st0 (`fst [esp+8] ; fld 0.0 ; fld st(1) ; fucompp`, popped with `fstp st0` on the positioned path) and ends `fld attenuation ; fmul value ; fmulp`; VC7.1 here spills the result (`fstp` + reload) and folds the tail into two memory `fmul`s. Call order, early-return layout and frame match
// Leaf: GetGain@CVoiceParams
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34440, 138 bytes, __thiscall `ret 4`, result in st0. Descriptive name (no PDB
//   covers this library): the catalog's `CDraw2DText_CalculateAlpha` is propagated. Twin of the pitch row 0x00C34410.
// Callees: CAudioParamBlock::GetGain(float value, unsigned short mask) 0x00C0B420 (agent431 model), the distance to the
//   listener at context +8 0x00C20340 (landed as CDragSource::BeginDrag; it tail-calls the vector length 0x00C25970 and
//   leaves the length in st0), CDistanceFalloff::GetAttenuation 0x00C1BDA0 (landed) on the falloffs at +0x14 and +0x1C.
// Constants: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
struct CAudioParamBlock {
    float GetGain(float value, unsigned short mask) const;    // 0x00C0B420
};

struct CAudioChannel {
    unsigned char _pad00[0x34];
    CAudioParamBlock* m_pParams;            // +0x34
};

struct CVector3 {
    float m_x, m_y, m_z;
};

struct CVoiceContext {
    void* m_field00;                        // +0x00
    CAudioChannel* m_pChannel;              // +0x04
    CVector3 m_listener;                    // +0x08
};

struct CVoicePosition {                     // 0x10 bytes
    CVector3 m_position;                    // +0x00
    unsigned char m_relative;               // +0x0C
    float GetDistance(const CVector3* listener) const;     // 0x00C20340
};

struct CDistanceFalloff {
    float m_near;                           // +0x00
    float m_far;                            // +0x04
    float GetAttenuation(float distance) const;            // 0x00C1BDA0
};

struct CVoiceParams {
    CVoicePosition m_position;              // +0x00
    unsigned long m_positioned;             // +0x10
    CDistanceFalloff m_falloff;             // +0x14
    CDistanceFalloff m_secondFalloff;       // +0x1C
    float m_secondAmount;                   // +0x24
    unsigned char _pad28[4];
    float m_gain;                           // +0x2C
    float m_gainScale;                      // +0x30
    float m_pitch;                          // +0x34
    float m_pitchScale;                     // +0x38
    unsigned short m_gainMask;              // +0x3C
    unsigned short m_pitchMask;             // +0x3E
    float GetGain(const CVoiceContext* context) const;
};

float CVoiceParams::GetGain(const CVoiceContext* context) const
{
    float gain = m_gainScale * m_gain;
    float value = context->m_pChannel->m_pParams->GetGain(gain, m_gainMask);
    if (value == 0.0f || !m_positioned)
        return value;
    float distance = m_position.GetDistance(&context->m_listener);
    float attenuation = m_falloff.GetAttenuation(distance);
    return attenuation * value * (1.0f - m_secondFalloff.GetAttenuation(distance) * m_secondAmount);
}
