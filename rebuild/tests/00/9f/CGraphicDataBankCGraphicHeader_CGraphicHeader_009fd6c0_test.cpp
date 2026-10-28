#include <cstdio>
#include <cstring>
#include <new>
// Behaviour test for CGraphicDataBank::CGraphicHeader::CGraphicHeader (0x009FD6C0).
class CPixelFormat { public: int FormatIndex; CPixelFormat() : FormatIndex(-1) {} };
class CGraphicHeader {
public:
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount;
    CPixelFormat PixelFormat;
    unsigned char TransparencyType, MipmapLevels;
    unsigned char IsFlatAnimatedGraphic : 1;
    unsigned char OtherBits : 7;
    unsigned int FrameDataSize;
    unsigned int CompressedMipmapSizes[1];
    CGraphicHeader()
        : Width(0), Height(0), Depth(0), FrameWidth(0), FrameHeight(0), FrameCount(1), PixelFormat(),
          TransparencyType(0), MipmapLevels(0), IsFlatAnimatedGraphic(false), FrameDataSize(0)
    {
        CompressedMipmapSizes[0] = 0;
    }
};
int main()
{
    unsigned char buf[sizeof(CGraphicHeader)];
    std::memset(buf, 0xAB, sizeof buf);
    CGraphicHeader* h = new (buf) CGraphicHeader;
    bool ok = h->Width == 0 && h->Height == 0 && h->Depth == 0 && h->FrameWidth == 0 && h->FrameHeight == 0
           && h->FrameCount == 1 && h->PixelFormat.FormatIndex == -1 && h->TransparencyType == 0 && h->MipmapLevels == 0
           && h->IsFlatAnimatedGraphic == 0 && h->FrameDataSize == 0 && h->CompressedMipmapSizes[0] == 0;
    if (!ok) { std::printf("009fd6c0_TEST FAIL\n"); return 1; }
    std::printf("009fd6c0_TEST PASS\n");
    return 0;
}