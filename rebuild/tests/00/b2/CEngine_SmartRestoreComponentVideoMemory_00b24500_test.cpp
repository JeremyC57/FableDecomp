// Behaviour test for 0x00B24500 CEngine::SmartRestoreComponentVideoMemory()
#include <cstdio>

static int g_released = 0, g_restored = 0;
static bool g_restoreFails = false;
static long g_shadowLimit = 3, g_effectsLimit = 3;     // the highest quality each renderer accepts
struct Def { long ShadowBufferSize; float ShadowDistanceScale; long Quality; };
struct Component {
    void OnReleaseDefaultPoolResources() { ++g_released; }
    bool OnRestoreDefaultPoolResources() { ++g_restored; return !g_restoreFails; }
};
struct ShadowRenderer { bool ChangeVideoOptionShadowQuality(long size, float) { return size <= g_shadowLimit; } };
struct EffectRenderer { bool SetEffectQuality(const Def& d) { return d.Quality <= g_effectsLimit; } };
static ShadowRenderer g_shadow; static EffectRenderer g_glow, g_blur;
static ShadowRenderer* g_pShadow = &g_shadow;
static EffectRenderer* g_pDisplacement = 0;              // absent renderers are skipped
static EffectRenderer* g_pGlow = &g_glow;
static EffectRenderer* g_pBlur = &g_blur;

static long Lower(long q) { switch (q) { case 1: return 0; case 3: return 2; case 2: return 1; } return 0; }
struct Engine {
    long ShadowQuality, EffectsQuality;
    Def Defs[4];
    Component Components[2];
    long SmartRestore(long& shadowOut, long& effectsOut)                 // the row's body (+ the qualities it ended on)
    {
        bool qualityLowered = false;
        long shadowQuality = ShadowQuality, effectsQuality = EffectsQuality;
        for (;;) {
            for (int i = 0; i < 2; ++i) Components[i].OnReleaseDefaultPoolResources();
            for (int i = 0; i < 2; ++i) if (!Components[i].OnRestoreDefaultPoolResources()) return 2;
            const Def& effectsDef = Defs[effectsQuality];
            const Def& shadowDef = Defs[shadowQuality];
            bool failed = false;
            failed |= g_pShadow && !g_pShadow->ChangeVideoOptionShadowQuality(shadowDef.ShadowBufferSize, shadowDef.ShadowDistanceScale);
            failed |= g_pDisplacement && !g_pDisplacement->SetEffectQuality(effectsDef);
            failed |= g_pGlow && !g_pGlow->SetEffectQuality(effectsDef);
            failed |= g_pBlur && !g_pBlur->SetEffectQuality(effectsDef);
            shadowOut = shadowQuality; effectsOut = effectsQuality;
            if (!failed) return qualityLowered ? 1 : 0;
            qualityLowered = true;
            if (shadowQuality > 1) shadowQuality = Lower(shadowQuality);
            else if (effectsQuality > 0) effectsQuality = Lower(effectsQuality);
            else if (shadowQuality > 0) shadowQuality = Lower(shadowQuality);
            else return 2;
        }
    }
};

int main()
{
    int ok = 1;
    Engine e;
    for (int q = 0; q < 4; ++q) { e.Defs[q].ShadowBufferSize = q; e.Defs[q].ShadowDistanceScale = 1.0f; e.Defs[q].Quality = q; }
    e.ShadowQuality = 3; e.EffectsQuality = 3;
    long s = -1, f = -1;
    ok &= e.SmartRestore(s, f) == 0 && s == 3 && f == 3 && g_released == 2 && g_restored == 2;
    g_shadowLimit = 1; g_effectsLimit = 1;                              // shadows first, down to 1
    ok &= e.SmartRestore(s, f) == 1 && s == 1 && f == 1;
    g_shadowLimit = 0;                                                  // then effects all the way, then shadows
    ok &= e.SmartRestore(s, f) == 1 && s == 0 && f == 0;
    g_shadowLimit = -1;                                                 // nothing left to lower
    ok &= e.SmartRestore(s, f) == 2 && s == 0 && f == 0;
    g_shadowLimit = 3; g_restoreFails = true;
    ok &= e.SmartRestore(s, f) == 2;
    if (ok) std::printf("00b24500_TEST PASS\n"); else std::printf("00b24500_TEST FAIL\n");
    return ok ? 0 : 1;
}