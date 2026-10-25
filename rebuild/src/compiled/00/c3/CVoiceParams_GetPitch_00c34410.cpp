// CVoiceParams::GetPitch(const CVoiceContext* context) const -- the voice's pitch scaled through the channel's parameter block (CAudioParamBlock::GetPitch with the voice's pitch mask)
// Leaf: GetPitch@CVoiceParams
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34410, 40 bytes, __fastcall `ret 4`, result in st0. Descriptive name (no PDB
//   covers this library): the catalog's `Graphics_MultiplyFloatParams` is a description. Twin of the gain row 0x00C34440.
// Callee: CAudioParamBlock::GetPitch(float value, unsigned short mask) 0x00C0B130 (agent431 model; `ret 8`, st0 result),
//   reached through context +4 -> +0x34.
// Shape: the product (`fld [+38h] ; fmul [+34h]`) is a named local, spilled through the push-ecx slot and pushed from
//   ecx after the mask; the product written inline in the call pushes it straight from st0 (35 bytes).
struct CAudioParamBlock {
    float GetPitch(float value, unsigned short mask) const;   // 0x00C0B130
};

struct CAudioChannel {
    unsigned char _pad00[0x34];
    CAudioParamBlock* m_pParams;            // +0x34
};

struct CVoiceContext {
    void* m_field00;                        // +0x00
    CAudioChannel* m_pChannel;              // +0x04
};

struct CVoiceParams {
    unsigned char _pad00[0x2C];
    float m_gain;                           // +0x2C
    float m_gainScale;                      // +0x30
    float m_pitch;                          // +0x34
    float m_pitchScale;                     // +0x38
    unsigned short m_gainMask;              // +0x3C
    unsigned short m_pitchMask;             // +0x3E
    float GetPitch(const CVoiceContext* context) const;
};

float CVoiceParams::GetPitch(const CVoiceContext* context) const
{
    float pitch = m_pitchScale * m_pitch;
    return context->m_pChannel->m_pParams->GetPitch(pitch, m_pitchMask);
}