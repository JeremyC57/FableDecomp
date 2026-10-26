// Behaviour test for 0x009A0EC0 CStreamingFontBank::SetAlphaMaskUVs: both UV pairs are copied into the statics.
#include <cstdio>

class C2DVector { public: float X, Y; };
static C2DVector g_StreamingFontAlphaMaskUV0;
static C2DVector g_StreamingFontAlphaMaskUV1;

struct CStreamingFontBank {
    static void SetAlphaMaskUVs(const C2DVector& uv0, const C2DVector& uv1)
    {
        g_StreamingFontAlphaMaskUV0 = uv0;
        g_StreamingFontAlphaMaskUV1 = uv1;
    }
};

static int Near(float a, float b) { float d = a - b; return d < 1e-6f && d > -1e-6f; }

int main()
{
    C2DVector a = { 0.25f, 0.5f };
    C2DVector b = { 0.75f, 1.0f };
    CStreamingFontBank::SetAlphaMaskUVs(a, b);
    if (!Near(g_StreamingFontAlphaMaskUV0.X, 0.25f) || !Near(g_StreamingFontAlphaMaskUV0.Y, 0.5f) ||
        !Near(g_StreamingFontAlphaMaskUV1.X, 0.75f) || !Near(g_StreamingFontAlphaMaskUV1.Y, 1.0f)) {
        std::printf("009a0ec0_TEST FAIL\n");
        return 1;
    }
    std::printf("009a0ec0_TEST PASS\n");
    return 0;
}