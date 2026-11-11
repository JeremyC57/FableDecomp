// Behaviour test for 0x00A52040 UninitCopy_C3DVectorWord_00a52040: placement copy of 6-byte short vectors.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct C3DVectorWord { short X; short Y; short Z; };

static C3DVectorWord* UninitCopy(const C3DVectorWord* first, const C3DVectorWord* last, C3DVectorWord* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) C3DVectorWord(*first);
    return result;
}

int main()
{
    C3DVectorWord src[3];
    for (int i = 0; i < 3; ++i) { src[i].X = (short)i; src[i].Y = (short)(-5 * i); src[i].Z = (short)(1000 + i); }
    C3DVectorWord dst[3];
    __false_type tag;
    C3DVectorWord* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && sizeof(C3DVectorWord) == 6;
    for (int k = 0; k < 3; ++k)
        if (dst[k].X != k || dst[k].Y != -5 * k || dst[k].Z != 1000 + k) ok = false;
    if (UninitCopy(src, src, dst + 1, tag) != dst + 1) ok = false;
    if (!ok) { std::printf("00a52040_TEST FAIL\n"); return 1; }
    std::printf("00a52040_TEST PASS\n");
    return 0;
}