// Behaviour test for CVtable_012bed90::RebuildGain (0x00C6DEA0): stores the selector, rebuilds the Q15 gain and step
// (gain >> 7) of the selected record set from scale * volume * 32767 (rounded, capped at 0x7FFF): the front pair always,
// the rear pair only with four or more output channels; the other record set is untouched.
#include <cstdio>

struct Ramp { unsigned short gain; short step; };
static long RoundToLong(float v) { return (long)(v >= 0.0f ? v + 0.5f : v - 0.5f); }

struct Effect {
    float volume; bool alternate; Ramp first[6], second[6]; Ramp* current; Ramp* previous; float scales[6];
    unsigned int channels;
    void Compute(Ramp* r, float scale)
    {
        unsigned long g = (unsigned long)RoundToLong(scale * volume * 32767.0f);
        if (g > 0x7fff) g = 0x7fff;
        r->gain = (unsigned short)g;
        r->step = (short)((unsigned short)g >> 7);
    }
    void RebuildGain(bool alt)
    {
        alternate = alt;
        Ramp* set = alt ? previous : current;
        Compute(&set[0], scales[0]);
        Compute(&set[1], scales[1]);
        if (channels >= 4) { Compute(&set[2], scales[2]); Compute(&set[3], scales[3]); }
    }
};

int main()
{
    int ok = 1;
    Effect e;
    for (int i = 0; i < 6; ++i) { e.first[i].gain = 1; e.first[i].step = 1; e.second[i].gain = 1; e.second[i].step = 1; }
    e.current = e.first; e.previous = e.second; e.volume = 1.0f; e.channels = 2;
    e.scales[0] = 0.5f; e.scales[1] = 2.0f; e.scales[2] = 0.25f; e.scales[3] = 0.0f;
    e.RebuildGain(false);
    ok &= !e.alternate && e.first[0].gain == 16384 && e.first[0].step == 128 && e.first[1].gain == 0x7fff;
    ok &= e.first[1].step == 255 && e.first[2].gain == 1 && e.second[0].gain == 1;
    e.channels = 4;
    e.RebuildGain(true);
    ok &= e.alternate && e.second[2].gain == 8192 && e.second[2].step == 64 && e.second[3].gain == 0 && e.first[2].gain == 1;
    if (!ok) { std::printf("00c6dea0_TEST FAIL\n"); return 1; }
    std::printf("00c6dea0_TEST PASS\n");
    return 0;
}