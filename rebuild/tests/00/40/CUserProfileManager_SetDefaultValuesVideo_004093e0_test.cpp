// Behaviour test for CUserProfileManager::SetDefaultValuesVideo (0x004093E0): the function's logic over a small copy of the model
// (defaults def or none, detection overrides, display modes, display refresh rate, the caps) with recording stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <vector>

struct Def { unsigned long Antialiasing; long ResolutionWidth, ResolutionHeight, BitDepth;
             float TextureDetail, MaxTextureDetail, ShadowDetail, MaxShadowDetail, MeshDetail, MaxMeshDetail, EffectsDetail, MaxEffectsDetail; };
struct Mode { long X, Y, Bpp; };
static long g_MaxDetailA = -1, g_MaxDetailB = -1, g_MaxDetailC = -1, g_MaxDetailD = -1, g_DetailA = 1, g_DetailB = 2, g_DetailC = 3, g_DetailD = 4;
static long g_MaxAntialiasing = 4, g_MaxResolutionWidth = -1, g_MaxRefreshRate = -1, g_MemoryA = 0, g_MemoryB = 0;
static const long g_refreshRateOptions[4] = { 60, 75, 85, 100 };   // retail table at 0x0122DE30 (Fable.exe bytes)
static long g_depth = 32, g_rtX = 1024, g_rtY = 768, g_refresh = 78;
static int g_applied = 0;

struct Profile {
    float Gamma, EffectsDetail, TextureDetail, MeshDetail, ShadowDetail; unsigned long Antialiasing, Resolution, RefreshRate;
    long ScreenAspectRatio; bool VerticalSyncEnabled; std::vector<Mode> DisplayModes; const Def* def;
    void ClampVideoOption(float& v, float n, float d, float m, float dm) { v = n * 100.0f + d + m * 0.001f + dm * 0.0001f; }
    void ClampAntialiasing(unsigned long& a, long max) { if ((long)a > max) a = max; }
    void ClampResolution(unsigned long& r, long maxW) {
        if (r >= DisplayModes.size()) r = 0;
        if (maxW != -1 && DisplayModes[r].X > maxW) { unsigned long i = r; while (i >= 0 && DisplayModes[i].X > maxW) --i; r = i; }
    }
    void ClampRefreshRate(unsigned long& r, long maxR) {
        if (r >= 4) r = 0;
        if (maxR != -1 && g_refreshRateOptions[r] > maxR) { unsigned long i = r; while (i >= 0 && g_refreshRateOptions[i] > maxR) --i; r = i; }
    }
    void SetDefaultValuesVideo() {                     // same statements as the row
        Gamma = 1.0f;
        if (!def) {
            ClampVideoOption(EffectsDetail, (float)g_DetailA, -1.0f, (float)g_MaxDetailA, -1.0f);
            ClampVideoOption(TextureDetail, (float)g_DetailD, -1.0f, (float)g_MaxDetailD, -1.0f);
            ClampVideoOption(MeshDetail, (float)g_DetailC, -1.0f, (float)g_MaxDetailC, -1.0f);
            ClampVideoOption(ShadowDetail, (float)g_DetailB, -1.0f, (float)g_MaxDetailB, -1.0f);
        } else {
            ClampVideoOption(EffectsDetail, (float)g_DetailA, def->EffectsDetail, (float)g_MaxDetailA, def->MaxEffectsDetail);
            ClampVideoOption(TextureDetail, (float)g_DetailD, def->TextureDetail, (float)g_MaxDetailD, def->MaxTextureDetail);
            ClampVideoOption(MeshDetail, (float)g_DetailC, def->MeshDetail, (float)g_MaxDetailC, def->MaxMeshDetail);
            ClampVideoOption(ShadowDetail, (float)g_DetailB, def->ShadowDetail, (float)g_MaxDetailB, def->MaxShadowDetail);
            Antialiasing = def->Antialiasing;
        }
        if (!def) Antialiasing = 0;
        ClampAntialiasing(Antialiasing, g_MaxAntialiasing);
        long width = -1, height = -1, depth = g_depth;
        if (g_MemoryA > 0) width = g_MemoryA;
        if (g_MemoryB > 0) height = g_MemoryB;
        if (def) { if (width == -1) width = def->ResolutionWidth; if (height == -1) height = def->ResolutionHeight; depth = def->BitDepth; }
        if (width == -1) width = g_rtX;
        if (height == -1) height = g_rtY;
        Resolution = 0;
        for (std::vector<Mode>::const_iterator it = DisplayModes.begin(); it != DisplayModes.end(); ++it) {
            if (it->X == width && it->Y == height && it->Bpp == depth) break;
            ++Resolution;
        }
        ClampResolution(Resolution, g_MaxResolutionWidth);
        long refreshRate = g_refresh;
        RefreshRate = 0;
        unsigned long i = 1;
        do {
            if (abs(refreshRate - g_refreshRateOptions[i]) < abs(refreshRate - g_refreshRateOptions[RefreshRate]) && g_refreshRateOptions[i] <= refreshRate)
                RefreshRate = i;
        } while (++i < 4);
        ClampRefreshRate(RefreshRate, g_MaxRefreshRate);
        ScreenAspectRatio = 0; VerticalSyncEnabled = true; ++g_applied;
    }
};

int main() {
    int bad = 0;
    Profile p; p.Antialiasing = 9; p.Resolution = 7; p.RefreshRate = 3; p.ScreenAspectRatio = 2; p.VerticalSyncEnabled = false; p.def = 0;
    Mode m[3] = { { 800, 600, 32 }, { 1024, 768, 32 }, { 1280, 1024, 32 } };
    p.DisplayModes.assign(m, m + 3);
    p.SetDefaultValuesVideo();                                       // no def: AA 0, render target 1024x768 -> mode 1, 78 Hz -> 75 (index 1)
    if (p.Antialiasing != 0 || p.Resolution != 1 || p.RefreshRate != 1 || p.Gamma != 1.0f || !p.VerticalSyncEnabled || p.ScreenAspectRatio != 0) bad |= 1;
    if (p.EffectsDetail != 100.0f - 1.0f - 0.001f - 0.0001f) bad |= 2;
    Def d = { 6, 1280, 1024, 32, 0.5f, 1, 0.25f, 1, 0.75f, 1, 0.125f, 1 };
    p.def = &d; g_refresh = 90; g_MaxResolutionWidth = 1024; g_MaxRefreshRate = 75;
    p.SetDefaultValuesVideo();                                       // def: AA 6 clamped to 4, mode 2 clamped to width 1024 -> 1, 90 Hz picks 85 (index 2), capped at 75 -> index 1
    if (p.Antialiasing != 4 || p.Resolution != 1 || p.RefreshRate != 1) bad |= 4;
    g_MemoryA = 1280; g_MemoryB = 1024; g_MaxResolutionWidth = -1; g_MaxRefreshRate = -1; g_refresh = 120;
    d.ResolutionWidth = 640; d.ResolutionHeight = 480;
    p.SetDefaultValuesVideo();                                       // detection overrides beat the 640x480 def -> mode 2 (not the
    if (p.Resolution != 2) bad |= 8;                                 //   def's missing mode, which clamps to 0); uncapped 120 Hz -> 100
    if (p.RefreshRate != 3) bad |= 256;                              //   (index 3, the fourth refresh-table entry)
    g_MemoryA = 0; g_MemoryB = 0;
    p.SetDefaultValuesVideo();                                       // no such mode: index runs off the end -> clamped to 0
    if (p.Resolution != 0 || g_applied != 4) bad |= 16;
    Mode m2[4] = { { 800, 600, 32 }, { 1024, 768, 16 }, { 1024, 768, 32 }, { 1280, 1024, 32 } };
    p.DisplayModes.assign(m2, m2 + 4); g_MaxRefreshRate = -1; g_refresh = 84;
    d.ResolutionWidth = 1024; d.ResolutionHeight = 768; d.BitDepth = 16;
    p.SetDefaultValuesVideo();                                       // def depth 16 beats the display's 32 -> mode 1; 85 Hz is nearer 84 but above it -> 75 (index 1)
    if (p.Resolution != 1) bad |= 32;
    if (p.RefreshRate != 1) bad |= 64;
    p.def = 0;
    p.SetDefaultValuesVideo();                                       // no def: display depth 32 -> mode 2
    if (p.Resolution != 2 || g_applied != 6) bad |= 128;
    if (bad) { printf("004093e0_TEST FAIL %d\n", bad); return 1; }
    printf("004093e0_TEST PASS\n");
    return 0;
}