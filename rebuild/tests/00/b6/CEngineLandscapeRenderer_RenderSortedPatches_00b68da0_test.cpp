// Behaviour test for CEngineLandscapeRenderer::RenderSortedPatches (0x00B68DA0): a copy of the model's logic over plain structs.
// The shader enables, the capture block (an RAII scope whose destructor restores it) and the lighting manager are stubs that
// append to an event log, so the restore order (shaders, then capture block) is observed rather than assumed. The faded-shadow
// step is the PDB inline SetEnableFadedShadowFactor(true) with its PeekGlobalShadowMode() != 1 guard (out-of-line copy 0x00B66EB0).
#include <cstdio>
#include <cstring>
#include <vector>

struct Patch { long ZSorterHandle; int renders; };
struct Entry { Patch* P; long AddedThisFrame : 1; long IsOcclusionBox : 1; long OcclusionID : 30; };

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); }

struct ShaderManager {
    bool vs, ps;
    void SetVertexShadersEnable(bool on, bool* old) { if (old) *old = vs; vs = on; Log(on ? "V+" : "V-"); }
    void SetPixelShadersEnable(bool on, bool* old) { if (old) *old = ps; ps = on; Log(on ? "P+" : "P-"); }
};
struct LightingManager {
    long ShadowMode;
    bool FadedShadowDiffuseFactorEnabled;
    unsigned long StateUpdateFlags;
    long PeekGlobalShadowMode() const { return ShadowMode; }
    void SetEnableFadedShadowFactor(bool enable)
    {
        if (PeekGlobalShadowMode() != 1)
            enable = false;
        if (enable != FadedShadowDiffuseFactorEnabled) {
            FadedShadowDiffuseFactorEnabled = enable;
            StateUpdateFlags |= 1;
        }
    }
    void UpdateGlobalLightingStates() { Log("U"); }
};
struct ScopedCaptureBlock {
    ScopedCaptureBlock() { Log("C+"); }
    ~ScopedCaptureBlock() { Log("C-"); }
};

static ShaderManager g_shaders = {false, false};
static LightingManager g_light = {1, false, 0};

static void Render(Patch* p, bool, long& q, bool& occ) { p->renders++; q = q + 10; occ = !occ; Log("R"); }

static void RenderSortedPatches(std::vector<Entry>& list)
{
    bool oldVS, oldPS;
    g_shaders.SetVertexShadersEnable(true, &oldVS);
    g_shaders.SetPixelShadersEnable(true, &oldPS);
    ScopedCaptureBlock capture;
    long count = 0;
    for (long i = 0; i < (long)list.size(); ++i) {
        Entry* entries = &list[0];
        Entry& entry = entries[i];
        if (entry.AddedThisFrame) {
            if (i > count) {
                entries[count].P = entry.P;
                entries[count].OcclusionID = entry.OcclusionID;
                entries[count].AddedThisFrame = 0;
                entries[count].P->ZSorterHandle = count;
            } else {
                entry.AddedThisFrame = 0;
            }
            ++count;
        } else if (entry.P) {
            entry.P->ZSorterHandle = -1;
        }
    }
    list.resize(count);
    Log("S");
    g_light.SetEnableFadedShadowFactor(true);
    g_light.UpdateGlobalLightingStates();
    for (unsigned long j = 0; j < list.size(); ++j) {
        Entry& e = list[j];
        long q = e.OcclusionID;
        bool occ = e.IsOcclusionBox != 0;
        Render(e.P, true, q, occ);
        e.OcclusionID = q;
        e.IsOcclusionBox = occ;
    }
    Log("X");
    g_shaders.SetVertexShadersEnable(oldVS, 0);
    g_shaders.SetPixelShadersEnable(oldPS, 0);
}

static int Fail() { std::printf("00b68da0_TEST FAIL\n"); return 1; }

int main()
{
    // Frame 1: a (kept 0) b c (added, moved down: i > count) d (not added).
    Patch a = {5, 0}, b = {6, 0}, c = {7, 0}, d = {8, 0};
    std::vector<Entry> list(4);
    list[0].P = &a; list[0].AddedThisFrame = 0; list[0].IsOcclusionBox = 0; list[0].OcclusionID = 1;
    list[1].P = &b; list[1].AddedThisFrame = -1; list[1].IsOcclusionBox = -1; list[1].OcclusionID = 2;
    list[2].P = &c; list[2].AddedThisFrame = -1; list[2].IsOcclusionBox = 0; list[2].OcclusionID = 3;
    list[3].P = &d; list[3].AddedThisFrame = 0; list[3].IsOcclusionBox = 0; list[3].OcclusionID = 4;
    g_log[0] = 0;
    RenderSortedPatches(list);
    if (std::strcmp(g_log, "V+P+C+SURRXV-P-C-") != 0) return Fail();   // shaders restored before the capture block
    if (list.size() != 2 || list[0].P != &b || list[1].P != &c
        || a.ZSorterHandle != -1 || d.ZSorterHandle != -1 || b.ZSorterHandle != 0 || c.ZSorterHandle != 1
        || list[0].AddedThisFrame != 0 || list[1].AddedThisFrame != 0
        || list[0].OcclusionID != 12 || list[1].OcclusionID != 13
        || list[0].IsOcclusionBox == 0 || list[1].IsOcclusionBox != 0
        || b.renders != 1 || c.renders != 1 || a.renders != 0 || d.renders != 0
        || !g_light.FadedShadowDiffuseFactorEnabled || g_light.StateUpdateFlags != 1 || g_shaders.vs || g_shaders.ps)
        return Fail();

    // Frame 2: both entries re-added and already in place (i == count): AddedThisFrame cleared in place, handles untouched
    // (sentinels 40/41 prove the in-place branch does not rewrite ZSorterHandle). Same shadow mode: no flag update.
    list[0].AddedThisFrame = -1; list[1].AddedThisFrame = -1;
    b.ZSorterHandle = 40; c.ZSorterHandle = 41;
    g_light.StateUpdateFlags = 0;
    g_shaders.vs = true;                                  // old vertex-shader state restored to true
    g_log[0] = 0;
    RenderSortedPatches(list);
    if (std::strcmp(g_log, "V+P+C+SURRXV+P-C-") != 0) return Fail();
    if (list.size() != 2 || list[0].P != &b || list[1].P != &c || list[0].AddedThisFrame != 0 || list[1].AddedThisFrame != 0
        || b.ZSorterHandle != 40 || c.ZSorterHandle != 41 || list[0].OcclusionID != 22 || list[1].OcclusionID != 23
        || g_light.StateUpdateFlags != 0 || !g_shaders.vs || g_shaders.ps)
        return Fail();

    // Frame 3: slot 0 kept in place (added), slot 1 dropped; shadow mode 2 forces the faded flag off despite `true`.
    list[0].AddedThisFrame = -1;
    g_light.ShadowMode = 2;
    g_shaders.vs = false;
    g_log[0] = 0;
    RenderSortedPatches(list);
    if (std::strcmp(g_log, "V+P+C+SURXV-P-C-") != 0) return Fail();
    if (list.size() != 1 || list[0].P != &b || list[0].AddedThisFrame != 0 || b.ZSorterHandle != 40 || c.ZSorterHandle != -1
        || g_light.FadedShadowDiffuseFactorEnabled || g_light.StateUpdateFlags != 1 || b.renders != 3 || c.renders != 2)
        return Fail();
    std::printf("00b68da0_TEST PASS\n");
    return 0;
}