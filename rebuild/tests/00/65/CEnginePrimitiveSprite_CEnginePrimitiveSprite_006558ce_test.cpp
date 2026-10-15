// Behaviour test for 0x006558CE (see src/006558ce.cpp): the constructor copied onto a portable model.
#include <cstdio>
#define CHECK(c) do { if (!(c)) { std::printf("006558ce_TEST FAIL\n"); return 1; } } while (0)
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

struct CSpriteColour { unsigned char B, G, R, A; CSpriteColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct C2DValue { float X, Y; C2DValue(float x, float y) : X(x), Y(y) { } };
struct CEnginePrimitiveSprite : CEnginePrimitive {
    C3DVector WorldPos; unsigned int BankIndex; unsigned int Frame; float SizeX; float SizeY; float FadeStart; float Angle;
    CSpriteColour Colour; unsigned short Flags; unsigned int BlendMode; unsigned int BlendOp; C2DValue Offset;
    CEnginePrimitiveSprite(const C3DVector& p, unsigned int bank, unsigned int frame, unsigned short flags, float angle, float sx, float sy,
                           unsigned int mode, unsigned char a, unsigned char r, unsigned char g, unsigned char b, float fade, unsigned int interp, unsigned int op)
        : CEnginePrimitive(7, interp), WorldPos(p), BankIndex(bank), Frame(frame), SizeX(sx), SizeY(sy), FadeStart(fade), Angle(angle),
          Colour(r, g, b, a), Flags(flags), BlendMode(mode), BlendOp(op), Offset(0.0f, 0.0f) { }
};
int main()
{
    CEnginePrimitiveSprite s(C3DVector(1, 2, 3), 4, 5, 0x1234, 0.25f, 8, 9, 2, 200, 10, 20, 30, 0.5f, 3, 1);
    CHECK(Base(s, 7, 3) && Vec(s.WorldPos, 1, 2, 3) && s.BankIndex == 4 && s.Frame == 5 && s.Flags == 0x1234);
    CHECK(s.Angle == 0.25f && s.SizeX == 8 && s.SizeY == 9 && s.BlendMode == 2 && s.FadeStart == 0.5f && s.BlendOp == 1);
    CHECK(s.Colour.A == 200 && s.Colour.R == 10 && s.Colour.G == 20 && s.Colour.B == 30 && s.Offset.X == 0 && s.Offset.Y == 0);
    std::printf("006558ce_TEST PASS\n");
    return 0;
}