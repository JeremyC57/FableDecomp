#include <cstdio>
static long g_a, g_b;
struct CDisplayElementBase { virtual void v00() { } unsigned char _body[0x20 - 4]; CDisplayElementBase(long a, long b) { g_a = a; g_b = b; } };
struct CDisplayElement : CDisplayElementBase { long m_style; CDisplayElement(long a, long s, long b) : CDisplayElementBase(a, b), m_style(s) { } };
int main() { CDisplayElement e(1, 3, 2); if (g_a != 1 || g_b != 2 || e.m_style != 3) { std::printf("00c1d880_TEST FAIL\n"); return 1; } std::printf("00c1d880_TEST PASS\n"); return 0; }
