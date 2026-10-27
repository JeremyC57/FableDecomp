// Test for 0x006CACB0 std::__uninitialized_fill_n<CCompositeBlendDesc*, unsigned int, CCompositeBlendDesc>: constructs n
// copies of x (both colours channel by channel) and returns first + n; n == 0 returns first.
#include <stdio.h>
#include <string.h>
#include <new>
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); }
};
class CCompositeBlendDesc { public: unsigned int TextureIndex; int BlendType; CRGBColour DiffuseColour; CRGBColour SpecularColour; };
static CCompositeBlendDesc* fill_n(CCompositeBlendDesc* first, unsigned int n, const CCompositeBlendDesc& x)
{
    CCompositeBlendDesc* cur = first;
    for (; n > 0; --n, ++cur) new ((void*)cur) CCompositeBlendDesc(x);
    return cur;
}
int main()
{
    CCompositeBlendDesc x; x.TextureIndex = 7; x.BlendType = 2; x.DiffuseColour.Set(1, 2, 3, 4); x.SpecularColour.Set(5, 6, 7, 8);
    static unsigned char raw[4 * sizeof(CCompositeBlendDesc)]; memset(raw, 0xEE, sizeof(raw));
    CCompositeBlendDesc* d = (CCompositeBlendDesc*)raw;
    bool ok = fill_n(d, 3, x) == d + 3;
    for (int i = 0; i < 3; ++i)
        if (d[i].TextureIndex != 7 || d[i].BlendType != 2 || d[i].DiffuseColour.R != 1 || d[i].DiffuseColour.A != 4 ||
            d[i].SpecularColour.B != 7 || d[i].SpecularColour.G != 6) ok = false;
    if (d[3].TextureIndex != 0xEEEEEEEEu) ok = false;
    if (fill_n(d, 0, x) != d) ok = false;
    puts(ok ? "006cacb0_TEST PASS" : "006cacb0_TEST FAIL");
    return ok ? 0 : 1;
}