// Behaviour test for 0x00A698F0 CTextureManager::CalculateTextureSize(width, height, mipmaps, format)
#include <cstdio>

struct SPixelFormatInfo { long d3dFormat; long rest[8]; };
static SPixelFormatInfo g_pixelFormatInfo[4] = {
    { 0x31545844L, { 0 } },                 // 'DXT1'
    { 0x33545844L, { 0 } },                 // 'DXT3'
    { 0x35545844L, { 0 } },                 // 'DXT5'
    { 21L, { 32 } },                        // D3DFMT_A8R8G8B8, 32 bits (rest[0] holds the stand-in depth)
};

class CPixelFormat {
public:
    long FormatIndex;
    unsigned long GetColourDepth() const { return (unsigned long)g_pixelFormatInfo[FormatIndex].rest[0]; }   // 0x009E3820 stand-in
};

class CTextureManager {
public:
    static unsigned long CalculateTextureSize(unsigned long width, unsigned long height, bool mipmaps,
                                              const CPixelFormat& format)
    {
        unsigned long minSize = 1;
        unsigned long bitsPerPixel;
        switch (g_pixelFormatInfo[format.FormatIndex].d3dFormat) {
        case 0x31545844:
            bitsPerPixel = 4;
            minSize = 4;
            break;
        case 0x33545844:
        case 0x35545844:
            bitsPerPixel = 8;
            minSize = 4;
            break;
        default:
            bitsPerPixel = format.GetColourDepth();
            break;
        }
        unsigned long size = 0;
        while (width > 0 || height > 0) {
            unsigned long w = width > minSize ? width : minSize;
            unsigned long h = height > minSize ? height : minSize;
            size += w * h;
            if (!mipmaps)
                break;
            width >>= 1;
            height >>= 1;
        }
        return size * bitsPerPixel / 8;
    }
};

int main()
{
    CPixelFormat dxt1, dxt5, argb;
    dxt1.FormatIndex = 0;
    dxt5.FormatIndex = 2;
    argb.FormatIndex = 3;
    bool ok = CTextureManager::CalculateTextureSize(256, 256, false, dxt1) == 32768 &&
              CTextureManager::CalculateTextureSize(256, 256, true, dxt1) == 43704 &&     // levels down to 1x1 on 4x4 blocks
              CTextureManager::CalculateTextureSize(2, 2, false, dxt5) == 16 &&           // one 4x4 block
              CTextureManager::CalculateTextureSize(64, 32, false, argb) == 8192 &&
              CTextureManager::CalculateTextureSize(4, 2, true, argb) == 44;              // (8 + 2 + 1) * 4
    if (!ok) { std::printf("00a698f0_TEST FAIL\n"); return 1; }
    std::printf("00a698f0_TEST PASS\n");
    return 0;
}