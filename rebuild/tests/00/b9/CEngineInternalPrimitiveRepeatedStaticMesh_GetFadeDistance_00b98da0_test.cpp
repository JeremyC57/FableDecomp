#include <cstdio>
struct SView { unsigned char _pad[0x88]; float m_fadeDistanceScale; };
static SView g_view; static SView* g_pView = &g_view;
struct CEngineFadeDistance { float FadeBeginDistance, MaxDrawDistance; CEngineFadeDistance(float b, float m) : FadeBeginDistance(b), MaxDrawDistance(m) { } };
struct CMesh { unsigned char _pad[0x14]; float begin, max; CEngineFadeDistance GetFadeDistance() const { float scale = g_pView->m_fadeDistanceScale; CEngineFadeDistance d(begin * scale, max * scale); return d; } };
int main()
{
    g_view.m_fadeDistanceScale = 2.5f; CMesh m; m.begin = 4.0f; m.max = 10.0f; CEngineFadeDistance d = m.GetFadeDistance();
    if (d.FadeBeginDistance != 10.0f || d.MaxDrawDistance != 25.0f) { std::printf("00b98da0_TEST FAIL\n"); return 1; }
    std::printf("00b98da0_TEST PASS\n"); return 0;
}