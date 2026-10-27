// behaviour test for 0x00A74560 std::__copy_backward over 16-byte C2DLineF elements (model with a 16-byte stand-in)
#include <cstdio>
struct TElem { int a, b, c, d; };
static TElem* CopyBackward(TElem* first, TElem* last, TElem* result) { for (int n = (int)(last - first); n > 0; --n) *--result = *--last; return result; }
int main()
{
    TElem e[5];
    for (int i = 0; i < 5; ++i) { e[i].a = i; e[i].b = i * 2; e[i].c = i * 3; e[i].d = i * 4; }
    TElem* r = CopyBackward(e, e + 3, e + 4);          // shift [0,3) up by one (overlapping)
    int ok = r == e + 1 && e[1].a == 0 && e[2].a == 1 && e[3].a == 2 && e[3].d == 8 && e[4].a == 4;
    ok = ok && CopyBackward(e, e, e + 2) == e + 2;
    if (!ok) { std::printf("00a74560_TEST FAIL\n"); return 1; }
    std::printf("00a74560_TEST PASS\n");
    return 0;
}