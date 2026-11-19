// Behaviour test for CReverbProcessor::UpdateTapDelays (0x00C78150): the room scale clamped to 0.9..2.5 maps to 0..1
// ((room - 0.8) / 1.7, 1.0 at or above 2.5); both outputs get the rounded base delay; each of the four tap columns gets
// one line segment, the tap delay is the time step from the previous column's latest tap (clamped to segment - 128),
// placed after the scaled line base (scaled for the first column only), rounded up to a multiple of 4; the first three
// columns record the running maximum. Model of the retail arithmetic (round-half-down through round-to-nearest-even).
#include <stdio.h>
#include <string.h>
#include <math.h>

static const float s_tapTimes[2][4] = {
    { 0.0388f, 0.3684f, 0.5717f, 0.9417f },
    { 0.0206f, 0.3166f, 0.6408f, 0.8517f },
};

static long RoundNearestEven(float value)
{
    double d = value;
    double down = floor(d);
    long r = (long)down;
    double frac = d - down;
    if (frac > 0.5 || (frac == 0.5 && (r & 1)))
        ++r;
    return r;
}

static long RoundHalfDownToLong(float value)
{
    long result = RoundNearestEven(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}

struct Result { long base; unsigned long taps[2][4]; unsigned long extent[3]; };

static Result Update(float room, unsigned long rate, unsigned long lineBase, unsigned long lineBytes)
{
    Result out;
    memset(&out, 0, sizeof(out));
    float scale;
    if (room < 0.9f)
        room = 0.9f;
    if (room < 2.5f)
        scale = (room - 0.8f) * 0.5882353f;
    else
        scale = 1.0f;
    out.base = (RoundHalfDownToLong((float)rate * scale) + 3) & ~3;
    unsigned long segment = lineBytes >> 2;
    unsigned long extent = 0;
    float previous = 0.0f;
    for (unsigned int i = 0; i < 4; ++i) {
        for (unsigned int j = 0; j < 2; ++j) {
            unsigned long offset = i * segment;
            unsigned long delay = RoundHalfDownToLong((s_tapTimes[j][i] - previous) * (float)rate * scale);
            if (delay > segment - 128)
                delay = segment - 128;
            unsigned long tap = (RoundHalfDownToLong((float)lineBase * (i == 0 ? scale : 1.0f) + (float)delay + (float)offset) + 3) & ~3;
            out.taps[j][i] = tap;
            if (i < 3 && extent < tap) {
                extent = tap;
                out.extent[i] = tap;
            }
        }
        previous = 0.0f;
        if (previous < s_tapTimes[0][i])
            previous = s_tapTimes[0][i];
        if (previous < s_tapTimes[1][i])
            previous = s_tapTimes[1][i];
    }
    return out;
}

int main()
{
    Result a = Update(1.0f, 44100, 1000, 0x10000);
    bool ok = a.base == 5188 && a.taps[0][0] == 320 && a.taps[1][0] == 224 && a.taps[0][1] == 19096 && a.taps[1][3] == 51248
        && a.extent[0] == 320 && a.extent[1] == 19096 && a.extent[2] == 35180;
    Result b = Update(0.5f, 22050, 800, 0x8000);              // clamped up to 0.9
    ok = ok && b.base == 1300 && b.taps[0][0] == 100 && b.taps[1][2] == 17536 && b.extent[2] == 17536;
    Result c = Update(3.0f, 48000, 0, 0x2000);                // scale 1.0; delays clamp to segment - 128
    ok = ok && c.base == 48000 && c.taps[0][1] == 3968 && c.taps[1][3] == 8064 && c.extent[0] == 1864;
    for (int j = 0; j < 2; ++j)
        for (int i = 0; i < 4; ++i)
            ok = ok && (a.taps[j][i] & 3) == 0 && (b.taps[j][i] & 3) == 0 && (c.taps[j][i] & 3) == 0;
    printf(ok ? "00c78150_TEST PASS\n" : "00c78150_TEST FAIL\n");
    return ok ? 0 : 1;
}