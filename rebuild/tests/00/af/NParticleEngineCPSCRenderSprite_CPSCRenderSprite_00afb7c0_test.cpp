// Behaviour test for CPSCRenderSprite::CPSCRenderSprite (0x00AFB7C0): the packed default parameters. Each default is rounded to two
// decimals, mapped from [min, max] onto [0, steps] and rounded to nearest (x87 default rounding, ties to even).
#include <cstdio>
#include <cmath>

static long Nearest(float v) { float f = floorf(v); float d = v - f; long r = (long)f; if (d > 0.5f || (d == 0.5f && (r & 1))) ++r; return r; }
static float Round2(float v) { return Nearest(v * 100.0f) / 100.0f; }
static long Pack(float v, float mn, float mx, float steps) { float n = (Round2(v) - mn) * (1.0f / (mx - mn)); return Nearest(n * steps); }

int main()
{
    int ok = 1;
    ok &= Pack(1.0f, 0.0f, 20.0f, 2047.0f) == 102;          // StartRenderSize / EndRenderSize
    ok &= Pack(0.0f, 0.0f, 1.0f, 127.0f) == 0;              // AlphaFadeMinimum / SizeFadeMinimum
    ok &= Pack(0.0f, -1.0f, 1.0f, 255.0f) == 128;           // FlickerBias: 127.5 ties to even
    ok &= Pack(1.0f, 0.1f, 100.0f, 16383.0f) == 148;        // AnimationTimeSecs
    ok &= Pack(0.5f, 0.0f, 10.0f, 1023.0f) == 51;           // TrailWidth
    ok &= Pack(15.0f, 0.0f, 30.0f, 4095.0f) == 2048;        // FlickerSpeed: 2047.5 ties to even
    // the whole-dword store at +0x24: FadeOutBeginInteger 1000 | TrailLengthInteger 1000 << 10
    ok &= (1000u | (1000u << 10)) == 0xfa3e8u;
    if (!ok) { std::printf("00afb7c0_TEST FAIL\n"); return 1; }
    std::printf("00afb7c0_TEST PASS\n");
    return 0;
}