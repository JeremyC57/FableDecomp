// Test for 0x006CAC20 std::__uninitialized_copy<CCompositeBlendDesc*, CCompositeBlendDesc*>: the source object must
// copy every 16-byte record of [first, last) (both colours channel by channel) and return result + n; an empty range
// returns result unchanged.
#include <stdio.h>
class CRGBColour {
public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
enum ECompositeBlendType { COMPOSITE_BLEND_TYPE_0 };
class CCompositeBlendDesc {
public:
    unsigned long TextureIndex; ECompositeBlendType BlendType; CRGBColour DiffuseColour; CRGBColour SpecularColour;
};
namespace std {
struct __false_type { __false_type() { } };
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
}
static unsigned char g_src[4][16], g_dst[4][16];
int main()
{
    int i, k;
    for (i = 0; i < 3; ++i) for (k = 0; k < 16; ++k) g_src[i][k] = (unsigned char)(i * 16 + k + 1);
    CCompositeBlendDesc* s = (CCompositeBlendDesc*)g_src;
    CCompositeBlendDesc* d = (CCompositeBlendDesc*)g_dst;
    std::__false_type tag;
    CCompositeBlendDesc* e = std::__uninitialized_copy(s, s + 3, d, tag);
    bool ok = e == d + 3;
    for (i = 0; i < 3; ++i) for (k = 0; k < 16; ++k) if (g_dst[i][k] != g_src[i][k]) ok = false;
    for (k = 0; k < 16; ++k) if (g_dst[3][k] != 0) ok = false;
    if (std::__uninitialized_copy(s, s, d + 3, tag) != d + 3) ok = false;
    if (!ok) { puts("006cac20_TEST FAIL"); return 1; }
    puts("006cac20_TEST PASS");
    return 0;
}
