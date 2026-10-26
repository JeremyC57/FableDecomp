#include <cstdio>
// behaviour copy of CEnginePrimitiveManagerMeshBase::GetAcceptablePreloadMipmap @ 0x00BEBBE0
struct CGraphicHeader { unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount; };
class CGraphicInfo { public: CGraphicHeader Header; };
class CGraphicInfoFrame { public: void* vt; unsigned short FrameNum; signed char Small; void* Data; CGraphicInfo* GraphicInfo; };
struct SPrimitiveRendererSettings { unsigned long PreloadMipmapBias; unsigned long MaxPreloadTextureSize; };
static SPrimitiveRendererSettings g_settings;
static SPrimitiveRendererSettings* g_pPrimitiveRenderer = &g_settings;

static unsigned long GetAcceptablePreloadMipmap(unsigned long mipmap, const CGraphicInfoFrame* frame)
{
    unsigned long size;
    if (frame->GraphicInfo->Header.FrameWidth > frame->GraphicInfo->Header.FrameHeight)
        size = frame->GraphicInfo->Header.FrameWidth;
    else
        size = frame->GraphicInfo->Header.FrameHeight;
    size >>= mipmap;
    unsigned long level = mipmap;
    while (size > g_pPrimitiveRenderer->MaxPreloadTextureSize) {
        size >>= 1;
        ++level;
    }
    return g_pPrimitiveRenderer->PreloadMipmapBias + level;
}

int main()
{
    CGraphicInfo info = { { 0, 0, 0, 1024, 256, 1 } };
    CGraphicInfoFrame frame = { 0, 0, 0, 0, &info };
    g_settings.PreloadMipmapBias = 0;
    g_settings.MaxPreloadTextureSize = 256;
    int ok = 1;
    if (GetAcceptablePreloadMipmap(0, &frame) != 2) ok = 0;        // 1024 -> 512 -> 256
    if (GetAcceptablePreloadMipmap(3, &frame) != 3) ok = 0;        // 128 already fits
    g_settings.PreloadMipmapBias = 1;
    info.Header.FrameWidth = 64; info.Header.FrameHeight = 2048;   // the taller side wins
    if (GetAcceptablePreloadMipmap(1, &frame) != 1 + 3) ok = 0;    // 1024 -> 512 -> 256: level 3, plus bias
    if (!ok) { std::printf("00bebbe0_TEST FAIL\n"); return 1; }
    std::printf("00bebbe0_TEST PASS\n");
    return 0;
}