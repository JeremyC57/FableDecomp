// Behaviour test for CTCSpotLight::FrameUpdate (0x00731BF0): the old factor is saved; below the flicker threshold nothing
// moves; otherwise the factor random-walks by (r - 0.5) * speed * flicker and is clamped to [1 - flicker, 1].
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
struct CSpotLightInfo { float Angle; float pad[3]; float Flicker; };
struct CSpotLightDef { float pad[14]; float Flicker; float FlickerSpeed; };
struct CTCSpotLight {                                   // the engine header's members by name
    void* __vftable;
    CSpotLightInfo* PLightInfo_Data;
    void* PLightInfo_Info;
    void* PLightDef_Object;
    bool Overridden;
    float AccumilativeFlickerFactor;
    float OldAccumilativeFlickerFactor;
};
static float g_random = 0.0f; static unsigned long g_seed = 0; static unsigned long* g_seedPassed = 0; static float g_range = 0.0f;
unsigned long* __fastcall GetPLocalSeed(CTCSpotLight* light) { return &g_seed; }
float __fastcall GFFloatRandom(float range, unsigned long* seed) { g_range = range; g_seedPassed = seed; return g_random; }

void __fastcall FrameUpdate(CTCSpotLight* light)
{
    light->OldAccumilativeFlickerFactor = light->AccumilativeFlickerFactor;
    CSpotLightDef* def = (CSpotLightDef*)light->PLightDef_Object;
    float flicker = (light->Overridden ? light->PLightInfo_Data->Flicker : def->Flicker) * 0.1;
    if (flicker > 0.00390625f) {
        float speed = ((CSpotLightDef*)light->PLightDef_Object)->FlickerSpeed;
        light->AccumilativeFlickerFactor += (GFFloatRandom(1.0f, GetPLocalSeed(light)) - 0.5f) * speed * flicker;
        float low = 1.0f - flicker;
        if (light->AccumilativeFlickerFactor < low)
            light->AccumilativeFlickerFactor = low;
        else if (light->AccumilativeFlickerFactor > 1.0f)
            light->AccumilativeFlickerFactor = 1.0f;
    }
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    int bad = 0;
    CSpotLightDef def; def.Flicker = 2.0f; def.FlickerSpeed = 3.0f;
    CSpotLightInfo info; info.Flicker = 0.01f;
    CTCSpotLight s; s.PLightDef_Object = &def; s.PLightInfo_Data = &info; s.Overridden = false;
    s.AccumilativeFlickerFactor = 0.9f; s.OldAccumilativeFlickerFactor = 0.0f;
    g_random = 0.6f;                                  // +0.1 * 3 * 0.2 = +0.06
    FrameUpdate(&s);
    if (!Near(s.OldAccumilativeFlickerFactor, 0.9f) || !Near(s.AccumilativeFlickerFactor, 0.96f)) bad |= 1;
    if (g_range != 1.0f || g_seedPassed != &g_seed) bad |= 2;
    g_random = 1.0f; FrameUpdate(&s);                 // +0.5*3*0.2 = +0.3 -> clamp to 1
    if (s.AccumilativeFlickerFactor != 1.0f) bad |= 4;
    s.AccumilativeFlickerFactor = 0.81f; g_random = 0.0f; FrameUpdate(&s);   // -0.3 -> 0.51 < 0.8 -> 0.8
    if (!Near(s.AccumilativeFlickerFactor, 0.8f)) bad |= 8;
    s.Overridden = true; s.AccumilativeFlickerFactor = 0.5f; FrameUpdate(&s);   // flicker 0.001 below the threshold
    if (s.AccumilativeFlickerFactor != 0.5f || s.OldAccumilativeFlickerFactor != 0.5f) bad |= 16;
    if (bad) { std::printf("00731bf0_TEST FAIL\n"); return 1; }
    std::printf("00731bf0_TEST PASS\n");
    return 0;
}