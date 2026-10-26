// Behaviour test for 0x008860d0 CEnvironmentLookupTexture::LookupFloatColour
#include <cstdio>
class CRGBFloatColour { public: float R, G, B, A; CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {} };
struct CEnvironmentLookupTexture {
    long m_pitch; unsigned char* m_pData;
    static float ByteToFloat(unsigned char c) { return (float)((c + 0.5) * (1.0 / 255.0)); }
    CRGBFloatColour LookupFloatColour(long x, long y) const
    {
        const unsigned char* texel = m_pData + (m_pitch * y + x * 4);
        return CRGBFloatColour(ByteToFloat(texel[2]), ByteToFloat(texel[1]), ByteToFloat(texel[0]), ByteToFloat(texel[3]));
    }
};
static bool Near(float a, float b) { float d = a - b; return d < 1e-6f && d > -1e-6f; }
int main()
{
    unsigned char data[2 * 12];
    for (int i = 0; i < 24; ++i) data[i] = 0;
    unsigned char* t = data + 12 + 4;          // row 1, texel 1 (pitch 12)
    t[0] = 0; t[1] = 127; t[2] = 254; t[3] = 255;
    CEnvironmentLookupTexture tex; tex.m_pitch = 12; tex.m_pData = data;
    CRGBFloatColour c = tex.LookupFloatColour(1, 1);
    bool ok = Near(c.R, 254.5f / 255.0f) && Near(c.G, 127.5f / 255.0f) && Near(c.B, 0.5f / 255.0f) && Near(c.A, 255.5f / 255.0f);
    if (!ok) { std::printf("008860d0_TEST FAIL\n"); return 1; }
    std::printf("008860d0_TEST PASS\n");
    return 0;
}