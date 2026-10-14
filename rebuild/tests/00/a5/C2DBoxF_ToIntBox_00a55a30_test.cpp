#include <cstdio>
struct C2DBoxI { long TLX, TLY, BRX, BRY; C2DBoxI(long a, long b, long c, long d) : TLX(a), TLY(b), BRX(c), BRY(d) { } };
struct C2DBoxF { float TLX, TLY, BRX, BRY; C2DBoxI ToIntBox() const { return C2DBoxI((long)TLX, (long)TLY, (long)BRX, (long)BRY); } };
int main() { C2DBoxF f = { 1.9f, -2.9f, 3.1f, 4.5f }; C2DBoxI b = f.ToIntBox(); if (b.TLX != 1 || b.TLY != -2 || b.BRX != 3 || b.BRY != 4) { std::printf("00a55a30_TEST FAIL\n"); return 1; } std::printf("00a55a30_TEST PASS\n"); return 0; }