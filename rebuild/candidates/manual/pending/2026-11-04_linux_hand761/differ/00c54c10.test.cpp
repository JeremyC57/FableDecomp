// Behaviour test for CAudioSource::ApplyParameters (0x00C54C10): each dirty bit pushes its own parameter (step 0x10074, direction
// 0x8000, direct filter 0x400000, effect filter 0x1000000 only with a volume scaler, pan gains 0x800000 unless the format is
// 0x2000, filters 0x1d0); the volume target is amplitude * level scales * 32767 (muted to 0 mB on non-v1 EAX for flag 0x2000), a
// change sets the ramp flag; the stream always gets its parameters; the filter flags record which filters were set.
#include <stdio.h>
#include <math.h>
#include <string.h>

static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }

static float MillibelsToAmplitude(long mb) { return mb <= -10000 ? 0.0f : (float)pow(10.0, mb / 2000.0); }

struct Source {
    bool scaler; long eaxVersion; unsigned long flags; unsigned long format; float levelScale; float master;
    short targetVolume; bool ramp; float streamGain;
    void Apply(unsigned long dirty, long volumeMb, long levelMb, unsigned long sourceFlags, float voiceLevel, bool hasFilter)
    {
        if (dirty & 0x10074) Log("step");
        if (dirty & 0x8000) Log("direction");
        if (dirty & 0x400000) Log("direct");
        if (scaler) {
            float amp = (eaxVersion != 0 && eaxVersion != 1 && (sourceFlags & 0x2000)) ? MillibelsToAmplitude(0) : MillibelsToAmplitude(volumeMb);
            short target = (short)floor(amp * levelScale * voiceLevel * master * 32767.0f + 0.5f);
            if (target != targetVolume) { targetVolume = target; ramp = true; }
            if (dirty & 0x1000000) Log("effect");
        }
        if ((dirty & 0x800000) && (format & 0x3c00) != 0x2000) Log("pan");
        streamGain = MillibelsToAmplitude(levelMb) * levelScale * voiceLevel * master;
        if (dirty & 0x1d0) {
            flags &= ~0x1e0000;
            if (hasFilter) flags |= 0x20000;
        }
    }
};

int main()
{
    Source s = { true, 3, 0x1e0000, 0, 1.0f, 1.0f, 0, false, 0.0f };
    s.Apply(0x10074 | 0x8000 | 0x1000000 | 0x800000 | 0x40, 0, 0, 0, 0.5f, true);
    bool ok = strcmp(g_log, "step,direction,effect,pan,") == 0 && s.targetVolume == 16384 && s.ramp && s.flags == 0x20000
        && fabs(s.streamGain - 0.5f) < 1e-6f;
    g_log[0] = 0; s.ramp = false;
    s.scaler = false; s.format = 0x2000;
    s.Apply(0x800000 | 0x1000000, -2000, -2000, 0x2000, 1.0f, false);
    ok = ok && g_log[0] == 0 && !s.ramp && fabs(s.streamGain - 0.1f) < 1e-6f;
    printf(ok ? "00c54c10_TEST PASS\n" : "00c54c10_TEST FAIL\n");
    return ok ? 0 : 1;
}
