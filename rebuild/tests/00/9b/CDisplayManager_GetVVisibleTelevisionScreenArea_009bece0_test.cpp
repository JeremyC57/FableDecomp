#include <cstdio>
static double g_w = 640.0, g_h = 480.0;
struct C2DBoxF { float TLX, TLY, BRX, BRY; C2DBoxF(float a, float b, float c, float d) : TLX(a), TLY(b), BRX(c), BRY(d) { } };
struct CDisplayManager { C2DBoxF Get() const { return C2DBoxF(0.0f, 0.0f, (float)g_w, (float)g_h); } };
int main() { CDisplayManager m; C2DBoxF b = m.Get(); if (b.TLX != 0.0f || b.TLY != 0.0f || b.BRX != 640.0f || b.BRY != 480.0f) { std::printf("009bece0_TEST FAIL\n"); return 1; } std::printf("009bece0_TEST PASS\n"); return 0; }