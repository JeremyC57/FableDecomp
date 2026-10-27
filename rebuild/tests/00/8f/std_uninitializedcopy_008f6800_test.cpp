// behaviour test for 0x008F6800 __uninitialized_copy (model: each 12-byte element copied into the raw block, the end returned)
#include <cstdio>
#include <new>
struct E { int a; float b; unsigned char c; };
int main()
{
    E src[2] = { { 1, 2.0f, 3 }, { 4, 5.0f, 6 } };
    static double raw[4];
    E* dst = (E*)raw; E* cur = dst;
    for (const E* p = src; p != src + 2; ++p, ++cur) new (cur) E(*p);
    int ok = cur == dst + 2 && dst[1].a == 4 && dst[1].c == 6 && dst[0].b == 2.0f;
    if (!ok) { std::printf("008f6800_TEST FAIL\n"); return 1; }
    std::printf("008f6800_TEST PASS\n");
    return 0;
}