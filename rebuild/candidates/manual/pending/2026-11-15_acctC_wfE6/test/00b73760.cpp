// Behaviour test for 0x00B73760 CEngineWaterRenderer::CEngineWaterRenderer(const CEngineInit&): a host model of the row with recording
// stand-ins for the shader lookups (bank number + name per vertex shader, name per pixel shader), the pixel-shader-version query and the
// two trailing calls. Checks: init flag bit 0x80 decides HighendWaterSupported / HighendWaterEnabled (both set or both clear, bit 0x7F
// ignored); the three ice CArrays get four shaders each in the base, _1, _2, _3 LIGHTS order, the COLBUFF array and the COLBUFF pixel
// shader only when the pixel shader version is >= 1400 (1399 skips, 1400 takes); the bank numbers 7 / 8 / 9 / 13 per lookup; the trailing
// member clears and WaterBumpFactor = 1.5f; InitialiseDynamicTextures runs before BuildConstantBuffers, after every lookup.
#include <cstdio>
#include <cstring>
struct Log { int n; int bank[64]; char name[64][48]; int calls[4]; int ncalls; };
static int V(Log& g, int bank, const char* s) { g.bank[g.n] = bank; strcpy(g.name[g.n], s); return ++g.n; }
static int P(Log& g, const char* s) { return V(g, -1, s); }
struct Arr { int v[8]; int n; void push_back(int x) { v[n++] = x; } };
struct R {
    bool HighendWaterSupported, HighendWaterEnabled, HaveBackgroundSea;
    int ForegroundVS, BackgroundVS, IceBackgroundVS, SeaBackgroundVS, SeaReflVS, SeaNonReflVS, BumpVS, IceBumpVS, RainVS, EnvVS, SkyVS;
    int ForegroundPS, BackgroundPS, IceForegroundPS, IceShadowedPS, RefractionPS, ColbuffPS, SeaLowVS, WaterLowVS;
    int LowPS, IceBackgroundPS, SeaBackgroundPS, SeaReflPS, SeaNonReflPS, BumpPS, IceBumpPS, RainPS, EnvPS, SkyPS;
    Arr Ice, IceShadowed, IceColbuff;
    long DynamicTexturePageHandle; void* EnvironmentMap; void* SeaBumpMap; void* WaterBumpMap; void* IceBumpMap;
    void* WaterGenerator; void* StaticMapBankFile; float WaterBumpFactor;
};
static void Row(R& r, Log& g, long init_flags, unsigned long ps_version)
{
    static const char* suf[4] = { "", "_1_LIGHTS", "_2_LIGHTS", "_3_LIGHTS" };
    char b[64];
    if (init_flags & 0x80) { r.HighendWaterSupported = true; r.HighendWaterEnabled = true; }
    else { r.HighendWaterSupported = false; r.HighendWaterEnabled = false; }
    r.ForegroundVS = V(g, 7, "VSHADER_WATER_FOREGROUND");
    r.BackgroundVS = V(g, 8, "VSHADER_WATER_BACKGROUND");
    for (int i = 0; i < 4; ++i) { sprintf(b, "VSHADER_ICE_FOREGROUND%s", suf[i]); r.Ice.push_back(V(g, 7, b)); }
    for (int i = 0; i < 4; ++i) { sprintf(b, "VSHADER_ICE_FOREGROUND_SHADOWED%s", suf[i]); r.IceShadowed.push_back(V(g, 7, b)); }
    if (ps_version >= 1400)
        for (int i = 0; i < 4; ++i) { sprintf(b, "VSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED%s", suf[i]); r.IceColbuff.push_back(V(g, 7, b)); }
    r.IceBackgroundVS = V(g, 8, "VSHADER_ICE_BACKGROUND");
    r.SeaBackgroundVS = V(g, 9, "VSHADER_SEA_BACKGROUND");
    r.SeaReflVS = V(g, 7, "VSHADER_SEA_FOREGROUND_REFLECTIVE");
    r.SeaNonReflVS = V(g, 7, "VSHADER_SEA_FOREGROUND_NON_REFLECTIVE");
    r.BumpVS = V(g, 13, "VSHADER_WATER_BUMPMAP"); r.IceBumpVS = V(g, 13, "VSHADER_ICE_BUMPMAP"); r.RainVS = V(g, 13, "VSHADER_WATER_BUMPMAP_RAIN");
    r.EnvVS = V(g, 13, "VSHADER_WATER_ENVIRONMENT_MAP"); r.SkyVS = V(g, 13, "VSHADER_WATER_SKY_MAP");
    r.ForegroundPS = P(g, "PSHADER_WATER_FOREGROUND"); r.BackgroundPS = P(g, "PSHADER_WATER_BACKGROUND");
    r.IceForegroundPS = P(g, "PSHADER_ICE_FOREGROUND"); r.IceShadowedPS = P(g, "PSHADER_ICE_FOREGROUND_SHADOWED");
    r.RefractionPS = P(g, "PSHADER_WATER_REFRACTION_FOREGROUND");
    if (ps_version >= 1400) r.ColbuffPS = P(g, "PSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED");
    r.SeaLowVS = V(g, 9, "VSHADER_SEA_BACKGROUND_LOW_END"); r.WaterLowVS = V(g, 8, "VSHADER_WATER_BACKGROUND_LOW_END");
    r.LowPS = P(g, "PSHADER_WATER_LOW_END"); r.IceBackgroundPS = P(g, "PSHADER_ICE_BACKGROUND"); r.SeaBackgroundPS = P(g, "PSHADER_SEA_BACKGROUND");
    r.SeaReflPS = P(g, "PSHADER_SEA_FOREGROUND_REFLECTIVE"); r.SeaNonReflPS = P(g, "PSHADER_SEA_FOREGROUND_NON_REFLECTIVE");
    r.BumpPS = P(g, "PSHADER_WATER_BUMPMAP"); r.IceBumpPS = P(g, "PSHADER_ICE_BUMPMAP"); r.RainPS = P(g, "PSHADER_WATER_BUMPMAP_RAIN");
    r.EnvPS = P(g, "PSHADER_WATER_ENVIRONMENT_MAP"); r.SkyPS = P(g, "PSHADER_WATER_SKY_MAP");
    r.DynamicTexturePageHandle = 0; r.EnvironmentMap = 0; r.SeaBumpMap = 0; r.WaterBumpMap = 0; r.IceBumpMap = 0;
    r.HaveBackgroundSea = false; r.WaterGenerator = 0; r.StaticMapBankFile = 0; r.WaterBumpFactor = 1.5f;
    g.calls[g.ncalls++] = 1;   // InitialiseDynamicTextures
    g.calls[g.ncalls++] = 2;   // BuildConstantBuffers
}
static void Junk(R& r) { memset(&r, 0x5a, sizeof r); r.Ice.n = r.IceShadowed.n = r.IceColbuff.n = 0; }
int main()
{
    bool ok = true;
    {
        R r; Junk(r); Log g; memset(&g, 0, sizeof g);
        Row(r, g, 0x80, 1400);
        ok = ok && r.HighendWaterSupported && r.HighendWaterEnabled && r.Ice.n == 4 && r.IceShadowed.n == 4 && r.IceColbuff.n == 4;
        ok = ok && g.n == 41 && r.ColbuffPS == 29 && strcmp(g.name[15], "VSHADER_SEA_BACKGROUND") == 0 && g.bank[15] == 9;
        ok = ok && strcmp(g.name[5], "VSHADER_ICE_FOREGROUND_3_LIGHTS") == 0 && g.bank[19] == 13 && g.bank[24] == -1;
        ok = ok && r.WaterBumpFactor == 1.5f && r.DynamicTexturePageHandle == 0 && r.EnvironmentMap == 0 && r.IceBumpMap == 0
                && !r.HaveBackgroundSea && r.WaterGenerator == 0 && r.StaticMapBankFile == 0;
        ok = ok && g.ncalls == 2 && g.calls[0] == 1 && g.calls[1] == 2;
    }
    {
        R r; Junk(r); Log g; memset(&g, 0, sizeof g);
        Row(r, g, 0x7f, 1399);
        ok = ok && !r.HighendWaterSupported && !r.HighendWaterEnabled && r.IceColbuff.n == 0 && g.n == 36;
        ok = ok && strcmp(g.name[10], "VSHADER_ICE_BACKGROUND") == 0 && strcmp(g.name[24], "VSHADER_SEA_BACKGROUND_LOW_END") == 0;
    }
    printf(ok ? "00b73760_TEST PASS\n" : "00b73760_TEST FAIL\n");
    return ok ? 0 : 1;
}
