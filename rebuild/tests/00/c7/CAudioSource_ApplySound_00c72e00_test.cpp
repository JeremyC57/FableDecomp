// Test for CAudioSource::ApplySound (0x00C72E00): re-initialise or take the sound's property flags, OR in the forced
// context flags, push AL_LOOPING when bit 0 is set, end an expired fade-out (clearing 0x200 / 0x5e and starting the
// sound's fade once), update gain/pitch, then hand the changed flags to the hardware or software path and clear bit 0.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 int64;
#else
typedef long long int64;
#endif
static unsigned long g_forced = 0; static long g_masterPending = 0;
static long g_alLooping = -1; static int g_fades = 0; static int g_hw = 0, g_sw = 0; static unsigned long g_lastFlags = 0;
struct Fade { float Target; };
struct Sound { unsigned long StateFlags; unsigned long PropFlags; const Fade* PFade; const float* Pitch; void* LoopData; bool Streamed; };
struct Source {
    int64 FadeOutTime; unsigned long Flags, Flags2; bool Hardware;
    unsigned long InitializeProperties(Sound* s) { return s->PropFlags | 0x40000; }
    bool UpdateGain(const Fade*, bool, int64) { return false; }
    void ApplySound(Sound* sound, int64 now)
    {
        unsigned long flags = (Flags & 1) ? InitializeProperties(sound) : sound->PropFlags;
        flags |= g_forced;
        if (flags & 1) g_alLooping = (sound->LoopData && (unsigned char)(sound->StateFlags >> 8) & 1) ? 1 : 0;
        if ((unsigned char)(Flags2 >> 9) & 1) {
            if (FadeOutTime <= now) {
                Flags2 &= ~0x200; Flags &= ~0x5e;
                if (sound->PFade && !(Flags & 0x20)) { Flags |= 0x20; ++g_fades; }
            }
        }
        if (!((unsigned char)(Flags >> 6) & 1)) { if (UpdateGain(sound->PFade, (unsigned char)(flags >> 3) & 1, now)) flags |= 8; }
        if (sound->Pitch) flags |= 0x20;
        if (flags || g_masterPending) {
            if (Hardware) { ++g_hw; } else { ++g_sw; }
            g_lastFlags = flags;
        }
        Flags &= ~1;
    }
};
int main()
{
    Fade fade = { 1.0f };
    Sound s = { 0x100, 1, &fade, 0, &fade, false };
    Source src; src.FadeOutTime = 10; src.Flags = 0x5f; src.Flags2 = 0x200; src.Hardware = false;
    src.ApplySound(&s, 20);
    bool ok = g_alLooping == 1 && g_fades == 1 && src.Flags2 == 0 && src.Flags == 0x20 && g_sw == 1 && g_hw == 0
           && g_lastFlags == 0x40001;
    s.PropFlags = 0; s.LoopData = 0; src.Hardware = true; src.Flags2 = 0x200; src.FadeOutTime = 100;
    src.ApplySound(&s, 20);                            // fade not yet due, nothing changed, master idle: no hand-off
    ok = ok && g_fades == 1 && src.Flags2 == 0x200 && g_hw == 0;
    g_forced = 1; src.ApplySound(&s, 20);
    ok = ok && g_alLooping == 0 && g_hw == 1;
    std::printf(ok ? "00c72e00_TEST PASS\n" : "00c72e00_TEST FAIL\n");
    return ok ? 0 : 1;
}