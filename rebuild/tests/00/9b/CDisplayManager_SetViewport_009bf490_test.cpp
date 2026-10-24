// Behaviour test for 0x009BF490 CDisplayManager::SetViewport(const C2DBoxF&): round each corner (current FPU mode) and forward.
#include <cstdio>
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) { TLX = tlx; TLY = tly; BRX = brx; BRY = bry; }
    C2DBoxI(const C2DBoxI& b) : TLX(b.TLX), TLY(b.TLY), BRX(b.BRX), BRY(b.BRY) { }
};
class CDisplayManager {
public:
    long Calls; long L, T, R, B; const CDisplayManager* Self;
    CDisplayManager() : Calls(0), L(0), T(0), R(0), B(0), Self(0) { }
    void SetViewport(const C2DBoxI& box) { ++Calls; L = box.TLX; T = box.TLY; R = box.BRX; B = box.BRY; Self = this; }
    void SetViewport(const C2DBoxF& box);
};
#ifdef _MSC_VER
static __forceinline long FtoL(float f) 
{
    long result;
    __asm fld f
    __asm fistp result
    return result;
}
#else
static inline long FtoL(float f) { int r; __asm__("fistpl %0" : "=m"(r) : "t"(f) : "st"); return r; }
#endif
void CDisplayManager::SetViewport(const C2DBoxF& box)
{
    SetViewport(C2DBoxI(FtoL(box.TLX), FtoL(box.TLY), FtoL(box.BRX), FtoL(box.BRY)));
}
int main()
{
    CDisplayManager d;
    C2DBoxF a = { 10.6f, -20.6f, 630.4f, 460.5f };
    d.SetViewport(a);
    if (d.Calls != 1 || d.Self != &d || d.L != 11 || d.T != -21 || d.R != 630 || d.B != 460) { std::printf("009bf490_TEST FAIL\n"); return 1; }
    C2DBoxF b = { 0.0f, 1.5f, -0.4f, 1023.5f };
    d.SetViewport(b);
    if (d.Calls != 2 || d.L != 0 || d.T != 2 || d.R != 0 || d.B != 1024) { std::printf("009bf490_TEST FAIL\n"); return 1; }
    std::printf("009bf490_TEST PASS\n");
    return 0;
}
