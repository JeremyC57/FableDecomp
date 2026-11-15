// Behaviour test for CEngineLandscapeRenderer::CEngineLandscapeRenderer (0x00B69000): a model of the constructor body -- the default
// landscape settings (mesh detail 2, procedural texture detail 3, fade 48..64, draw flags, LOD update time 0.004, 1.5 pixels per texel,
// 32 KB file blocks, static-map occlusion off), the fade factors computed from the fade range, the LOD Z bands (0, 0, fade end, then
// doubling), the foreground shader tables (6 entries each: base, 2, 2, 4, 4, 5 lights) with the three colour-buffer tables and the two
// colour-buffer pixel shaders only for pixel-shader version >= 1400, the shadowed-shader pointers aimed at the depth-buffer sets, and the
// memory manager created from the memory init.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct Settings { float MeshDetail, ProcDetail; bool DrawFg, DrawBg, DrawBgWater; float FadeStart, FadeEnd; float FgFade[4], BgFade[4];
                  bool EnableUpdate, Fog, StaticMapOcclusion, Bump; float LODTime, PixelsPerTexel; bool FgNoMap, Debug; long BlockSize; bool UpdateTess; };
struct Renderer {
    Settings S; float LODZ[8];
    std::vector<std::string> Fg, FgDepth, FgBumpDepth, FgSpotDepth, FgCol, FgBumpCol, FgSpotCol;
    std::string PsDepth, PsBumpDepth, PsCol, PsBumpCol, PsFg, PsBlackout;
    std::vector<std::string>* FgShadowed; std::vector<std::string>* FgBumpShadowed; std::vector<std::string>* FgSpotShadowed;
    std::string* PsShadowed; std::string* PsBumpShadowed; long MemoryInit;
};
static void CalcFade(Settings& s)
{
    s.BgFade[0] = 1.0f / (s.FadeEnd - s.FadeStart); s.BgFade[1] = s.BgFade[2] = 0.0f; s.BgFade[3] = s.FadeEnd / (s.FadeStart - s.FadeEnd) + 1.0f;
    s.FgFade[0] = 1.0f / (s.FadeStart - s.FadeEnd); s.FgFade[1] = s.FgFade[2] = 0.0f; s.FgFade[3] = s.FadeStart / (s.FadeEnd - s.FadeStart) + 1.0f;
}
static void Six(std::vector<std::string>& v, const char* suffix)
{
    static const char* n[6] = { "", "_2LIGHTS", "_2LIGHTS", "_4LIGHTS", "_4LIGHTS", "_5LIGHTS" };
    v.resize(6);
    for (int i = 0; i < 6; ++i) v[i] = std::string("VSHADER_LANDSCAPE_FOREGROUND") + n[i] + suffix;
}
static void Construct(Renderer& r, unsigned long ps_version, long memory_init)
{
    Settings& s = r.S;
    s.MeshDetail = 2.0f; s.ProcDetail = 3.0f; s.FadeStart = 48.0f; s.FadeEnd = 64.0f;
    CalcFade(s);
    s.DrawFg = s.DrawBg = s.DrawBgWater = true; s.EnableUpdate = true; s.Fog = true; s.Debug = false; s.BlockSize = 0x8000;
    s.LODTime = 0.004f; s.PixelsPerTexel = 1.5f; s.FgNoMap = true; s.Bump = true; s.UpdateTess = true; s.StaticMapOcclusion = false;
    r.LODZ[0] = 0.0f; r.LODZ[1] = 0.0f;
    float z = s.FadeEnd;
    for (long i = 2; i < 8; ++i) { r.LODZ[i] = z; z *= 2.0f; }
    Six(r.Fg, ""); Six(r.FgDepth, "_SHADOWED"); Six(r.FgBumpDepth, "_BUMP_SHADOWED"); Six(r.FgSpotDepth, "_SPOT_SHADOWED");
    if (ps_version >= 1400) { Six(r.FgCol, "_COLBUFF_SHADOWED"); Six(r.FgBumpCol, "_BUMP_COLBUFF_SHADOWED"); Six(r.FgSpotCol, "_SPOT_COLBUFF_SHADOWED"); }
    r.PsDepth = "PSHADER_LANDSCAPE_FOREGROUND_SHADOWED"; r.PsBumpDepth = "PSHADER_LANDSCAPE_FOREGROUND_BUMP_SHADOWED";
    if (ps_version >= 1400) { r.PsCol = "PSHADER_LANDSCAPE_FOREGROUND_COLBUFF_SHADOWED"; r.PsBumpCol = "PSHADER_LANDSCAPE_FOREGROUND_BUMP_COLBUFF_SHADOWED"; }
    r.PsFg = "PSHADER_LANDSCAPE_FOREGROUND"; r.PsBlackout = "PSHADER_DIFFUSE_ONLY";
    r.PsShadowed = &r.PsDepth; r.PsBumpShadowed = &r.PsBumpDepth;
    r.FgShadowed = &r.FgDepth; r.FgBumpShadowed = &r.FgBumpDepth; r.FgSpotShadowed = &r.FgSpotDepth;
    r.MemoryInit = memory_init;
}
static int g_fail = 0;
#define CHECK(x) do { if (!(x)) { std::printf("FAIL line %d: %s\n", __LINE__, #x); g_fail = 1; } } while (0)
int main()
{
    Renderer a; Construct(a, 1400, 7);
    CHECK(a.LODZ[0] == 0.0f && a.LODZ[1] == 0.0f && a.LODZ[2] == 64.0f && a.LODZ[3] == 128.0f && a.LODZ[7] == 2048.0f);
    CHECK(a.S.BgFade[0] == 1.0f / 16.0f && a.S.BgFade[3] == 64.0f / -16.0f + 1.0f && a.S.FgFade[0] == -1.0f / 16.0f && a.S.FgFade[3] == 48.0f / 16.0f + 1.0f);
    CHECK(a.S.MeshDetail == 2.0f && a.S.BlockSize == 0x8000 && !a.S.StaticMapOcclusion && a.S.LODTime == 0.004f);
    CHECK(a.Fg.size() == 6 && a.Fg[0] == "VSHADER_LANDSCAPE_FOREGROUND" && a.Fg[2] == "VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS" && a.Fg[5] == "VSHADER_LANDSCAPE_FOREGROUND_5LIGHTS");
    CHECK(a.FgSpotCol.size() == 6 && a.FgSpotCol[4] == "VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_SPOT_COLBUFF_SHADOWED" && a.PsBumpCol == "PSHADER_LANDSCAPE_FOREGROUND_BUMP_COLBUFF_SHADOWED");
    CHECK(a.FgShadowed == &a.FgDepth && a.FgSpotShadowed == &a.FgSpotDepth && a.PsBumpShadowed == &a.PsBumpDepth && a.MemoryInit == 7);
    Renderer b; Construct(b, 1399, 3);
    CHECK(b.FgCol.empty() && b.FgBumpCol.empty() && b.FgSpotCol.empty() && b.PsCol.empty() && b.PsBumpCol.empty());
    CHECK(b.FgBumpDepth[1] == "VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_BUMP_SHADOWED" && b.PsBlackout == "PSHADER_DIFFUSE_ONLY");
    if (!g_fail) std::printf("00b69000_TEST PASS\n");
    return g_fail;
}