// Behaviour test for 0x0045BBC5 std::vector<CRGBColour>::_M_allocate_and_copy(n, first, last)
#include <cstdio>
#include <cstdlib>
#include <new>
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) { }
};
static void Construct(CRGBColour* p, const CRGBColour& v) { new (p) CRGBColour(v); }
static CRGBColour* AllocateAndCopy(unsigned int n, const CRGBColour* first, const CRGBColour* last)
{
    CRGBColour* result = n != 0 ? static_cast<CRGBColour*>(std::malloc(n * sizeof(CRGBColour))) : 0;
    CRGBColour* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return result;
}
int main()
{
    CRGBColour src[4];
    for (int i = 0; i < 4; ++i) { src[i].B = (unsigned char)i; src[i].G = (unsigned char)(i + 10); src[i].R = (unsigned char)(i + 20); src[i].A = 255; }
    CRGBColour* p = AllocateAndCopy(6, src, src + 4);
    for (int i = 0; i < 4; ++i)
        if (p[i].B != i || p[i].G != i + 10 || p[i].R != i + 20 || p[i].A != 255) { std::printf("0045bbc5_TEST FAIL\n"); return 1; }
    std::free(p);
    if (AllocateAndCopy(0, src, src) != 0) { std::printf("0045bbc5_TEST FAIL\n"); return 1; }
    std::printf("0045bbc5_TEST PASS\n");
    return 0;
}