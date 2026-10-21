// Behaviour test for 0x007CEE40 CTCCreatureTintEffect::FrameUpdate: fade-in blends white toward the colour by
// CurTime/FadeInTime (a zero fade time snaps to the colour), fade-out blends back, dead/appearance-less things are
// skipped, the component is removed once the fade-out ends, and CurTime always advances.
#include <cstdio>

struct Colour { unsigned char B, G, R, A; };
static Colour g_last;
static int g_tints = 0, g_removed = 0;

struct Thing { bool hasAppearance; bool dead; };

struct TintEffect {
    long CurTime, FadeInTime;
    bool StartFadeOut;
    long FadeOutTimeStart, FadeOutTime;
    long Color[4];
    Thing* things; int count;
    void SetTint(Thing* parent, float amount)
    {
        float white = (float)(int)((1.0f - amount) * 255.0f);
        Colour c;
        c.R = (unsigned char)(int)(Color[0] * amount + white);
        c.G = (unsigned char)(int)(Color[1] * amount + white);
        c.B = (unsigned char)(int)(Color[2] * amount + white);
        c.A = (unsigned char)(int)(Color[3] * amount + white);
        if (parent->hasAppearance) { g_last = c; ++g_tints; }
        for (int i = 0; i < count; ++i)
            if (!things[i].dead && things[i].hasAppearance) { g_last = c; ++g_tints; }
    }
    void FrameUpdate(Thing* parent)
    {
        if (CurTime <= FadeInTime) {
            float amount = (float)CurTime / (float)FadeInTime;
            if (FadeInTime == 0)
                amount = 1.0f;
            SetTint(parent, amount);
        } else if (StartFadeOut) {
            float amount = 1.0f - (float)(CurTime - FadeOutTimeStart) / (float)FadeOutTime;
            if (FadeOutTime == 0)
                amount = 1.0f;
            SetTint(parent, amount);
            if (CurTime > FadeOutTimeStart + FadeOutTime)
                ++g_removed;
        }
        ++CurTime;
    }
};

int main()
{
    Thing parent = { true, false };
    Thing list[2] = { { true, true }, { true, false } };
    TintEffect e = { 0, 10, false, 0, 4, { 100, 0, 200, 255 }, list, 2 };
    e.FrameUpdate(&parent);                       // amount 0 -> white
    int ok = g_tints == 2 && g_last.R == 255 && g_last.G == 255 && g_last.B == 255 && e.CurTime == 1;
    e.CurTime = 10;
    e.FrameUpdate(&parent);                       // amount 1 -> the colour
    ok = ok && g_last.R == 100 && g_last.G == 0 && g_last.B == 200 && g_last.A == 255 && e.CurTime == 11;
    e.FrameUpdate(&parent);                       // past fade-in, no fade-out: nothing
    ok = ok && g_tints == 4 && e.CurTime == 12;
    e.StartFadeOut = true; e.FadeOutTimeStart = 12;
    e.FrameUpdate(&parent);                       // fade-out start: still the colour
    ok = ok && g_tints == 6 && g_last.R == 100 && g_removed == 0;
    e.CurTime = 17;
    e.FrameUpdate(&parent);                       // past the end: removed
    ok = ok && g_removed == 1 && e.CurTime == 18;
    if (!ok) {
        std::printf("007cee40_TEST FAIL\n");
        return 1;
    }
    std::printf("007cee40_TEST PASS\n");
    return 0;
}
