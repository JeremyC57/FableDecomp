// CAudioSource::SetPitchProperties(const SPitchProperties* pitch, __int64 now) -- reset the pitch state of a source: clear the pitch flag bits, derive the pitch period from the clock rate and the requested rate (at least one tick), reset the pitch to 1.0 and start the pitch timer at `now`
// Leaf: ?SetPitchProperties@CAudioSource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71F10, length 109 bytes, ret 0Ch (the time is an __int64 by value). Callee: __ftol2 0x00BFEA70. Called by
//   InitializeProperties 0x00C72B91 and the source updates 0x00C728D5 / 0x00C72F4C.
// Layout (descriptive; no PDB class for the libpk OpenAL source): the landed CAudioSource model of
//   CAudioSource::InitializeProperties 0x00C72AF0 (Flags +0x28, FadeGain +0x30, SourceId +0x58, FadeEndTime +0x68,
//   PitchEndTime +0x78), extended with FadeRate +0x34, Pitch +0x3C, FadeTicks +0x70 and PitchPeriod +0x80. The audio
//   clock rate is the __int64 0x0139CA20.
// Shape: the flag mask is written twice (before the test and again in the else arm), which VC7.1 keeps as a second
//   `and eax,0FFFFFE7Fh` on the live register; `period == 0` is the `or eax,ecx` test of the __ftol2 pair.
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

void CAudioSource::SetPitchProperties(const SPitchProperties* pitch, __int64 now)
{
    Flags &= ~0x180;
    if (pitch) {
        PitchPeriod = (__int64)(g_AudioClockRate / pitch->m_rate);
        if (PitchPeriod == 0)
            PitchPeriod = 1;
    } else
        Flags &= ~0x180;
    Pitch = 1.0f;
    PitchEndTime = now;
}