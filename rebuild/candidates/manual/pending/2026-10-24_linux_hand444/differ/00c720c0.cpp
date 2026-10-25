// CAudioSource::StartExponentialFade(__int64 now, float duration) -- start an exponential gain fade of `duration` seconds: the fade length in clock ticks, the per-tick log decay -ln(32768)/ticks (1.0 for an empty fade) and the end time
// Grade: DIFFER(111v111) -- x87 compare operand order only: retail `fld dword [0.0f] ; fld st(1) ; fucompp`, ours `fld st(0) ; fld dword [0.0f] ; fucompp` (everything else, incl. the after-merge pop and the eax/ecx split of the tick stores, matches) | tried: float length local, no local (`ticks != 0.0f`), double local with a double compare (qword zero), `(float)length != 0.0f` | next: `0.0f != (float)length`
// Leaf: ?StartExponentialFade@CAudioSource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C720C0, length 111 bytes, ret 0Ch. Callee: __ftol2 0x00BFEA70; the log is the x87 intrinsic (fldln2 ; fyl2x)
//   over the double 32768.0 (0x012A2428). Four callers (0x00C7232E, 0x00C7234A, 0x00C72403, 0x00C72F02).
// Layout (descriptive; no PDB class for the libpk OpenAL source): the landed CAudioSource model of
//   CAudioSource::InitializeProperties 0x00C72AF0 (Flags +0x28, FadeGain +0x30, SourceId +0x58, FadeEndTime +0x68,
//   PitchEndTime +0x78), extended with FadeRate +0x34, Pitch +0x3C, FadeTicks +0x70 and PitchPeriod +0x80. The audio
//   clock rate is the __int64 0x0139CA20.
// Shape: FadeTicks is stored straight from __ftol2 and copied into a named __int64 local (homed in the frame for the fild, its low word kept in ecx for the
//   end-time add); a DOUBLE length local keeps the fild on the x87
//   stack across both arms (popped after the merge; a float local is popped inside the arms), and the
//   `(float)length != 0.0f` compare loads the dword zero (a double compare loads a qword zero).
extern "C" double __cdecl log(double x);
#pragma intrinsic(log)

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

void CAudioSource::StartExponentialFade(__int64 now, float duration)
{
    FadeTicks = (__int64)(g_AudioClockRate * duration);
    __int64 ticks = FadeTicks;
    double length = (double)ticks;
    if ((float)length != 0.0f)
        FadeRate = (float)-(log(32768.0) / length);
    else
        FadeRate = 1.0f;
    FadeEndTime = ticks + now;
}
