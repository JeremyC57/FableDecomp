// Behaviour test for 0x00AFE220 (see src/00afe220.cpp): the constructor copied onto a portable model.
#include <cstdio>
#define CHECK(c) do { if (!(c)) { std::printf("00afe220_TEST FAIL\n"); return 1; } } while (0)
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
class CRGBColour { public:                                      // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); }
};
class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
class CEnginePrimitive { public:
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

struct CEnginePrimitiveParticleLine : CEnginePrimitive {
    C3DVector Start; C3DVector End; CRGBColour Colour; float Width;
    CEnginePrimitiveParticleLine(unsigned int, unsigned int, const C3DVector& s, const C3DVector& e, const CRGBColour& c, float w, unsigned int interp)
        : CEnginePrimitive(0x15, interp), Start(s), End(e), Colour(c), Width(w) { }
};
int main()
{
    CEnginePrimitiveParticleLine l(0, 0, C3DVector(1, 2, 3), C3DVector(4, 5, 6), CRGBColour(255, 0, 128, 64), 2.5f, 2);
    CHECK(Base(l, 0x15, 2) && Vec(l.Start, 1, 2, 3) && Vec(l.End, 4, 5, 6) && Col(l.Colour, 255, 0, 128, 64) && l.Width == 2.5f);
    std::printf("00afe220_TEST PASS\n");
    return 0;
}