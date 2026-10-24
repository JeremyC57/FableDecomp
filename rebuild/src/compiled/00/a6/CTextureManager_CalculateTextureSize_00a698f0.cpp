// CTextureManager::CalculateTextureSize(unsigned long width, unsigned long height, bool mipmaps, const CPixelFormat& format) -- bytes needed for a texture: DXT1 = 4 bits/pixel and DXT3/DXT5 = 8 over 4x4 minimum blocks, otherwise the format's colour depth over 1x1; with mipmaps every level down to 1x1 is summed
// Leaf: ?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A698F0, 136 bytes (+8 int3), static __fastcall (width ecx, height edx,
//   mipmaps and format on the stack), `ret 8`.
// Callees: CPixelFormat::GetColourDepth() const @ 0x009E3820. Data: the 0x24-byte pixel-format table @ 0x0129BA40 (D3DFORMAT
//   first; the DXTn D3DFORMATs are their FourCCs 'DXT1' 0x31545844, 'DXT3' 0x33545844, 'DXT5' 0x35545844).
// Label: the manifest's `CalculateTextureSize` is right; FableWin.pdb `?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z`.
// Shape: a switch on the D3DFORMAT (an if-chain lays the DXT1 arm out first; retail has it last), then
//   `while (width > 0 || height > 0) { size += max(width, min) * max(height, min); if (!mipmaps) break; halve }`,
//   then `size * bitsPerPixel / 8` (unsigned).
struct SPixelFormatInfo { long d3dFormat; long rest[8]; };   // 0x24 bytes
extern SPixelFormatInfo g_pixelFormatInfo[];                 // 0x0129BA40

class CPixelFormat {
public:
    long FormatIndex;                                        // +0x00
    unsigned long GetColourDepth() const;                    // 0x009E3820
};

class CTextureManager {
public:
    static unsigned long __fastcall CalculateTextureSize(unsigned long width, unsigned long height, bool mipmaps,
                                                         const CPixelFormat& format);
};

unsigned long __fastcall CTextureManager::CalculateTextureSize(unsigned long width, unsigned long height, bool mipmaps,
                                                               const CPixelFormat& format)
{
    unsigned long minSize = 1;
    unsigned long bitsPerPixel;
    switch (g_pixelFormatInfo[format.FormatIndex].d3dFormat) {
    case 0x31545844:                                         // 'DXT1'
        bitsPerPixel = 4;
        minSize = 4;
        break;
    case 0x33545844:                                         // 'DXT3'
    case 0x35545844:                                         // 'DXT5'
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