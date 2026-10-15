#include <cstdio>
// Behaviour test for 009be6c0.

struct CD3DFormatInfo { long D3DFormat, PFType, BitDepth, AlphaBits, RBits, GBits, BBits, StencilBits; bool Linear; };
static const CD3DFormatInfo g_D3DFormatInfo[] = {
    { 20, 2, 24, 0, 8, 8, 8, 0, false }, { 21, 2, 32, 8, 8, 8, 8, 0, false }, { 25, 2, 16, 1, 5, 5, 5, 0, false },
    { 26, 2, 16, 4, 4, 4, 4, 0, false }, { 60, 6, 16, 0, 8, 8, 0, 0, false }, { 61, 6, 16, 0, 5, 5, 6, 0, false },
    { 64, 6, 32, 0, 16, 16, 0, 0, false }, { 0, 0, -1, 0, 0, 0, 0, 0, false } };
struct CPixelFormat { long f; void Initialise(long format) { f = format; } };
struct CDisplayManager {
    long unsupported;
    long CheckDeviceFormat(long format) { return format == unsupported ? -1 : 0; }

    // CDisplayManager::GetBestAlphaTextureFormat: the first supported RGB format of the bit depth with R, G, B and alpha above 1 bit.
    bool GetBestAlphaTextureFormat(long bitDepth, CPixelFormat& out)
    {
        for (long i = 0; g_D3DFormatInfo[i].BitDepth != -1; ++i) {
            const CD3DFormatInfo& info = g_D3DFormatInfo[i];
            if (info.PFType == 2 && info.BitDepth == bitDepth && info.RBits > 1 && info.GBits > 1 && info.BBits > 1 && info.AlphaBits > 1
                && CheckDeviceFormat(info.D3DFormat) >= 0) { out.Initialise(info.D3DFormat); return true; }
        }
        return false;
    }
};
int main()
{
    CDisplayManager m; m.unsupported = 0; CPixelFormat f; f.f = 0;
    int ok = m.GetBestAlphaTextureFormat(16, f) && f.f == 26;       // 25 (A1R5G5B5) has a 1-bit alpha
    ok = ok && m.GetBestAlphaTextureFormat(32, f) && f.f == 21 && !m.GetBestAlphaTextureFormat(24, f);
    m.unsupported = 26; ok = ok && !m.GetBestAlphaTextureFormat(16, f);
    if (!ok) { std::printf("009be6c0_TEST FAIL\n"); return 1; }
    std::printf("009be6c0_TEST PASS\n");
    return 0;
}