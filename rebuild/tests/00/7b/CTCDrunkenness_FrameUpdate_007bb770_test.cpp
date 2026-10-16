// behaviour test for 0x007BB770 CTCDrunkenness::FrameUpdate
#include <cstdio>
static long g_frame = 0;
struct Thing { bool scripted; bool IsUnderScriptSupervision() const { return scripted; } };
struct Drunkenness {
    Thing* ParentThing; bool Drunk; float D; long Last; float SecondsPerFrame; float Threshold;
    void FrameUpdate()
    {
        if (Last > 0) D -= (float)(g_frame - Last) * SecondsPerFrame * 0.005f;
        Last = g_frame;
        if (D < 0.0f) D = 0.0f;
        if (D > 0.0f && ParentThing->IsUnderScriptSupervision()) D = 0.0f;
        Drunk = D > Threshold;
    }
};
int main()
{
    Thing t = { false };
    Drunkenness d = { &t, false, 1.0f, 0, 0.1f, 0.5f };
    g_frame = 10; d.FrameUpdate();
    bool ok = d.Last == 10 && d.D == 1.0f && d.Drunk;
    g_frame = 1010; d.FrameUpdate();        // 1000 frames * 0.1 s * 0.005 = 0.5
    ok = ok && d.D > 0.49f && d.D < 0.51f && d.Last == 1010;
    g_frame = 3010; d.FrameUpdate();
    ok = ok && d.D == 0.0f && !d.Drunk;
    d.D = 2.0f; t.scripted = true; d.FrameUpdate();
    ok = ok && d.D == 0.0f && !d.Drunk;
    if (!ok) { std::printf("007bb770_TEST FAIL\n"); return 1; }
    std::printf("007bb770_TEST PASS\n");
    return 0;
}