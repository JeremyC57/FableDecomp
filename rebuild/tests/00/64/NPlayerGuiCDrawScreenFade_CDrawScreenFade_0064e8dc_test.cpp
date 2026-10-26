#include <cstdio>
#include <cstring>
#include <cmath>

class CPlayerGui { public: void* w; void* pm; void* ii; void* comp; void* bank; };
static int g_resizeCount = 0, g_resizeArg = -1, g_baseCalls = 0;
struct CRetailHandleVector { void* f; void* l; void* e; void Resize(long n) { g_resizeCount++; g_resizeArg = n; } };
struct CDrawBase {
    virtual void v00() {}
    CPlayerGui* PlayerGui; void* PlayerManager; void* World; void* Component; void* Bank; bool Drawing; unsigned char _p[3];
    CRetailHandleVector PrimitiveHandles;
    CDrawBase(CPlayerGui* g, void* list) : PlayerGui(g), Drawing(false) { PrimitiveHandles.f = PrimitiveHandles.l = PrimitiveHandles.e = (void*)1; g_baseCalls++; (void)list; }
};

struct CDrawScreenFade : CDrawBase { virtual void v00() {} bool DrawnFullFadeOut; CDrawScreenFade(CPlayerGui* g, void* l) : CDrawBase(g, l), DrawnFullFadeOut(false) { PrimitiveHandles.Resize(1); } };
int main() {
    CPlayerGui gui; CDrawScreenFade c(&gui, 0);
    if (g_baseCalls != 1 || c.DrawnFullFadeOut || g_resizeCount != 1 || g_resizeArg != 1) { std::printf("0064e8dc_TEST FAIL\n"); return 1; }
    std::printf("0064e8dc_TEST PASS\n"); return 0;
}