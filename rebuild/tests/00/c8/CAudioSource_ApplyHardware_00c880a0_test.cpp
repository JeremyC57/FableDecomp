// Behaviour test for CAudioSource::ApplyHardware (0x00C880A0): EAX direct level = offset + 2000*log10(distance x cone gain)
// (-10000 for silence) clamped to [-10000, 1000]; room block with a zero room-rolloff; AL gain capped at 1; AL pitch retried
// at 80% while alGetError reports AL_INVALID_VALUE; AL position with z negated, normalised without caps bit 0x2000.
#include <cstdio>
#include <cmath>
struct Call { int kind; unsigned long prop; float f; long l; float v[3]; };
static Call g_calls[32]; static int g_count = 0; static int g_rejects = 0;
static void EAXSet(unsigned long prop, long value) { Call c = Call(); c.kind = 1; c.prop = prop; c.l = value; g_calls[g_count++] = c; }
static void EAXSetF(unsigned long prop, float value) { Call c = Call(); c.kind = 1; c.prop = prop; c.f = value; g_calls[g_count++] = c; }
static void Sourcef(unsigned long prop, float value) { Call c = Call(); c.kind = 2; c.prop = prop; c.f = value; g_calls[g_count++] = c; }
static long GetError() { if (g_rejects > 0) { --g_rejects; return 0xa003; } return 0; }
static void Sourcefv(unsigned long prop, const float* v) { Call c = Call(); c.kind = 3; c.prop = prop; c.v[0] = v[0]; c.v[1] = v[1]; c.v[2] = v[2]; g_calls[g_count++] = c; }
static long AmplitudeToMillibels(float a)
{
    if (a == 0.0f) return -10000;
    long r = (long)floor(log10(a) * 2000.0f + 0.5f);
    return r < -10000 ? -10000 : r;
}
struct Obj { unsigned long caps; float gain, pitch; float pos[3]; float cone, dist, doppler; long directOffset, directHF; float hfRef;
             long room, roomHF; float roomHFRef; };
static void Apply(const Obj* o, float srcGain, float srcPitch, float master, unsigned long flags)
{
    if ((flags & 0x400000) || ((flags & 0xa48002) && !(o->caps & 0x2000))) {
        long direct = o->directOffset + AmplitudeToMillibels(o->dist * o->cone);
        if (direct > 1000) direct = 1000; else if (direct < -10000) direct = -10000;
        EAXSet(5, direct);
        if (flags & 0x400000) { EAXSet(6, o->directHF); EAXSetF(22, o->hfRef); }
    }
    if (flags & 0x1000000) { EAXSet(7, o->room); EAXSet(8, o->roomHF); EAXSetF(20, 0.0f); EAXSetF(22, o->roomHFRef); }
    if (flags & 0xa4800a) {
        float gain = o->gain * srcGain;
        if (o->caps & 0x2000) { gain *= o->dist; gain *= o->cone; }
        gain *= master; if (gain > 1.0f) gain = 1.0f;
        Sourcef(0x100a, gain);
    }
    if (flags & 0x10024) {
        GetError();
        float pitch = o->doppler * o->pitch * srcPitch; long error;
        do { if (!(pitch > 0.0f)) break; Sourcef(0x1003, pitch); error = GetError(); pitch *= 0.8f; } while (error == 0xa003);
    }
    if (flags & 0x8000) {
        float p[3] = { o->pos[0], o->pos[1], -o->pos[2] };
        if (!(o->caps & 0x2000)) {
            float l2 = o->pos[0] * o->pos[0] + (o->pos[1] * o->pos[1] + o->pos[2] * o->pos[2]);
            if (l2 != 0.0f && std::sqrt(l2) != 0.0f) { float s = 1.0f / std::sqrt(l2); p[0] *= s; p[1] *= s; p[2] *= s; }
            else p[1] = 1.0f;
        }
        Sourcefv(0x1004, p);
    }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Obj o = { 0, 0.5f, 1.0f, { 3.0f, 0.0f, 4.0f }, 0.1f, 1.0f, 2.0f, 300, -100, 5000.0f, -200, -300, 250.0f };
    Apply(&o, 1.0f, 1.0f, 1.0f, 0x400000 | 0x1000000);
    bool ok = g_count == 7 && g_calls[0].prop == 5 && g_calls[0].l == 300 - 2000 && g_calls[1].l == -100 && g_calls[2].f == 5000.0f
              && g_calls[3].l == -200 && g_calls[4].l == -300 && g_calls[5].prop == 20 && g_calls[5].f == 0.0f && g_calls[6].f == 250.0f;
    g_count = 0; o.directOffset = 5000; Apply(&o, 1.0f, 1.0f, 1.0f, 2); ok = ok && g_count == 2 && g_calls[0].l == 1000 && Near(g_calls[1].f, 0.5f);
    g_count = 0; o.cone = 0.0f; o.caps = 0x2000; Apply(&o, 4.0f, 1.0f, 2.0f, 2); ok = ok && g_count == 1 && g_calls[0].f == 0.0f;
    g_count = 0; o.cone = 1.0f; Apply(&o, 4.0f, 1.0f, 2.0f, 2); ok = ok && g_count == 1 && g_calls[0].f == 1.0f;
    // the first alGetError only clears a stale error; pitch 2.0 is then rejected 3 times: 2.0, 1.6, 1.28, 1.024
    g_count = 0; g_rejects = 4; Apply(&o, 1.0f, 1.0f, 1.0f, 0x20);
    ok = ok && g_count == 4 && Near(g_calls[0].f, 2.0f) && Near(g_calls[3].f, 1.024f);
    // a position change also refreshes the direct level and the gain
    g_count = 0; o.caps = 0; Apply(&o, 1.0f, 1.0f, 1.0f, 0x8000);
    ok = ok && g_count == 3 && g_calls[0].prop == 5 && g_calls[1].prop == 0x100a && g_calls[2].kind == 3
            && Near(g_calls[2].v[0], 0.6f) && Near(g_calls[2].v[2], -0.8f);
    g_count = 0; o.pos[0] = o.pos[2] = 0.0f; Apply(&o, 1.0f, 1.0f, 1.0f, 0x8000);
    ok = ok && g_count == 3 && g_calls[2].v[1] == 1.0f && g_calls[2].v[0] == 0.0f;
    if (!ok) { std::printf("00c880a0_TEST FAIL\n"); return 1; }
    std::printf("00c880a0_TEST PASS\n");
    return 0;
}