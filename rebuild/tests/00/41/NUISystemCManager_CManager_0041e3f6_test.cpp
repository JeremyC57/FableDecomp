#include <cstdio>
#include <cstring>
// Behaviour test for the NUISystem::CManager constructor (0x0041E3F6): scalar members cleared, keyboard input selected, the texture
// formats picked into the bank parameters, the bank chosen by the front-end component flag, and the meta layer reset.
struct CBankParams { long F[7]; bool Flag1c; long Value20, Value24; bool Flag28; };
struct CDisplayManager { long Calls; long Pick(long bits, long* out) { Calls++; *out = bits * 10 + Calls; return 1; } };
struct CNewFrontendGameComponent { bool Active; };
static CDisplayManager g_dm = { 0 };
static CNewFrontendGameComponent* g_pFE = 0;
static const char* g_lastBank = 0;
static bool g_relative = false;
struct CManager {
    void* PInv[10]; bool HasError; void* Error; bool Restoring; double LastRender; void* Cursor; void* Dropped;
    float ScrollOffset; bool Flags[4]; long InputType; long MetaLayer; const char* Bank; CBankParams Used;
    void SetInput(long t) { InputType = t; }
    void SetMetaLayer(long l) { MetaLayer = l; }
    CManager() : HasError(false), Error(0), Restoring(false), LastRender(0.0), Cursor(0), Dropped(0), ScrollOffset(-1.0f)
    {
        int i;
        for (i = 0; i < 4; i++) Flags[i] = false;
        for (i = 0; i < 10; i++) PInv[i] = 0;
        g_relative = true;
        SetInput(0);
        PInv[2] = 0;
        CBankParams p;
        std::memset(&p, 0xff, sizeof(p));
        g_dm.Pick(0x10, &p.F[0]); g_dm.Pick(0x10, &p.F[1]); g_dm.Pick(0x10, &p.F[3]);
        g_dm.Pick(0x20, &p.F[5]); g_dm.Pick(0x20, &p.F[4]); g_dm.Pick(0x10, &p.F[6]);
        p.Value20 = -1; p.Value24 = -1; p.Flag1c = false; p.Flag28 = false;
        if (g_pFE && g_pFE->Active) Bank = "GBANK_FRONT_END"; else Bank = "GBANK_MAIN";
        Used = p;
        SetMetaLayer(0);
    }
};
static int Fail() { std::printf("0041e3f6_TEST FAIL\n"); return 1; }
int main()
{
    CManager a;
    if (std::strcmp(a.Bank, "GBANK_MAIN") != 0 || a.InputType != 0 || a.MetaLayer != 0 || a.ScrollOffset != -1.0f || !g_relative) return Fail();
    if (a.Used.F[0] != 0xa1 || a.Used.F[5] != 0x144 || a.Used.F[6] != 0xa6 || a.Used.Value20 != -1 || a.Used.Flag1c) return Fail();
    CNewFrontendGameComponent fe = { true };
    g_pFE = &fe;
    CManager b;
    if (std::strcmp(b.Bank, "GBANK_FRONT_END") != 0) return Fail();
    fe.Active = false;
    CManager c;
    if (std::strcmp(c.Bank, "GBANK_MAIN") != 0 || c.PInv[9] != 0 || c.Error != 0) return Fail();
    std::printf("0041e3f6_TEST PASS\n");
    return 0;
}