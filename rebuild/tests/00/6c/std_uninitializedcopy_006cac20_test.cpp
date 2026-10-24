#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() : B(0), G(0), R(0), A(0) { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
enum ECompositeBlendType { COMPOSITE_BLEND_TYPE_0, COMPOSITE_BLEND_TYPE_3 = 3 };
struct CCompositeBlendDesc { unsigned long TextureIndex; ECompositeBlendType BlendType; CRGBColour DiffuseColour; CRGBColour SpecularColour; CCompositeBlendDesc() : TextureIndex(0), BlendType(COMPOSITE_BLEND_TYPE_0) { } };
typedef CCompositeBlendDesc TElem;
static void Fill(TElem& e, int i)
{
    e.TextureIndex = 9u * i; e.BlendType = COMPOSITE_BLEND_TYPE_3; e.DiffuseColour.Set((unsigned char)i, (unsigned char)(i + 1), (unsigned char)(i + 2), (unsigned char)(i + 3)); e.SpecularColour.Set((unsigned char)(50 + i), (unsigned char)(60 + i), (unsigned char)(70 + i), (unsigned char)(80 + i));
}
static bool Check(const TElem& e, int i)
{
    return e.TextureIndex == 9u * i && e.BlendType == COMPOSITE_BLEND_TYPE_3 && e.DiffuseColour.R == i && e.DiffuseColour.G == i + 1 && e.DiffuseColour.B == i + 2 && e.DiffuseColour.A == i + 3 && e.SpecularColour.R == 50 + i && e.SpecularColour.G == 60 + i && e.SpecularColour.B == 70 + i && e.SpecularColour.A == 80 + i;
}
static TElem* UninitCopy(const TElem* first, const TElem* last, TElem* result, const __false_type&)
{
    TElem* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) TElem(*first);
    return cur;
}
int main()
{
    static double srcStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    static double dstStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    TElem* src = (TElem*)srcStorage;
    TElem* dst = (TElem*)dstStorage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(src + i)) TElem(); Fill(src[i], i + 1); }
    __false_type tag;
    TElem* end = UninitCopy(src, src + 3, dst, tag);
    bool ok = end == dst + 3;
    for (i = 0; i < 3; ++i) if (!Check(dst[i], i + 1)) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3) ok = false;
    
    if (!ok) { std::printf("006cac20_TEST FAIL\n"); return 1; }
    std::printf("006cac20_TEST PASS\n");
    return 0;
}