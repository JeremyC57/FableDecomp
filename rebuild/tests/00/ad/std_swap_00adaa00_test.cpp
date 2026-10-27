// behaviour test for 0x00ADAA00 std::swap (model: the two 12-byte elements exchange)
#include <cstdio>
struct E { unsigned int w[3]; };
static void Swap(E& a, E& b) { E t = a; a = b; b = t; }
int main()
{
    E a = { { 1, 2, 3 } }, b = { { 4, 5, 6 } };
    Swap(a, b);
    int ok = a.w[0] == 4 && a.w[2] == 6 && b.w[0] == 1 && b.w[2] == 3;
    if (!ok) { std::printf("00adaa00_TEST FAIL\n"); return 1; }
    std::printf("00adaa00_TEST PASS\n");
    return 0;
}