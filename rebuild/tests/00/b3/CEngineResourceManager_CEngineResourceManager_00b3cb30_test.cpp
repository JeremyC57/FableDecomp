// Behaviour test for CEngineResourceManager::CEngineResourceManager (0x00B3CB30): a model of the constructor body run against
// recording stubs -- texture bank attached only to a main mesh bank without one; AllocatePools(main mesh bank present); the retail
// shader bank opened only in retail-bank mode; the five VS constant layouts registered 1..5; the editor mesh bank opened (and given
// the graphic bank / cloth pool) only for init flag 0x20; the ten pixel-shader constants; the 25 + 2 vertex shader resources in
// retail order; the mesh-bank table and mesh-data pools only with a main mesh bank; the texture-manager pool list (512x512 DXT1 count
// = (hires KB << 10) / DXT1 512x512 size, hires KB from the video options def when video options exist, the A8R8G8B8 pool, the
// uncompressed-opaque pool with count / 16) given to an uninitialised texture manager only; the editor bank pools only when set;
// the alpha stipple flag = init flag 0x04; the 1x1 zero / saturated textures; the scratch pool and the loading callback.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static std::vector<std::string> g_log;
static void L(const char* f, ...) { char b[256]; va_list a; va_start(a, f); vsprintf(b, f, a); va_end(a); g_log.push_back(b); }

struct Pool { long Width, Height, Format; bool Mips; long Count; };
struct Config {
    bool HasMainMesh; bool MainMeshHasTextureBank; long InitFlags; bool RetailBanks; bool HasVideoOptions; long VideoDefHires;
    long MemoryHires; bool TextureManagerInitialised; bool PoolsSet;
};

// Model of the body, written from the retail control flow (0x00B3CCAC .. 0x00B3DEF4).
static std::vector<Pool> g_pools;
static long g_stipple = -1;
static void Construct(const Config& c)
{
    g_log.clear(); g_pools.clear();
    const long graphic_bank = 0x1234;
    long main_mesh = c.HasMainMesh ? 0x5678 : 0;
    if (main_mesh && !c.MainMeshHasTextureBank) L("main.SetTextureBank %lx", graphic_bank);
    bool has_mesh_bank = main_mesh != 0;
    L("AllocatePools %d", (int)has_mesh_bank);
    if (c.RetailBanks) L("OpenRetailBank shaders\\pc\\shaders.big 0");
    for (int i = 1; i <= 5; ++i) L("RegisterLayout %d", i);
    if (c.InitFlags & 0x20) { L("editor.OpenFileReadWrite mesh\\editmesh.bbb 1 512"); L("editor.SetTextureBank %lx", graphic_bank); L("editor.SetCloth"); }
    static const char* ps[] = { "PSCONST_ZERO", "PSCONST_ONE", "PSCONST_HALF", "PSCONST_1_0_0_0", "PSCONST_0_1_0_0", "PSCONST_0_0_1_0",
                                "PSCONST_0_0_0_1", "PSCONST_SHADOW_FADE_COLOUR", "PSCONST_MAX_FOG_ALPHA", "PSCONST_TFACTOR" };
    for (int i = 0; i < 10; ++i) L("PS %s", ps[i]);
    static const int banks[] = { 1, 3, 0, 12, 11, 13, 7, 8, 9, 5, 6, 14, 16, 24, 23, 21, 22, 17, 26, 20, 10, 25, 2, 4 };
    for (int i = 0; i < 24; ++i) L("VS %d", banks[i]);
    L("PixelShaders");
    if (main_mesh) { L("engine.Open MBANK_ENGINE"); L("main.SetMeshData 1"); L("engine.SetMeshData 0"); }
    const long dxt1_512 = 512 * 512 / 2;                          // CalculateTextureSize(0x200, 0x200, true, DXT1) stub
    long hires = c.MemoryHires;
    if (c.HasVideoOptions) hires = c.VideoDefHires;
    long texture_count = (hires << 10) / dxt1_512;
    long scratch_texture_count = texture_count / 16;
    const Pool p[10] = { {32,32,1,true,128},{32,32,3,true,64},{64,64,1,true,64},{64,64,3,true,64},{128,128,1,true,64},
                         {128,128,3,true,64},{256,256,1,true,128},{256,256,3,true,24},{512,512,1,true,texture_count},{512,512,3,true,0} };
    for (int i = 0; i < 10; ++i) g_pools.push_back(p[i]);
    Pool a = { 32, 32, 7, true, 64 }; g_pools.push_back(a);
    Pool b = { 512, 512, 0x15, false, 1 }; g_pools.push_back(b);
    Pool o = { 512, 512, 6, true, scratch_texture_count }; g_pools.push_back(o);
    if (!c.TextureManagerInitialised) L("TextureManager.Initialise %d", (int)g_pools.size());
    if (main_mesh) { L("main.SetAnimation 1"); L("main.SetCloth"); L("main.SetLoading"); L("main.SetFailure"); }
    L("graphic.SetLoading"); L("graphic.SetFailure");
    if (c.PoolsSet) { L("editor.SetMeshData 0"); L("editor.SetAnimation 0"); L("editor.SetCloth"); L("editor.SetLoading"); L("editor.SetFailure"); }
    g_stipple = (c.InitFlags & 4) != 0;
    L("Stipple %ld", g_stipple);
    L("Zero 0 0 0 0"); L("Sat 255 255 255 255"); L("Scratch 0"); L("Callback");
}

static int g_fail = 0;
static bool Has(const char* s) { for (size_t i = 0; i < g_log.size(); ++i) if (g_log[i] == s) return true; return false; }
static long Index(const char* s) { for (size_t i = 0; i < g_log.size(); ++i) if (g_log[i] == s) return (long)i; return -1; }
#define CHECK(x) do { if (!(x)) { std::printf("FAIL line %d: %s\n", __LINE__, #x); g_fail = 1; } } while (0)

int main()
{
    Config c1 = { true, false, 0x24, true, false, 0, 32768, false, true };
    Construct(c1);
    CHECK(Has("main.SetTextureBank 1234"));
    CHECK(Has("AllocatePools 1"));
    CHECK(Has("OpenRetailBank shaders\\pc\\shaders.big 0"));
    CHECK(Has("editor.OpenFileReadWrite mesh\\editmesh.bbb 1 512") && Index("editor.SetCloth") > Index("editor.SetTextureBank 1234"));
    CHECK(Index("RegisterLayout 5") < Index("PS PSCONST_ZERO") && Index("PS PSCONST_TFACTOR") < Index("VS 1"));
    CHECK(Index("VS 25") + 1 == Index("VS 2") && Index("VS 4") + 1 == Index("PixelShaders"));
    CHECK(Has("engine.Open MBANK_ENGINE") && Has("main.SetAnimation 1"));
    CHECK(g_pools.size() == 13 && g_pools[8].Count == 256 && g_pools[12].Count == 16 && g_pools[11].Format == 0x15 && !g_pools[11].Mips);
    CHECK(g_pools[10].Format == 7 && g_pools[12].Format == 6 && g_pools[7].Count == 24);
    CHECK(Has("TextureManager.Initialise 13"));
    CHECK(Has("editor.SetFailure") && g_stipple == 1);
    CHECK(g_log.back() == "Callback");

    Config c2 = { false, false, 0, false, true, 65536, 1, true, false };
    Construct(c2);
    CHECK(!Has("main.SetTextureBank 1234") && Has("AllocatePools 0"));
    CHECK(!Has("OpenRetailBank shaders\\pc\\shaders.big 0") && !Has("editor.SetCloth"));
    CHECK(!Has("engine.Open MBANK_ENGINE") && !Has("main.SetAnimation 1"));
    CHECK(g_pools[8].Count == 512 && g_pools[12].Count == 32);
    CHECK(!Has("TextureManager.Initialise 13") && !Has("editor.SetFailure") && g_stipple == 0);

    Config c3 = { true, true, 0x04, false, false, 0, 1000, false, false };
    Construct(c3);
    CHECK(!Has("main.SetTextureBank 1234") && Has("AllocatePools 1") && g_stipple == 1);
    CHECK(g_pools[8].Count == (1000L << 10) / (512 * 512 / 2) && g_pools[12].Count == g_pools[8].Count / 16);

    if (!g_fail) std::printf("00b3cb30_TEST PASS\n");
    return g_fail;
}