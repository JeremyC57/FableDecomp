// Behaviour test for 0x0070BFC0 CAnimSet::SetAlpha (clamp to [0,1], roll current into last).
#include <cstdio>

struct CAnimSet {
    float CurrentAlpha; float LastAlpha;
    void SetAlpha(float alpha)
    {
        if (alpha < 0.0f) alpha = 0.0f;
        else if (alpha > 1.0f) alpha = 1.0f;
        LastAlpha = CurrentAlpha;
        CurrentAlpha = alpha;
    }
};

int main()
{
    CAnimSet s; s.CurrentAlpha = 0.5f; s.LastAlpha = 0.0f;
    s.SetAlpha(0.25f);
    bool ok = s.CurrentAlpha == 0.25f && s.LastAlpha == 0.5f;
    s.SetAlpha(-3.0f);
    ok = ok && s.CurrentAlpha == 0.0f && s.LastAlpha == 0.25f;
    s.SetAlpha(7.0f);
    ok = ok && s.CurrentAlpha == 1.0f && s.LastAlpha == 0.0f;
    s.SetAlpha(1.0f);
    ok = ok && s.CurrentAlpha == 1.0f && s.LastAlpha == 1.0f;
    if (!ok) { std::printf("0070bfc0_TEST FAIL\n"); return 1; }
    std::printf("0070bfc0_TEST PASS\n");
    return 0;
}