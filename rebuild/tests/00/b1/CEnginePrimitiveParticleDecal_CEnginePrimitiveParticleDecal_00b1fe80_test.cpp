// Behaviour test for 0x00B1FE80 (see src/00b1fe80.cpp): the constructor copied onto a portable model.
#include <cstdio>
#define CHECK(c) do { if (!(c)) { std::printf("00b1fe80_TEST FAIL\n"); return 1; } } while (0)
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

class CRightHandedSet { public: C3DVector Up; C3DVector Forward; CRightHandedSet(const C3DVector& u, const C3DVector& f) : Up(u), Forward(f) { } };
struct CEnginePrimitiveParticleDecal : CEnginePrimitive {
    C3DVector WorldPos; C2DVector Size; CRGBColour Colour; int DecalFrame; CRightHandedSet RHSet;
    CEnginePrimitiveParticleDecal(int frame, const C3DVector& pos, const C2DVector& size, const CRGBColour& colour, const CRightHandedSet& rh)
        : CEnginePrimitive(0x19), WorldPos(pos), Size(size), Colour(colour), DecalFrame(frame), RHSet(rh) { }
};
int main()
{
    CEnginePrimitiveParticleDecal d(7, C3DVector(1, 2, 3), C2DVector(4, 5), CRGBColour(10, 20, 30, 40),
                                    CRightHandedSet(C3DVector(0, 0, 1), C3DVector(0, 1, 0)));
    CHECK(Base(d, 0x19, 0));
    CHECK(Vec(d.WorldPos, 1, 2, 3) && d.Size.X == 4 && d.Size.Y == 5 && Col(d.Colour, 10, 20, 30, 40) && d.DecalFrame == 7);
    CHECK(Vec(d.RHSet.Up, 0, 0, 1) && Vec(d.RHSet.Forward, 0, 1, 0));
    std::printf("00b1fe80_TEST PASS\n");
    return 0;
}