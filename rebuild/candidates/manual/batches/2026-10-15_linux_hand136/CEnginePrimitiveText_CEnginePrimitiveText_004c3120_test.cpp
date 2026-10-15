// Behaviour test for 0x004C3120 (see src/004c3120.cpp): the constructor copied onto a portable model.
#include <cstdio>
#define CHECK(c) do { if (!(c)) { std::printf("004c3120_TEST FAIL\n"); return 1; } } while (0)
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
struct CRGBColour {                                      // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); }
};
struct CEnginePrimitiveThemeChannelDesc {
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
struct CEnginePrimitive {
    unsigned int Type;
    unsigned int InterpolationSet;
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
    CEnginePrimitive(unsigned int type, unsigned int interpolationSet = 0) : Type(type), InterpolationSet(interpolationSet) { }
};
static bool Base(const CEnginePrimitive& p, unsigned int type, unsigned int interp)
{
    return p.Type == type && p.InterpolationSet == interp && p.ThemeChannelDesc.ChannelIndex0 == 0
        && p.ThemeChannelDesc.ChannelIndex1 == 0 && p.ThemeChannelDesc.ChannelBlendFactor == 0;
}
static bool Col(const CRGBColour& c, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return c.R == r && c.G == g && c.B == b && c.A == a;
}
static bool Vec(const C3DVector& v, float x, float y, float z) { return v.X == x && v.Y == y && v.Z == z; }

static int g_copies = 0;
struct CWideString { const unsigned short* PStringData; CWideString(const unsigned short* s) : PStringData(s) { } CWideString(const CWideString& o) : PStringData(o.PStringData) { ++g_copies; } };
struct CFontBank { int id; };
struct CEnginePrimitiveText : CEnginePrimitive {
    C3DVector Pos; float Scale; CRGBColour Colour; CWideString Text; CFontBank* PFont; bool ScaleIn3D;
    CEnginePrimitiveText(unsigned int, unsigned int, const C3DVector& p, float scale, const CWideString& t, const CFontBank& f, const CRGBColour& c, bool s3d, unsigned int interp)
        : CEnginePrimitive(5, interp), Pos(p), Scale(scale), Colour(c), Text(t), PFont(const_cast<CFontBank*>(&f)), ScaleIn3D(s3d) { }
};
int main()
{
    static const unsigned short hello[] = { 'h', 'i', 0 };
    CFontBank font = { 3 };
    CEnginePrimitiveText t(0, 0, C3DVector(1, 2, 3), 0.75f, CWideString(hello), font, CRGBColour(1, 1, 2, 255), true, 6);
    CHECK(Base(t, 5, 6) && Vec(t.Pos, 1, 2, 3) && t.Scale == 0.75f && Col(t.Colour, 1, 1, 2, 255));
    CHECK(t.Text.PStringData == hello && g_copies == 1 && t.PFont == &font && t.ScaleIn3D);
    std::printf("004c3120_TEST PASS\n");
    return 0;
}
