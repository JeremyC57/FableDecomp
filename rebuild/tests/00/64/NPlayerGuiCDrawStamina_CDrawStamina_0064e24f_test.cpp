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

static int g_emitterResize = 0, g_fadeCtor = 0;
struct CRetailEmitterVector { void* f; void* l; void* e; CRetailEmitterVector() : f(0), l(0), e(0) {} void Resize(long n) { g_emitterResize = n; } };
struct CFadeInFadeOutBase { unsigned char b[0x1c]; CFadeInFadeOutBase() { std::memset(b, 0, sizeof b); g_fadeCtor++; } };
struct CDrawStamina : CDrawBase { virtual void v00() {} CRetailEmitterVector Effects, IncreaseEffects; long PreviousStamina, PreviousMaxStamina; CFadeInFadeOutBase StaminaIncreaseFade;
    CDrawStamina(CPlayerGui* g, void* l) : CDrawBase(g, l), Effects(), IncreaseEffects(), PreviousStamina(-1), PreviousMaxStamina(-1), StaminaIncreaseFade() { PrimitiveHandles.Resize(10); Effects.Resize(2); } };
int main() {
    CPlayerGui gui; CDrawStamina c(&gui, 0);
    if (g_baseCalls != 1 || c.Effects.f || c.IncreaseEffects.e || c.PreviousStamina != -1 || c.PreviousMaxStamina != -1 || g_fadeCtor != 1 || g_resizeArg != 10 || g_emitterResize != 2) { std::printf("0064e24f_TEST FAIL\n"); return 1; }
    std::printf("0064e24f_TEST PASS\n"); return 0;
}