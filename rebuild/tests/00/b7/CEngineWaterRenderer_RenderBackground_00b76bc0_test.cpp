// Behaviour test for CEngineWaterRenderer::RenderBackground @ 0x00B76BC0 (restated with fakes): shader choice,
// high-end bump constant or low-end static colour constants, list order.
#include <cstdio>
#include <cstring>

class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
static CRGBFloatColour Mul(const CRGBFloatColour& a, const CRGBFloatColour& b) { return CRGBFloatColour(a.R * b.R, a.G * b.G, a.B * b.B, a.A * b.A); }

struct Out { int highShaders, lowShaders; float c3[4]; float ps5[4]; float ps6[4]; char order[64]; };

static void RenderBackground(bool highEnd, float bumpFactor, float bumpScale, const CRGBFloatColour& ambient, Out& o)
{
    if (highEnd) ++o.highShaders; else ++o.lowShaders;
    if (highEnd) {
        float bump = bumpFactor * bumpScale;
        o.c3[0] = bump; o.c3[1] = 0.0f; o.c3[2] = bump * -0.5f; o.c3[3] = 1.0f;
    } else {
        static CRGBFloatColour lowEndConstant(0.8f, 0.8f, 0.0f, 0.0f);
        static CRGBFloatColour lowEndColour(0.0f, 0.5f, 0.75f, 1.0f);
        CRGBFloatColour colour = Mul(lowEndColour, ambient);
        o.ps5[0] = lowEndConstant.R; o.ps5[1] = lowEndConstant.G; o.ps5[2] = lowEndConstant.B; o.ps5[3] = lowEndConstant.A;
        o.ps6[0] = colour.R; o.ps6[1] = colour.G; o.ps6[2] = colour.B; o.ps6[3] = colour.A;
    }
    std::strcat(o.order, "alphaed;opaque;");
}

int main()
{
    int fails = 0;
    Out o; std::memset(&o, 0, sizeof o);
    CRGBFloatColour amb(2.0f, 2.0f, 2.0f, 0.5f);
    RenderBackground(true, 2.0f, 4.0f, amb, o);
    if (o.highShaders != 1 || o.c3[0] != 8.0f || o.c3[2] != -4.0f || o.c3[3] != 1.0f) ++fails;
    RenderBackground(false, 2.0f, 4.0f, amb, o);
    if (o.lowShaders != 1 || o.ps5[0] != 0.8f || o.ps6[1] != 1.0f || o.ps6[2] != 1.5f || o.ps6[3] != 0.5f) ++fails;
    if (std::strcmp(o.order, "alphaed;opaque;alphaed;opaque;") != 0) ++fails;
    if (fails == 0) std::printf("00b76bc0_TEST PASS\n"); else std::printf("00b76bc0_TEST FAIL %d\n", fails);
    return fails;
}