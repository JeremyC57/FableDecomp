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

struct C2DCoordI { long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) {} };
struct CDrawMouseCursor : CDrawBase { virtual void v00() {} C2DCoordI Pos; CDrawMouseCursor(CPlayerGui* g, void* l) : CDrawBase(g, l), Pos(0, 0) { PrimitiveHandles.Resize(1); } };
int main() {
    CPlayerGui gui; CDrawMouseCursor c(&gui, 0);
    if (g_baseCalls != 1 || c.Pos.X != 0 || c.Pos.Y != 0 || g_resizeCount != 1 || g_resizeArg != 1 || c.PlayerGui != &gui) { std::printf("00658240_TEST FAIL\n"); return 1; }
    std::printf("00658240_TEST PASS\n"); return 0;
}