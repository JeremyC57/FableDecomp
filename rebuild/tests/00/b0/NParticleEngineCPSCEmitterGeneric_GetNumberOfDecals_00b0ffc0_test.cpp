// Behaviour test for CPSCEmitterGeneric::GetNumberOfDecals (0x00B0FFC0): rate = round2(pps/16383*100); with a positive time
// the count is max(rate*time, start); else a life-limited, non-timeline emitter gives max((life-start)*rate, start); else a
// zero rate gives the start count, and anything else is unbounded (FLT_MAX, false).
#include <cstdio>
#include <cmath>

static float Round2(float v) { float x = v * 100.0f; long r = (long)floor(x + 0.5f); return r / 100.0f; }

struct E { unsigned start, startRand, pps, life, startTime; bool useLife, useTimeline; };

static bool GetNumberOfDecals(const E& e, float& count, float time)
{
    float rate = Round2((float)e.pps * (1.0f / 16383.0f) * 100.0f);
    float base = (float)(e.startRand + e.start);
    if (time > 0.0f) { float n = rate * time; count = n > base ? n : base; return true; }
    if (e.useLife && !e.useTimeline) {
        float life = Round2((float)e.life * (1.0f / 32767.0f) * 300.0f);
        float st = Round2((float)e.startTime * (1.0f / 32767.0f) * 300.0f);
        float n = (life - st) * rate; count = n > base ? n : base; return true;
    }
    if (rate <= 0.0f) { count = base; return true; }
    count = 3.402823466e+38f; return false;
}

int main()
{
    int ok = 1; float c;
    E e = { 5, 3, 16383, 32767, 0, false, false };          // 100 per second, start 8
    ok &= GetNumberOfDecals(e, c, 2.0f) && fabs(c - 200.0f) < 1e-3f;
    ok &= GetNumberOfDecals(e, c, 0.05f) && c == 8.0f;      // the start count wins
    e.useLife = true;                                        // life 300 s from 0
    ok &= GetNumberOfDecals(e, c, 0.0f) && fabs(c - 30000.0f) < 1e-1f;
    e.useTimeline = true;
    ok &= !GetNumberOfDecals(e, c, 0.0f) && c > 1e38f;
    e.pps = 0;
    ok &= GetNumberOfDecals(e, c, 0.0f) && c == 8.0f;
    if (!ok) { std::printf("00b0ffc0_TEST FAIL\n"); return 1; }
    std::printf("00b0ffc0_TEST PASS\n");
    return 0;
}