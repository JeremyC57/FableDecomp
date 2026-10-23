// Behaviour test for CTCElectrocutedEffect::FrameUpdate (007ce420): fade factor per phase, whitened tint (Color * t + (1 - t) * 255), highlight alpha/width, and removal after the fade-out
#include <cstdio>

struct Colour { unsigned char B, G, R, A; };
struct Effect { long CurTime, FadeInTime, FadeOutTimeStart, FadeOutTime; };
struct Result { bool applied; float t; Colour tint; unsigned char highlightAlpha; float width; bool removed; };

static Result FrameUpdate(Effect& e, const long* colour, float highlightWidth)
{
    Result r;
    r.applied = false;
    r.removed = false;
    float t = 0.0f;
    if (e.CurTime <= e.FadeInTime) {
        t = (float)e.CurTime / e.FadeInTime;
        if (!e.FadeInTime) t = 1.0f;
        r.applied = true;
    } else if (e.CurTime >= e.FadeOutTimeStart) {
        t = 1.0f - (float)(e.CurTime - e.FadeOutTimeStart) / e.FadeOutTime;
        if (!e.FadeOutTime) t = 1.0f;
        r.applied = true;
    }
    if (r.applied) {
        long white = (long)((1.0f - t) * 255.0f);
        r.t = t;
        r.tint.R = (unsigned char)(long)(colour[0] * t + white);
        r.tint.G = (unsigned char)(long)(colour[1] * t + white);
        r.tint.B = (unsigned char)(long)(colour[2] * t + white);
        r.tint.A = (unsigned char)(long)(colour[3] * t + white);
        r.highlightAlpha = (unsigned char)(long)(colour[3] * t);
        r.width = t * highlightWidth;
    }
    if (++e.CurTime > e.FadeOutTimeStart + e.FadeOutTime) r.removed = true;
    return r;
}

int main()
{
    long colour[4] = { 100, 0, 200, 255 };
    Effect e = { 0, 10, 20, 10 };
    Result r0 = FrameUpdate(e, colour, 2.0f);
    if (!r0.applied || r0.tint.R != 255 || r0.tint.G != 255 || r0.tint.B != 255 || r0.highlightAlpha != 0 || r0.width != 0.0f) {
        std::printf("007ce420_TEST FAIL\n"); return 1;
    }
    e.CurTime = 5;
    Result r1 = FrameUpdate(e, colour, 2.0f);
    if (r1.tint.R != 177 || r1.tint.G != 127 || r1.tint.B != 227 || r1.tint.A != 254 || r1.highlightAlpha != 127 || r1.width != 1.0f) {
        std::printf("007ce420_TEST FAIL\n"); return 1;
    }
    e.CurTime = 15;
    Result r2 = FrameUpdate(e, colour, 2.0f);
    if (r2.applied || r2.removed) { std::printf("007ce420_TEST FAIL\n"); return 1; }
    e.CurTime = 30;
    Result r3 = FrameUpdate(e, colour, 2.0f);
    if (!r3.applied || r3.tint.R != 255 || !r3.removed) { std::printf("007ce420_TEST FAIL\n"); return 1; }
    std::printf("007ce420_TEST PASS\n");
    return 0;
}