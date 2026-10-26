#include <cstdio>
// Behaviour test for 009be590.

struct CD3DFormatInfo { long D3DFormat, PFType, BitDepth, AlphaBits, RBits, GBits, BBits, StencilBits; bool Linear; };
static const CD3DFormatInfo g_D3DFormatInfo[] = {
    { 20, 2, 24, 0, 8, 8, 8, 0, false }, { 21, 2, 32, 8, 8, 8, 8, 0, false }, { 25, 2, 16, 1, 5, 5, 5, 0, false },
    { 26, 2, 16, 4, 4, 4, 4, 0, false }, { 60, 6, 16, 0, 8, 8, 0, 0, false }, { 61, 6, 16, 0, 5, 5, 6, 0, false },
    { 64, 6, 32, 0, 16, 16, 0, 0, false }, { 0, 0, -1, 0, 0, 0, 0, 0, false } };
class CPixelFormat { public: long f; void Initialise(long format) { f = format; } };
class CDisplayManager { public:
    long unsupported;
    long CheckDeviceFormat(long format) { return format == unsupported ? -1 : 0; }

    // CDisplayManager::GetBestSignedTextureFormat: the first supported signed (type 6) format of the bit depth.
    bool GetBestSignedTextureFormat(long bitDepth, CPixelFormat& out)
    {
        const CD3DFormatInfo* best = 0;
        for (long i = 0; g_D3DFormatInfo[i].BitDepth != -1; ++i) {
            const CD3DFormatInfo& info = g_D3DFormatInfo[i];
            if (info.PFType == 6 && info.BitDepth == bitDepth && best == 0 && CheckDeviceFormat(info.D3DFormat) >= 0) best = &info;
        }
        if (best != 0) { out.Initialise(best->D3DFormat); return true; }
        return false;
    }
};
int main()
{
    CDisplayManager m; m.unsupported = 60; CPixelFormat f; f.f = 0;
    int ok = m.GetBestSignedTextureFormat(16, f) && f.f == 61;
    m.unsupported = 0; ok = ok && m.GetBestSignedTextureFormat(16, f) && f.f == 60;
    ok = ok && m.GetBestSignedTextureFormat(32, f) && f.f == 64 && !m.GetBestSignedTextureFormat(8, f);
    if (!ok) { std::printf("009be590_TEST FAIL\n"); return 1; }
    std::printf("009be590_TEST PASS\n");
    return 0;
}