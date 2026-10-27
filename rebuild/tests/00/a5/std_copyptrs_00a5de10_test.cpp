#include <cstdio>
// Behaviour model of STLport __copy over 8-byte trivially assigned pairs: n = last - first assignments, return the end.
struct P { int a, b; };
static P* copy_(const P* first, const P* last, P* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    P src[4] = { {1, 2}, {3, 4}, {5, 6}, {7, 8} };
    P dst[4] = { {0, 0}, {0, 0}, {0, 0}, {0, 0} };
    P* e = copy_(src + 1, src + 4, dst);
    if (e != dst + 3 || dst[0].a != 3 || dst[1].b != 6 || dst[2].a != 7 || dst[3].a != 0) { std::printf("00a5de10_TEST FAIL\n"); return 1; }
    if (copy_(src, src, dst) != dst) { std::printf("00a5de10_TEST FAIL\n"); return 1; }
    P v[4] = { {1, 1}, {2, 2}, {3, 3}, {4, 4} };                    // overlapping down-shift (erase)
    copy_(v + 1, v + 4, v);
    if (v[0].a != 2 || v[2].a != 4 || v[3].a != 4) { std::printf("00a5de10_TEST FAIL\n"); return 1; }
    std::printf("00a5de10_TEST PASS\n");
    return 0;
}