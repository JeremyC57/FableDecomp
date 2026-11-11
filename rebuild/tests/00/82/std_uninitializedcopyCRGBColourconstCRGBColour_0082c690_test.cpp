// Behaviour test for 0x0082C690 UninitCopy_CRGBColour_0082c690: placement copy of colours through Set(r, g, b, a).
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() : B(0), G(0), R(0), A(0) { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

static CRGBColour* UninitCopy(const CRGBColour* first, const CRGBColour* last, CRGBColour* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) CRGBColour(*first);
    return result;
}

int main()
{
    CRGBColour src[3];
    for (int i = 0; i < 3; ++i) src[i].Set((unsigned char)(10 + i), (unsigned char)(20 + i), (unsigned char)(30 + i), (unsigned char)(40 + i));
    CRGBColour dst[3];
    __false_type tag;
    CRGBColour* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && sizeof(CRGBColour) == 4;
    for (int k = 0; k < 3; ++k)
        if (dst[k].R != 10 + k || dst[k].G != 20 + k || dst[k].B != 30 + k || dst[k].A != 40 + k) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("0082c690_TEST FAIL\n"); return 1; }
    std::printf("0082c690_TEST PASS\n");
    return 0;
}