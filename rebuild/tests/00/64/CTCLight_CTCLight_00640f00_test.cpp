#include <cstdio>
// Behaviour model of CTCLight::CTCLight (0x00640F00): cleared state, flicker factors 1.0, registered with the
// appearance and the light sub-def fetched.
struct Light {
    void* data; void* info; const void* def; bool active, overridden; float flicker, oldFlicker; bool initialised;
    void* v[6]; int registered;
    Light(const void* subDef) : data(0), info(0), def(0), active(false), overridden(false), flicker(1.0f), oldFlicker(1.0f), initialised(false), registered(0)
    {
        for (int i = 0; i < 6; ++i) v[i] = 0;
        registered++;
        def = subDef;
    }
};
int main()
{
    int dummy = 0;
    Light l(&dummy);
    if (l.data || l.info || l.def != &dummy || l.active || l.overridden || l.flicker != 1.0f || l.oldFlicker != 1.0f || l.initialised || l.v[5] || l.registered != 1) { std::printf("00640f00_TEST FAIL\n"); return 1; }
    std::printf("00640f00_TEST PASS\n");
    return 0;
}