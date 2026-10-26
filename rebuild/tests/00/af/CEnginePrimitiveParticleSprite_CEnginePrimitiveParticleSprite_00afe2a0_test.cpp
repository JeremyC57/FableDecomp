// Behaviour test for 0x00AFE2A0 (see src/00afe2a0.cpp): the constructor copied onto a portable model.
#include <cstdio>
#define CHECK(c) do { if (!(c)) { std::printf("00afe2a0_TEST FAIL\n"); return 1; } } while (0)
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

struct CEnginePrimitiveParticleSprite : CEnginePrimitive {
    unsigned int Frame; C3DVector WorldPos; C2DVector Size; float Angle; CRGBColour Colour;
    CEnginePrimitiveParticleSprite(unsigned int, unsigned int, unsigned int frame, const C3DVector& pos, const C2DVector& size, float angle,
                                   const CRGBColour& c, unsigned int interp)
        : CEnginePrimitive(0x16, interp), Frame(frame), WorldPos(pos), Size(size), Angle(angle), Colour(c) { }
};
int main()
{
    CEnginePrimitiveParticleSprite s(0, 0, 11, C3DVector(1, 2, 3), C2DVector(5, 6), 0.5f, CRGBColour(9, 8, 7, 6), 1);
    CHECK(Base(s, 0x16, 1) && s.Frame == 11 && Vec(s.WorldPos, 1, 2, 3) && s.Size.X == 5 && s.Size.Y == 6);
    CHECK(s.Angle == 0.5f && Col(s.Colour, 9, 8, 7, 6));
    std::printf("00afe2a0_TEST PASS\n");
    return 0;
}