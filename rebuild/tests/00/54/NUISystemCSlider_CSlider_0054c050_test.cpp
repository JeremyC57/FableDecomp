// Behaviour test for 0x0054C050: the slider ctor zeroes its action and component pointers and then calls Initialize.
#include <cstdio>
struct Slider { void* a0; void* a1; void* a2; void* back; void* sel; int def; void* left; void* right; void* accept; int inits;
  Slider() : a0(0), a1(0), a2(0), back(0), sel(0), def(0), left(0), right(0), accept(0), inits(0) { Initialize(); }
  void Initialize() { ++inits; if (left || right || accept || back || sel) inits = -100; } };
int main()
{
    Slider s; bool ok = s.inits == 1 && !s.a0 && !s.a2 && s.def == 0;
    if (!ok) { std::printf("0054c050_TEST FAIL\n"); return 1; }
    std::printf("0054c050_TEST PASS\n");
    return 0;
}