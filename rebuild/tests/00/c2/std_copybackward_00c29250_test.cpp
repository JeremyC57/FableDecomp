// Behaviour test for std::copy_backward<CHandleInfo*, CHandleInfo*> @ 0x00C29250 (the non-scalar element loop).
#include <cstdio>

struct CHeader;
struct CHandleInfo {
    union { CHeader* Header; long NextFreeHandle; };
};

namespace model {
template<class BidIt1, class BidIt2>
BidIt2 copy_backward(BidIt1 first, BidIt1 last, BidIt2 dest)
{
    while (first != last)
        *--dest = *--last;
    return dest;
}
}

int main()
{
    CHandleInfo a[6];
    for (int i = 0; i < 6; ++i) a[i].NextFreeHandle = i + 10;
    // overlapping move right by 2: [0,4) -> [2,6)
    CHandleInfo* r = model::copy_backward(a, a + 4, a + 6);
    bool ok = (r == a + 2);
    long expect[6] = { 10, 11, 10, 11, 12, 13 };
    for (int i = 0; i < 6; ++i) if (a[i].NextFreeHandle != expect[i]) ok = false;
    // empty range returns dest unchanged
    if (model::copy_backward(a, a, a + 3) != a + 3) ok = false;
    if (!ok) { std::printf("00c29250_TEST FAIL\n"); return 1; }
    std::printf("00c29250_TEST PASS\n");
    return 0;
}