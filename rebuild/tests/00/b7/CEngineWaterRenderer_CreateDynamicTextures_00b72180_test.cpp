// Behaviour test for 0x00B72180 CEngineWaterRenderer::CreateDynamicTextures(bool, bool, bool) (self-contained host model; plain C++98).
// Models: the five bump-map source frames (water, sea, ice, water 2, sea 2) fetched in retail order with every GetFrame evaluated
// (`ok &=`, an unloaded frame starts loading and makes the call return false before any scratch texture is taken); the four scratch
// textures; the scoped shader enable (on during the work, previous state restored at scope end) and render-state capture block; the
// environment map pass only with reflection generation AND high-end water (rendered, detached, mipped); the per-level bump-map loops
// (level < GetNoLevels(), stop at the first level narrower or lower than 32, render target attached to the level surface, water ->
// RenderBumpMap(0), sea -> RenderBumpMap(1), ice -> RenderIceBumpMap with one source texture), and the tidy-up (back screen, both
// targets detached, camera transforms refreshed).
#include <cstdio>

struct Frame { bool Loaded; int LoadRequests; };
struct Texture { long Levels; unsigned long Width, Height; bool Mipped; };
struct Log {
    int Calls[64]; int Count; int Started; bool VsOn, PsOn; int Captures, Restores, BackScreen, CameraRefresh;
    void Add(int c) { Calls[Count++] = c; }
};
enum { ENV_MAP = 100, WATER_BUMP = 0, SEA_BUMP = 1, ICE_BUMP = 2 };

static bool GetFrame(Frame& f) { if (f.Loaded) return true; ++f.LoadRequests; return false; }

struct Water {
    Frame Frames[5];                    // water, sea, ice, water 2, sea 2 (retail fetch order)
    Texture EnvironmentMap, WaterBumpMap, SeaBumpMap, IceBumpMap;
    bool EnableReflectionGeneration, HighendWaterEnabled;
    Log L;
    void RenderLevels(Texture& t, int what)
    {
        long level = 0;
        while (level < t.Levels) {
            if ((t.Width >> level) < 32 || (t.Height >> level) < 32)
                break;
            L.Add(what * 10 + (int)level);
            ++level;
        }
    }
    bool CreateDynamicTextures(bool water, bool sea, bool ice)
    {
        bool ok = GetFrame(Frames[0]);
        ok &= GetFrame(Frames[1]);
        ok &= GetFrame(Frames[2]);
        ok &= GetFrame(Frames[3]);
        ok &= GetFrame(Frames[4]);
        if (!ok)
            return false;
        L.Started = 4;
        ++L.Captures;
        bool old_vs = L.VsOn, old_ps = L.PsOn;
        L.VsOn = L.PsOn = true;
        if (EnableReflectionGeneration && HighendWaterEnabled) { L.Add(ENV_MAP); EnvironmentMap.Mipped = true; }
        if (water) RenderLevels(WaterBumpMap, WATER_BUMP);
        if (sea) RenderLevels(SeaBumpMap, SEA_BUMP);
        if (ice) RenderLevels(IceBumpMap, ICE_BUMP);
        ++L.BackScreen; ++L.CameraRefresh;
        L.VsOn = old_vs; L.PsOn = old_ps;
        ++L.Restores;
        return true;
    }
};

static Water Make()
{
    Water w = Water();
    for (int i = 0; i < 5; ++i) w.Frames[i].Loaded = true;
    Texture big = { 4, 256, 128, false };          // levels 256x128, 128x64, 64x32, 32x16 -> 3 usable
    Texture small = { 3, 16, 16, false };          // nothing usable
    w.EnvironmentMap = big; w.WaterBumpMap = big; w.SeaBumpMap = big; w.IceBumpMap = small;
    return w;
}

int main()
{
    int fail = 0;
    // 1. ice frame (3rd) not loaded: false, every frame still asked (sea 2 after it too), no texture started, nothing rendered.
    Water a = Make(); a.Frames[2].Loaded = false;
    if (a.CreateDynamicTextures(true, true, true) || a.Frames[2].LoadRequests != 1 || a.L.Started != 0 || a.L.Count != 0 || a.L.Captures != 0) fail |= 1;
    Water a2 = Make(); a2.Frames[0].Loaded = false; a2.Frames[4].Loaded = false;
    if (a2.CreateDynamicTextures(true, false, false) || a2.Frames[0].LoadRequests != 1 || a2.Frames[4].LoadRequests != 1) fail |= 2;
    // 2. all loaded, reflections off: water levels 0..2 then sea levels 0..2; ice texture too small; shaders restored (were off).
    Water b = Make(); b.EnableReflectionGeneration = true; b.HighendWaterEnabled = false;
    if (!b.CreateDynamicTextures(true, true, true)) fail |= 4;
    int want_b[] = { 0, 1, 2, 10, 11, 12 };
    if (b.L.Count != 6) fail |= 8;
    for (int i = 0; i < 6 && i < b.L.Count; ++i) if (b.L.Calls[i] != want_b[i]) fail |= 16;
    if (b.EnvironmentMap.Mipped || b.L.VsOn || b.L.PsOn || b.L.Restores != 1 || b.L.BackScreen != 1 || b.L.CameraRefresh != 1) fail |= 32;
    // 3. reflections + high-end: environment map first and mipped; only the sea pass asked; shaders were on and stay on.
    Water c = Make(); c.EnableReflectionGeneration = true; c.HighendWaterEnabled = true; c.L.VsOn = true; c.L.PsOn = false;
    c.IceBumpMap.Width = 64; c.IceBumpMap.Height = 64;
    if (!c.CreateDynamicTextures(false, true, true)) fail |= 64;
    int want_c[] = { ENV_MAP, 10, 11, 12, 20, 21 };
    if (c.L.Count != 6) fail |= 128;
    for (int i = 0; i < 6 && i < c.L.Count; ++i) if (c.L.Calls[i] != want_c[i]) fail |= 256;
    if (!c.EnvironmentMap.Mipped || !c.L.VsOn || c.L.PsOn) fail |= 512;
    if (fail) { std::printf("00b72180_TEST FAIL\n"); return 1; }
    std::printf("00b72180_TEST PASS\n");
    return 0;
}