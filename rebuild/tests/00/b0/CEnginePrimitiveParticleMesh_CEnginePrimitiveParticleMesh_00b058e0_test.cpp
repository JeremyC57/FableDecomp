// Behaviour test for 0x00B058E0 (see src/00b058e0.cpp): the constructor copied onto a portable model.
#include <cstdio>
#define CHECK(c) do { if (!(c)) { std::printf("00b058e0_TEST FAIL\n"); return 1; } } while (0)
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

struct CRightHandedSet { C3DVector Up; C3DVector Forward; CRightHandedSet(const C3DVector& u, const C3DVector& f) : Up(u), Forward(f) { } };
struct CEnginePrimitiveParticleMesh : CEnginePrimitive {
    C3DVector WorldPos; C3DVector Size; CRightHandedSet RightHandedSet; CRGBColour Colour;
    CEnginePrimitiveParticleMesh(unsigned int, unsigned int, const C3DVector& pos, const CRightHandedSet& rh, const C3DVector& size,
                                 const CRGBColour& c, unsigned int interp)
        : CEnginePrimitive(0x17, interp), WorldPos(pos), Size(size), RightHandedSet(rh), Colour(c) { }
};
int main()
{
    CEnginePrimitiveParticleMesh m(0, 0, C3DVector(1, 2, 3), CRightHandedSet(C3DVector(4, 5, 6), C3DVector(7, 8, 9)),
                                   C3DVector(2, 2, 2), CRGBColour(1, 2, 3, 4), 3);
    CHECK(Base(m, 0x17, 3) && Vec(m.WorldPos, 1, 2, 3) && Vec(m.Size, 2, 2, 2));
    CHECK(Vec(m.RightHandedSet.Up, 4, 5, 6) && Vec(m.RightHandedSet.Forward, 7, 8, 9) && Col(m.Colour, 1, 2, 3, 4));
    std::printf("00b058e0_TEST PASS\n");
    return 0;
}