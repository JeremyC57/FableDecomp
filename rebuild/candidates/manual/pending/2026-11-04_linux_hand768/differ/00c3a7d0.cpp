// CRampedParam::ApplyGainToFrame(CCodaFrame& frame, unsigned long sampleRate) -- audio: take the ramped gain (wrapped into [-pi, pi) for an angle parameter) clamped to [0, 1], advance the ramp by the frame's duration in ms, then scale every 16-bit sample of the frame by the gain
// Grade: DIFFER(272v273) -- x87 store order: retail stores the clamped gain before the 0 compare (`fst [gain]; fcomp 0`) and reloads it for the > 1 test; VC7.1 compares first (`fcom; fst`) and keeps it in st(0), one byte less (plus a loop-alignment nop). Tried ternary / Clamp templates / std::min,max / separate ifs / /Op /Os /O1 /G5-/G7 (~22 grades).
// Leaf: ?ApplyGainToFrame@CRampedParam@@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3A7D0, 273 bytes (+ int3 padding), __thiscall `ret 8`.
// Library: libpk audio (no PDB/RTTI): descriptive names. `this` is the 0x34-byte ramped parameter of the landed CRampedParam
//   rows (Update 0x00C0B7B0, SetTarget 0x00C0B570; the out-of-line GetValue copy 0x00C0B730 is inline here); its only caller
//   0x00C39007 applies the voice's +0x138 gain parameter to the frame its source returned. The frame is the landed CCodaFrame
//   (.\CFrame.cpp; GetData 0x00C491D0 = +0x10). The method name is descriptive (labels: AudioLib_ApplyGainEnvelope_00c3a7d0).
// Constants: pi 0x0128F604, 2*pi 0x0128F608, -pi 0x012B1EB8, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 2^32 0x0122DCB4 (the unsigned
//   to float fix-ups), 1000.0f 0x01252370, 32767.0f 0x0129C820; __ftol2 0x00BFEA70.
struct CCodaFrame {
    unsigned long m_channels;                   // +0x00
    unsigned long m_samples;                    // +0x04 capacity in samples per channel
    unsigned long m_validSamples;               // +0x08
    unsigned long m_position;                   // +0x0c
    short*        m_pData;                      // +0x10 interleaved 16-bit PCM
    short* GetData();                           // 0x00C491D0
};

struct CRampedParam {                           // 0x34-byte ramped audio parameter (chunk 431 model)
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    float m_targetRate;                         // +0x08 rate on arrival
    float m_rate;                               // +0x0c current rate
    float m_10;                                 // +0x10
    float m_time;                               // +0x14 elapsed ramp time
    float m_duration;                           // +0x18 ramp duration
    float m_coef[5];                            // +0x1c Taylor coefficients of the ramp
    bool  m_bAngle;                             // +0x30 wrap the value into [-pi, pi)
    void  Update(float timeStep);               // 0x00C0B7B0
    float GetValue() const;                     // out-of-line copy 0x00C0B730
    void  ApplyGainToFrame(CCodaFrame& frame, unsigned long sampleRate);
};

static const float kPi = 3.14159265f;           // 0x0128F604
static const float kTwoPi = 6.28318531f;        // 0x0128F608
static const float kMinusPi = -3.14159265f;     // 0x012B1EB8

inline float CRampedParam::GetValue() const
{
    float value = m_value;
    if (m_bAngle) {
        if (value >= kPi)
            value -= kTwoPi;
        else if (value < kMinusPi)
            value += kTwoPi;
    }
    return value;
}

void CRampedParam::ApplyGainToFrame(CCodaFrame& frame, unsigned long sampleRate)
{
    if (!frame.m_validSamples)
        return;
    float gain = GetValue();
    if (gain < 0.0f)
        gain = 0.0f;
    else if (gain > 1.0f)
        gain = 1.0f;
    Update((float)frame.m_validSamples * 1000.0f / (float)sampleRate);
    short* sample = frame.GetData();
    unsigned long frames = frame.m_validSamples;
    long scale = (long)(gain * 32767.0f);
    for (; frames; --frames) {
        for (unsigned long n = frame.m_channels; n; --n) {
            *sample = (short)(*sample * (unsigned short)scale / 32767);
            ++sample;
        }
    }
}
