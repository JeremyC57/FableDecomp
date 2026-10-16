#include <cstdio>
struct CSlider {
    void* a0; void* a1; void* a2; void* back; void* sel; float delay; long def; void* l; void* r; void* acc; unsigned int maxc; int inits;
    void Initialize() { ++inits; }
    CSlider() : a0(0), a1(0), a2(0), back(0), sel(0), delay(0), def(0), l(0), r(0), acc(0), maxc(0), inits(0) { Initialize(); }
};
int main()
{
    CSlider s;
    if (s.inits != 1 || s.a0 || s.back || s.sel || s.delay != 0.0f || s.def || s.l || s.r || s.acc || s.maxc) { std::printf("00549f60_TEST FAIL\n"); return 1; }
    std::printf("00549f60_TEST PASS\n");
    return 0;
}
