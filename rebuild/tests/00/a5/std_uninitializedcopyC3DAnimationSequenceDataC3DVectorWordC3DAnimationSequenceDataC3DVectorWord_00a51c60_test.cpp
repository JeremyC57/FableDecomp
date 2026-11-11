// Behaviour test for 0x00A51C60 UninitCopy_C3DTriangle2_00a51c60: placement copy of 6-byte index triangles.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
class C3DTriangle2 { public: unsigned short VertexIndex[3]; };

static C3DTriangle2* UninitCopy(const C3DTriangle2* first, const C3DTriangle2* last, C3DTriangle2* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) C3DTriangle2(*first);
    return result;
}

int main()
{
    C3DTriangle2 src[4];
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 3; ++j)
            src[i].VertexIndex[j] = (unsigned short)(100 * i + j);
    C3DTriangle2 dst[4];
    __false_type tag;
    C3DTriangle2* r = UninitCopy(src, src + 4, dst, tag);
    bool ok = r == dst + 4 && sizeof(C3DTriangle2) == 6;
    for (int k = 0; k < 4; ++k)
        for (int m = 0; m < 3; ++m)
            if (dst[k].VertexIndex[m] != (unsigned short)(100 * k + m)) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("00a51c60_TEST FAIL\n"); return 1; }
    std::printf("00a51c60_TEST PASS\n");
    return 0;
}