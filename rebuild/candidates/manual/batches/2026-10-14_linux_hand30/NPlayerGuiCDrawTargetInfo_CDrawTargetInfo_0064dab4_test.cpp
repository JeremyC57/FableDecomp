#include <cstdio>
#include <cstring>
#include <cmath>

struct CPlayerGui { void* w; void* pm; void* ii; void* comp; void* bank; };
static int g_resizeCount = 0, g_resizeArg = -1, g_baseCalls = 0;
struct CRetailHandleVector { void* f; void* l; void* e; void Resize(long n) { g_resizeCount++; g_resizeArg = n; } };
struct CDrawBase {
    virtual void v00() {}
    CPlayerGui* PlayerGui; void* PlayerManager; void* World; void* Component; void* Bank; bool Drawing; unsigned char _p[3];
    CRetailHandleVector PrimitiveHandles;
    CDrawBase(CPlayerGui* g, void* list) : PlayerGui(g), Drawing(false) { PrimitiveHandles.f = PrimitiveHandles.l = PrimitiveHandles.e = (void*)1; g_baseCalls++; (void)list; }
};

static int g_fadeCtor = 0;
struct CFadeInFadeOutBase { unsigned char b[0x1c]; CFadeInFadeOutBase() { std::memset(b, 0, sizeof b); b[0x1b] = 0xff; g_fadeCtor++; } };
struct CDrawTargetInfo : CDrawBase { virtual void v00() {} CFadeInFadeOutBase AttackAlphaFade, BlockAlphaFade; CDrawTargetInfo(CPlayerGui* g, void* l) : CDrawBase(g, l), AttackAlphaFade(), BlockAlphaFade() {} };
int main() {
    CPlayerGui gui; CDrawTargetInfo c(&gui, 0);
    if (g_baseCalls != 1 || g_fadeCtor != 2 || c.AttackAlphaFade.b[0] || c.BlockAlphaFade.b[0x1b] != 0xff) { std::printf("0064dab4_TEST FAIL\n"); return 1; }
    std::printf("0064dab4_TEST PASS\n"); return 0;
}