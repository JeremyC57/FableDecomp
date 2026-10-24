// Behaviour test for 0x00718390 __uninitialized_copy<CCameraTag*>: every 24-byte tag copied, end of the copy returned.
#include <cstdio>

struct CCameraTag { int PositionPerson, Position, FocusPerson, FocusPosition, ZoomType, View; };
static CCameraTag* UninitCopy(const CCameraTag* first, const CCameraTag* last, CCameraTag* result)
{
    CCameraTag* cur = result;
    for (; first != last; ++first, ++cur) *cur = *first;
    return cur;
}
int main()
{
    CCameraTag src[3] = { { 2, 3, 1, 0, 0, 1 }, { 1, 0, 2, 3, 1, 0 }, { 4, 4, 4, 4, 4, 4 } };
    CCameraTag dst[3] = { { 0 } };
    CCameraTag* end = UninitCopy(src, src + 3, dst);
    bool ok = end == dst + 3 && dst[0].Position == 3 && dst[0].View == 1 && dst[1].FocusPosition == 3 && dst[2].ZoomType == 4;
    ok = ok && UninitCopy(src, src, dst) == dst;
    if (!ok) { std::printf("00718390_TEST FAIL\n"); return 1; }
    std::printf("00718390_TEST PASS\n");
    return 0;
}