// behaviour test for 0x008901C0 CGameScriptInterface::DisplacementMonochromeEffectColourFadeTo
#include <stdio.h>
class CRGBFloatColour { public: float R, G, B, A; };

namespace NScreenEffect { enum ScreenEffectPriority { SCREEN_EFFECT_PRIORITY_SCRIPT = 1, SCREEN_EFFECT_PRIORITY_FORCE_DWORD = 0x7fffffff }; }
class CEnvironmentSpellEffectInterpolator {
public:
    void* DisplacementMonochromeEffectColourFadeTo(float time, NScreenEffect::ScreenEffectPriority priority, const CRGBFloatColour& colour);   // 0x006B8370
};
class CEnvironment {
public:
    unsigned char                        _pad00[0x1964];
    CEnvironmentSpellEffectInterpolator* SpellEffectInterpolator;   // +0x1964
    CEnvironmentSpellEffectInterpolator* GetSpellEffectInterpolator() const { return SpellEffectInterpolator; }
};
class CWorld {
public:
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                          // +0x18
    CEnvironment* GetEnvironment() const { return PEnvironment; }
};
class CGameScriptInterface {
public:
    virtual void* DisplacementMonochromeEffectColourFadeTo(float time, const CRGBFloatColour& colour) const;
    CWorld* World;                                       // +0x04
    CWorld* GetWorld() const { return World; }
};

void* CGameScriptInterface::DisplacementMonochromeEffectColourFadeTo(float time, const CRGBFloatColour& colour) const
{
    return GetWorld()->GetEnvironment()->GetSpellEffectInterpolator()->DisplacementMonochromeEffectColourFadeTo(time, NScreenEffect::SCREEN_EFFECT_PRIORITY_SCRIPT, colour);
}
static float g_time; static int g_prio; static const CRGBFloatColour* g_col; static int g_handle;
void* CEnvironmentSpellEffectInterpolator::DisplacementMonochromeEffectColourFadeTo(float time, NScreenEffect::ScreenEffectPriority priority, const CRGBFloatColour& colour)
{ g_time = time; g_prio = priority; g_col = &colour; return &g_handle; }
static unsigned char g_envmem[0x2000];
int main()
{
    CEnvironmentSpellEffectInterpolator interp;
    CEnvironment* env = (CEnvironment*)g_envmem; env->SpellEffectInterpolator = &interp;
    CWorld world; world.PEnvironment = env;
    CGameScriptInterface gsi; gsi.World = &world; const CGameScriptInterface* p = &gsi;
    CRGBFloatColour c = { 1.0f, 0.5f, 0.25f, 1.0f };
    void* h = p->DisplacementMonochromeEffectColourFadeTo(2.5f, c);
    int ok = h == &g_handle && g_time == 2.5f && g_prio == 1 && g_col == &c;
    printf(ok ? "008901c0_TEST PASS\n" : "008901c0_TEST FAIL\n");
    return ok ? 0 : 1;
}