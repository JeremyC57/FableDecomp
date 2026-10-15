// Behaviour test for 0x00544480: def values copied into both offset pairs, alpha scaled by 1/255.
#include <cstdio>
struct V2 { float X, Y; };
struct Def { int count; V2 off; unsigned char alpha; float delay; void Release() { --count; } };
struct List { V2 pos, posUse; float alpha, alphaUse, delay;
  void Get(Def& d) { ++d.count; posUse = d.off; pos = d.off; alpha = alphaUse = (float)d.alpha * (1.0f / 255.0f); delay = d.delay; d.Release(); } };
int main()
{
    Def d = {1, {2.0f, 3.0f}, 255, 0.25f}; List l; l.Get(d);
    bool ok = l.pos.X == 2.0f && l.posUse.Y == 3.0f && l.alpha == l.alphaUse && l.alpha > 0.999f && l.alpha < 1.001f && l.delay == 0.25f && d.count == 1;
    if (!ok) { std::printf("00544480_TEST FAIL\n"); return 1; }
    std::printf("00544480_TEST PASS\n");
    return 0;
}
