// Grade: DIFFER(304v273) -- retail keeps the wrapped value on the x87 stack through the angular wrap (fcom/fsub/fadd merge, then fst into the dead buffer slot); ours spills it each step; class identity unconfirmed (audio library region 0x00C3xxxx, label CWorld::AnimationCurve_ApplyTiming)
// CGainEnvelope_00c3a7d0::ApplyToBuffer(CSampleBuffer&, unsigned long) -- audio: clamp the envelope value to [0,1] (wrapping it into [-pi,pi) first for angular envelopes), advance the envelope by the buffer's duration in ms and scale every 16-bit sample by the value
// Leaf: ApplyToBuffer
const float g_fPi = 3.14159274f;                     // 0x0128F604
const float g_fTwoPi = 6.28318548f;                  // 0x0128F608
const float g_fMinusPi = -3.14159274f;               // 0x012B1EB8
struct CSampleBuffer {
    unsigned long Channels;                          // +0x00
    unsigned long _pad04;
    unsigned long Frames;                            // +0x08
    unsigned long _pad0c;
    short*        Samples;                           // +0x10
    short* GetSamples();                             // 0x00C491D0
};
struct CGainEnvelope_00c3a7d0 {
    float         Value;                             // +0x00
    unsigned char _pad04[0x10];
    float         Time;                              // +0x14
    float         Duration;                          // +0x18
    unsigned char _pad1c[0x14];
    bool          Angular;                           // +0x30
    void Advance(float deltaMs);                     // 0x00C0B7B0
    void ApplyToBuffer(CSampleBuffer& buffer, unsigned long sampleRate);
};
void CGainEnvelope_00c3a7d0::ApplyToBuffer(CSampleBuffer& buffer, unsigned long sampleRate)
{
    if (!buffer.Frames)
        return;
    float gain = Value;
    if (Angular) {
        if (gain >= g_fPi)
            gain -= g_fTwoPi;
        else if (gain < g_fMinusPi)
            gain += g_fTwoPi;
    }
    if (gain < 0.0f)
        gain = 0.0f;
    else if (gain > 1.0f)
        gain = 1.0f;
    Advance((float)buffer.Frames * 1000.0f / (float)sampleRate);
    short* sample = buffer.GetSamples();
    long scale = (long)(gain * 32767.0f);
    for (unsigned long frames = buffer.Frames; frames; --frames) {
        for (unsigned long n = buffer.Channels; n; --n) {
            *sample = (short)(*sample * (unsigned short)scale / 32767);
            ++sample;
        }
    }
}
