// Behaviour test for NUISystem::CIMEFont::SetColor (retail 0x00404D20): keeps the colour's B, G, R bytes and replaces
// the alpha with the IME composition alpha.
#include <cstdio>
#include <cstring>
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
static unsigned char g_alpha = 0x80;
struct CIMEFont {
    CRGBColour Colour;
    void SetColor(unsigned long colour)
    {
        unsigned long high = colour >> 8;
        Colour = CRGBColour((unsigned char)(high >> 8), (unsigned char)high, (unsigned char)colour, g_alpha);
    }
};
int main()
{
    CIMEFont f;
    f.SetColor(0xFF112233UL);
    if (f.Colour.R != 0x11 || f.Colour.G != 0x22 || f.Colour.B != 0x33 || f.Colour.A != 0x80) {
        std::printf("00404d20_TEST FAIL\n"); return 1;
    }
    std::printf("00404d20_TEST PASS\n");
    return 0;
}