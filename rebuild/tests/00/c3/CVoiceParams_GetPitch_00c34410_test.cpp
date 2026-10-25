// Behaviour test for 00c34410: CVoiceParams::GetPitch -- pitch scale x pitch handed to the channel's parameter block with the pitch mask
#include <cstdio>
struct CAudioParamBlock {
    float m_factor; unsigned short m_lastMask;
    float GetPitch(float value, unsigned short mask) { m_lastMask = mask; return value * m_factor; }
};
struct CAudioChannel { unsigned char _pad00[0x34]; CAudioParamBlock* m_pParams; };
struct CVoiceContext { void* m_field00; CAudioChannel* m_pChannel; };
struct CVoiceParams {
    unsigned char _pad00[0x2C];
    float m_gain, m_gainScale, m_pitch, m_pitchScale;
    unsigned short m_gainMask, m_pitchMask;
    float GetPitch(const CVoiceContext* context) const
    {
        float pitch = m_pitchScale * m_pitch;
        return context->m_pChannel->m_pParams->GetPitch(pitch, m_pitchMask);
    }
};
int main()
{
    CAudioParamBlock block; block.m_factor = 2.0f; block.m_lastMask = 0;
    CAudioChannel channel; channel.m_pParams = &block;
    CVoiceContext context; context.m_field00 = 0; context.m_pChannel = &channel;
    CVoiceParams v; v.m_pitch = 1.5f; v.m_pitchScale = 3.0f; v.m_pitchMask = 0x0123; v.m_gainMask = 0x7777;
    float r = v.GetPitch(&context);
    float d = r - 9.0f; if (d < 0) d = -d;
    int ok = d < 1e-5f && block.m_lastMask == 0x0123;
    std::printf(ok ? "00c34410_TEST PASS\n" : "00c34410_TEST FAIL\n");
    return ok ? 0 : 1;
}