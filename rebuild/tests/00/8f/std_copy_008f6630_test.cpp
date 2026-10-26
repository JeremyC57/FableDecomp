// behaviour test for 0x008F6630 std::__copy<const CHeightMapCell*, CHeightMapCell*, int>: copies last - first 12-byte records forward and returns
// result + n (result unchanged for an empty or reversed range)
#include <cstdio>
struct Rec { int a; int b; int c; };
static Rec* Copy(const Rec* first, const Rec* last, Rec* result)    // the function under test
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
int main()
{
    int fail = 0;
    Rec src[3] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    Rec dst[4] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { -1, -1, -1 } };
    if (Copy(src, src + 3, dst) != dst + 3) fail |= 1;
    if (dst[0].a != 1 || dst[1].b != 5 || dst[2].c != 9 || dst[3].a != -1) fail |= 2;
    if (Copy(src, src, dst + 1) != dst + 1 || Copy(src + 2, src, dst) != dst) fail |= 4;
    Rec ov[4] = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 }, { 4, 4, 4 } };
    Copy(ov + 1, ov + 4, ov);                                    // forward copy to a lower address is safe
    if (ov[0].a != 2 || ov[2].a != 4 || ov[3].a != 4) fail |= 8;
    if (sizeof(Rec) != (unsigned int)12) fail |= 16;
    if (fail) { std::printf("008f6630_TEST FAIL %d\n", fail); return 1; }
    std::printf("008f6630_TEST PASS\n");
    return 0;
}