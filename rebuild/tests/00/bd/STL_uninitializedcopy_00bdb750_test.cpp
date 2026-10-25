// behaviour test for 0x00BDB750 std::__uninitialized_copy over C2DLineF
#include <cstdio>
#include <new>

struct C2DVectorF { float X, Y; };
struct C2DLineF { C2DVectorF Start; C2DVectorF End; };
struct FalseType { };

static C2DLineF* UninitCopy(const C2DLineF* first, const C2DLineF* last, C2DLineF* result, const FalseType&)
{
    for (; first != last; ++first, ++result)
        new (result) C2DLineF(*first);
    return result;
}

int main()
{
    C2DLineF src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].Start.X = (float)i; src[i].Start.Y = (float)(i * 2);
        src[i].End.X = (float)(i * 3); src[i].End.Y = (float)(i * 4);
    }
    C2DLineF dst[4];
    dst[3].Start.X = 99.0f;
    FalseType tag;
    C2DLineF* end = UninitCopy(src, src + 3, dst, tag);
    if (end != dst + 3) { std::printf("00bdb750_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (dst[i].Start.X != src[i].Start.X || dst[i].Start.Y != src[i].Start.Y ||
            dst[i].End.X != src[i].End.X || dst[i].End.Y != src[i].End.Y) { std::printf("00bdb750_TEST FAIL\n"); return 1; }
    if (dst[3].Start.X != 99.0f) { std::printf("00bdb750_TEST FAIL\n"); return 1; }
    if (UninitCopy(src, src, dst, tag) != dst) { std::printf("00bdb750_TEST FAIL\n"); return 1; }
    std::printf("00bdb750_TEST PASS\n");
    return 0;
}