#include <cstdio>
struct CEnginePrimitiveThemeChannelDesc { long m_channel; };
struct CTCGraphicAppearance {
    long channel; bool a, b; unsigned long changed; int sets;
    void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& c, bool x, bool y) { channel = c.m_channel; a = x; b = y; ++sets; }
    void SetAsChanged(unsigned long f) { changed |= f; }
};
struct CThing { CTCGraphicAppearance* m_pGraphicAppearance; };
struct CTCVolumeContainmentTracker {
    CEnginePrimitiveThemeChannelDesc CurrentThemeChannel; bool PreviouslyUpdated; int updates;
    void Update() { ++updates; PreviouslyUpdated = true; CurrentThemeChannel.m_channel = 9; }
    void OnThingCarried(CThing* carrier)
    {
        if (!PreviouslyUpdated) Update();
        if (carrier->m_pGraphicAppearance) {
            carrier->m_pGraphicAppearance->SetLightingChannel(CurrentThemeChannel, true, true);
            carrier->m_pGraphicAppearance->SetAsChanged(4);
        }
    }
};
int main()
{
    CTCGraphicAppearance g; g.channel = 0; g.a = g.b = false; g.changed = 0; g.sets = 0;
    CThing t; t.m_pGraphicAppearance = &g;
    CTCVolumeContainmentTracker v; v.PreviouslyUpdated = false; v.updates = 0; v.CurrentThemeChannel.m_channel = 1;
    v.OnThingCarried(&t);
    if (v.updates != 1 || g.channel != 9 || !g.a || !g.b || g.changed != 4 || g.sets != 1) { std::printf("00785c80_TEST FAIL\n"); return 1; }
    v.CurrentThemeChannel.m_channel = 5; v.OnThingCarried(&t);
    if (v.updates != 1 || g.channel != 5 || g.sets != 2) { std::printf("00785c80_TEST FAIL\n"); return 1; }
    CThing none; none.m_pGraphicAppearance = 0; v.OnThingCarried(&none);
    if (g.sets != 2) { std::printf("00785c80_TEST FAIL\n"); return 1; }
    std::printf("00785c80_TEST PASS\n");
    return 0;
}