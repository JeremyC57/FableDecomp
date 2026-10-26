#include <cstdio>

class CRGBColour { public: unsigned char B, G, R, A; };
class CRGBFloatColour { public:
    float R, G, B, A;
    void Set(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }
    CRGBFloatColour(const CRGBColour& colour)
    {
        Set(colour.R * (1.0f / 255.0f), colour.G * (1.0f / 255.0f), colour.B * (1.0f / 255.0f), colour.A * (1.0f / 255.0f));
    }
};
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
static bool Run()
{
    CRGBColour c; c.B = 0; c.G = 51; c.R = 255; c.A = 102;
    CRGBFloatColour f(c);
    return Near(f.R, 1.0f) && Near(f.G, 0.2f) && Near(f.B, 0.0f) && Near(f.A, 0.4f);
}

int main()
{
    if (!Run()) { std::printf("0043e470_TEST FAIL\n"); return 1; }
    std::printf("0043e470_TEST PASS\n");
    return 0;
}