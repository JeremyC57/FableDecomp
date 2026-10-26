// Behaviour test for CEnginePrimitive2DText::CEnginePrimitive2DText(const CEnginePrimitive2DText&) @ 0x00655048.
#include <cstdio>
static int g_textCopies = 0;
class C2DVector { public: float X, Y; };
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); }
};
class CWideString { public: const void* m_pData; CWideString(const void* p) : m_pData(p) { } CWideString(const CWideString& o) : m_pData(o.m_pData) { ++g_textCopies; } };
class CFontBank { public: int id; };
struct SEnginePrimitive2DHeader { long m_type, m_interpolation; bool m_flag8, m_flag9, m_flagA; };
class CEnginePrimitive2DText { public:
    SEnginePrimitive2DHeader m_header; C2DVector m_position; unsigned int m_userData; long m_layer; float m_depth;
    CRGBColour m_colour; CWideString m_text; CFontBank* m_pFont;
    CEnginePrimitive2DText(const SEnginePrimitive2DHeader& h, const CRGBColour& c, const CWideString& t)
        : m_header(h), m_colour(c), m_text(t) { }
    CEnginePrimitive2DText(const CEnginePrimitive2DText& o)
        : m_header(o.m_header), m_position(o.m_position), m_userData(o.m_userData), m_layer(o.m_layer), m_depth(o.m_depth),
          m_colour(o.m_colour), m_text(o.m_text), m_pFont(o.m_pFont) { }
};
int main()
{
    SEnginePrimitive2DHeader h = { 0x27, 3, true, false, true }; CRGBColour c(1, 2, 3, 4); static const char txt[] = "hi";
    CWideString t(txt); CFontBank font = { 9 };
    CEnginePrimitive2DText a(h, c, t);
    a.m_position.X = 5; a.m_position.Y = 6; a.m_userData = 7; a.m_layer = 8; a.m_depth = 0.5f; a.m_pFont = &font;
    g_textCopies = 0;
    CEnginePrimitive2DText b(a);
    int ok = b.m_header.m_type == 0x27 && b.m_header.m_interpolation == 3 && b.m_header.m_flag8 && !b.m_header.m_flag9 && b.m_header.m_flagA
          && b.m_position.X == 5 && b.m_position.Y == 6 && b.m_userData == 7 && b.m_layer == 8 && b.m_depth == 0.5f
          && b.m_colour.R == 1 && b.m_colour.G == 2 && b.m_colour.B == 3 && b.m_colour.A == 4
          && b.m_text.m_pData == txt && g_textCopies == 1 && b.m_pFont == &font;
    if (!ok) { std::printf("00655048_TEST FAIL\n"); return 1; }
    std::printf("00655048_TEST PASS\n");
    return 0;
}