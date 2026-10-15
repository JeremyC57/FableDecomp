// Behaviour test for 0x00AF08D0 (see src/00af08d0.cpp): the constructor copied onto a portable model.
#include <cstdio>
#define CHECK(c) do { if (!(c)) { std::printf("00af08d0_TEST FAIL\n"); return 1; } } while (0)
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
struct CEnginePrimitiveParticleGroup : CEnginePrimitive {
    float DistanceFromCam; bool ScreenDisplacement; bool ReadZBuffer; unsigned int WaterStage; bool WaterEffect;
    bool ZBufferWritable; bool ClipEffectToWeatherMask; bool EnableDithering; C3DBoundingSphere BSphere;
    CEnginePrimitiveParticleGroup(float dist, const C3DBoundingSphere& s, bool sd, bool rz, unsigned int ws, bool zw, bool clip, bool dith)
        : CEnginePrimitive(0x18), DistanceFromCam(dist), ScreenDisplacement(sd), ReadZBuffer(rz), WaterStage(ws),
          ZBufferWritable(zw), ClipEffectToWeatherMask(clip), EnableDithering(dith), BSphere(s) { }
};
int main()
{
    CEnginePrimitiveParticleGroup g(12.5f, C3DBoundingSphere(C3DVector(1, 2, 3), 9), true, false, 2, true, false, true);
    CHECK(Base(g, 0x18, 0) && g.DistanceFromCam == 12.5f && g.ScreenDisplacement && !g.ReadZBuffer && g.WaterStage == 2);
    CHECK(g.ZBufferWritable && !g.ClipEffectToWeatherMask && g.EnableDithering);
    CHECK(Vec(g.BSphere.Center, 1, 2, 3) && g.BSphere.Radius == 9);
    std::printf("00af08d0_TEST PASS\n");
    return 0;
}