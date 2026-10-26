// Behaviour test for 0x0045BB8F std::_Construct<CRGBColour>(p, value): placement copy of a B,G,R,A colour, null-safe
#include <cstdio>
#include <new>
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); }
};
static void Construct(CRGBColour* p, const CRGBColour& v) { new (p) CRGBColour(v); }
int main()
{
    CRGBColour src; src.B = 1; src.G = 2; src.R = 3; src.A = 4;
    CRGBColour dst; dst.B = dst.G = dst.R = dst.A = 0;
    Construct(&dst, src);
    if (dst.B != 1 || dst.G != 2 || dst.R != 3 || dst.A != 4) { std::printf("0045bb8f_TEST FAIL\n"); return 1; }
    std::printf("0045bb8f_TEST PASS\n");
    return 0;
}