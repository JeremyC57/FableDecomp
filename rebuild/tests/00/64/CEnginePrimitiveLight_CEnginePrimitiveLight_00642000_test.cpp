// Behaviour test for 0x00642000 (see src/00642000.cpp): the constructor copied onto a portable model.
#include <cstdio>
#define CHECK(c) do { if (!(c)) { std::printf("00642000_TEST FAIL\n"); return 1; } } while (0)
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

struct CEnginePrimitiveLight : CEnginePrimitive {
    C3DVector WorldPos; CRGBColour Colour; float Radius; float SpotRadius; unsigned int LightingChannelMask;
    CEnginePrimitiveLight(unsigned int, unsigned int, const C3DVector& p, const CRGBColour& c, float r, float sr, unsigned int interp, unsigned int mask)
        : CEnginePrimitive(0x0f, interp), WorldPos(p), Colour(c), Radius(r), SpotRadius(sr), LightingChannelMask(mask) { }
};
int main()
{
    CEnginePrimitiveLight l(0, 0, C3DVector(1, 2, 3), CRGBColour(1, 2, 3, 4), 10, 5, 4, 0x80000001u);
    CHECK(Base(l, 0x0f, 4) && Vec(l.WorldPos, 1, 2, 3) && Col(l.Colour, 1, 2, 3, 4) && l.Radius == 10 && l.SpotRadius == 5);
    CHECK(l.LightingChannelMask == 0x80000001u);
    std::printf("00642000_TEST PASS\n");
    return 0;
}