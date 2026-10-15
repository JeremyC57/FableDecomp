// Behaviour test for 0x0049D000 (see src/0049d000.cpp): the constructor copied onto a portable model.
#include <cstdio>
#define CHECK(c) do { if (!(c)) { std::printf("0049d000_TEST FAIL\n"); return 1; } } while (0)
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

struct C3DBoundingSphere { C3DVector Center; float Radius; C3DBoundingSphere(const C3DVector& c, float r) : Center(c), Radius(r) { } };
struct C3DBoundingBox { C3DVector Min; C3DVector Max; C3DBoundingBox(const C3DVector& a, const C3DVector& b) : Min(a), Max(b) { } };
struct CMatrix3x4 { float m[12]; };
struct CEnginePrimitiveWeatherMask : CEnginePrimitive {
    C3DBoundingSphere BSphere; C3DBoundingBox BBox; CMatrix3x4 InvTransform; float Scale; unsigned int Flags;
    CEnginePrimitiveWeatherMask(const C3DBoundingSphere& s, const C3DBoundingBox& b, const CMatrix3x4& m, float scale, unsigned int flags, unsigned int interp)
        : CEnginePrimitive(0x1f, interp), BSphere(s), BBox(b), InvTransform(m), Scale(scale), Flags(flags) { }
};
int main()
{
    CMatrix3x4 m; for (int i = 0; i < 12; ++i) m.m[i] = (float)i;
    CEnginePrimitiveWeatherMask w(C3DBoundingSphere(C3DVector(1, 2, 3), 4), C3DBoundingBox(C3DVector(0, 0, 0), C3DVector(5, 6, 7)), m, 2, 0x11, 1);
    CHECK(Base(w, 0x1f, 1) && Vec(w.BSphere.Center, 1, 2, 3) && w.BSphere.Radius == 4 && Vec(w.BBox.Max, 5, 6, 7));
    for (int i = 0; i < 12; ++i) CHECK(w.InvTransform.m[i] == (float)i);
    CHECK(w.Scale == 2 && w.Flags == 0x11);
    std::printf("0049d000_TEST PASS\n");
    return 0;
}
