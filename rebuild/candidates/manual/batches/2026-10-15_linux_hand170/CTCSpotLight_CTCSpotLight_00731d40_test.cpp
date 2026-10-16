#include <cstdio>
// Behaviour model of CTCSpotLight::CTCSpotLight (0x00731D40): the light info and def are cleared, the flicker factors start at
// 1.0, no primitive, not overridden, and the component registers itself with the owner's graphic appearance once.
struct SpotLight {
    void* infoData; void* infoInfo; void* def; bool overridden; void* primitive; float accum, oldAccum;
};
static int g_registered = 0;
static void Construct(SpotLight& s)
{
    s.infoData = 0; s.infoInfo = 0; s.def = 0;
    s.accum = s.oldAccum = 1.0f;
    s.primitive = 0;
    s.overridden = false;
    s.def = 0;
    g_registered++;
}
int main()
{
    SpotLight s; s.overridden = true; s.accum = 5.0f;
    Construct(s);
    if (s.infoData || s.def || s.primitive || s.overridden || s.accum != 1.0f || s.oldAccum != 1.0f || g_registered != 1) { std::printf("00731d40_TEST FAIL\n"); return 1; }
    std::printf("00731d40_TEST PASS\n");
    return 0;
}
