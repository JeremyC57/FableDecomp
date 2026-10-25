// CAudioSource::StartLinearFade(__int64 now, float duration) -- start a linear gain fade of `duration` seconds: the fade length in clock ticks, the per-tick gain step 1/ticks and the end time when the remaining (1 - gain) is covered; an empty fade jumps to gain rate 1.0 and ends now
// Leaf: ?StartLinearFade@CAudioSource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C72130, length 116 bytes, ret 0Ch. Callee: __ftol2 0x00BFEA70 (twice). Callers 0x00C72289, 0x00C7229E.
// Layout (descriptive; no PDB class for the libpk OpenAL source): the landed CAudioSource model of
//   CAudioSource::InitializeProperties 0x00C72AF0 (Flags +0x28, FadeGain +0x30, SourceId +0x58, FadeEndTime +0x68,
//   PitchEndTime +0x78), extended with FadeRate +0x34, Pitch +0x3C, FadeTicks +0x70 and PitchPeriod +0x80. The audio
//   clock rate is the __int64 0x0139CA20.
// Shape: the length is converted from the stored member (fild [esi+70h]); the rate store is an `fst` the end-time
//   division reuses from the x87 stack.
extern __int64 g_AudioClockRate;                         // 0x0139CA20 (ticks per second)

struct SPitchProperties {
    float m_time;                                        // +0x00 seconds
    float m_rate;                                        // +0x04
};

struct CAudioSource {
    unsigned char  _pad00[0x28];
    unsigned long  Flags;                                // +0x28
    unsigned char  _pad2c[4];
    float          FadeGain;                             // +0x30
    float          FadeRate;                             // +0x34 (gain change per tick)
    unsigned char  _pad38[4];
    float          Pitch;                                // +0x3C
    unsigned char  _pad40[0x18];
    unsigned long  SourceId;                             // +0x58
    unsigned char  _pad5c[0xc];
    __int64        FadeEndTime;                          // +0x68
    __int64        FadeTicks;                            // +0x70
    __int64        PitchEndTime;                         // +0x78
    __int64        PitchPeriod;                          // +0x80
    void SetPitchProperties(const SPitchProperties* pitch, __int64 now);
    void StartExponentialFade(__int64 now, float duration);
    void StartLinearFade(__int64 now, float duration);
};

void CAudioSource::StartLinearFade(__int64 now, float duration)
{
    FadeTicks = (__int64)(g_AudioClockRate * duration);
    float length = (float)FadeTicks;
    if (length != 0.0f) {
        FadeRate = 1.0f / length;
        FadeEndTime = (__int64)((1.0f - FadeGain) / FadeRate) + now;
    } else {
        FadeRate = 1.0f;
        FadeEndTime = now;
    }
}