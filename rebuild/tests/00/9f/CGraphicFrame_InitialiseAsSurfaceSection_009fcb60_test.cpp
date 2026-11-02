// Behaviour test for CGraphicFrame::InitialiseAsSurfaceSection (0x009FCB60): a fresh frame data block (reference count 1, null counted texture,
// IsInitialised clear) is stored in the frame before the block's own initialiser runs with the forwarded arguments.
#include <cstdio>
#include <cstdlib>
#include <new>
class CTexture { public: int Id; };
class CCPPointerInfo { public: unsigned int RefCount; void* DeleteFunc; void* Data; };
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CGraphicFrameData;
class CGraphicFrame { public:
    CGraphicFrameData* FrameData;
    bool InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float tlx, float tly, long width, long height, bool alpha, bool booleanAlpha);
};
#pragma pack(push, 1)
class CGraphicFrameData { public:
    unsigned int ReferenceCount; void* Data; CCountedPointer<CTexture> PTexture;
    unsigned short TLX, TLY, Width, Height, Pitch;
    unsigned char Alpha : 1; unsigned char BooleanAlpha : 1; unsigned char UsesSurface : 1;
    unsigned char SectionOfTPage : 1; unsigned char IsInitialised : 1; unsigned char ScreenGraphic : 1;
    CGraphicFrameData() : ReferenceCount(1) { IsInitialised = false; }
    bool InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float tlx, float tly, long width, long height, bool alpha, bool booleanAlpha);
};
#pragma pack(pop)
static CGraphicFrame* gFrame; static int gCalls; static bool gSawFresh; static long gW, gH; static bool gA, gB; static float gX, gY;
static const CCountedPointer<CTexture>* gTex;
bool CGraphicFrameData::InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float tlx, float tly, long width, long height, bool alpha, bool booleanAlpha)
{
    ++gCalls; gTex = &texture; gW = width; gH = height; gA = alpha; gB = booleanAlpha; gX = tlx; gY = tly;
    gSawFresh = (gFrame->FrameData == this) && ReferenceCount == 1 && PTexture.Data == 0 && PTexture.Info == 0 && !IsInitialised;
    IsInitialised = true; Width = (unsigned short)width; Height = (unsigned short)height;
    return width > 0;
}
bool CGraphicFrame::InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float tlx, float tly, long width, long height, bool alpha, bool booleanAlpha)
{
    FrameData = new CGraphicFrameData;
    return FrameData->InitialiseAsSurfaceSection(texture, tlx, tly, width, height, alpha, booleanAlpha);
}
int main()
{
    if (sizeof(void*) == 4 && sizeof(CGraphicFrameData) != 27) { std::printf("009fcb60_TEST FAIL\n"); return 1; }
    void* raw = std::malloc(sizeof(CGraphicFrameData)); unsigned char* b = (unsigned char*)raw;
    for (unsigned int i = 0; i < sizeof(CGraphicFrameData); ++i) b[i] = 0xff;      // ctor must clear only its own members
    CGraphicFrameData* d = new (raw) CGraphicFrameData;
    if (d->ReferenceCount != 1 || d->PTexture.Data || d->PTexture.Info || d->IsInitialised || !d->Alpha || !d->ScreenGraphic) {
        std::printf("009fcb60_TEST FAIL\n"); return 1; }
    std::free(raw);
    CTexture tex; tex.Id = 7; CCountedPointer<CTexture> p; p.Data = &tex;
    CGraphicFrame frame; frame.FrameData = 0; gFrame = &frame;
    bool r = frame.InitialiseAsSurfaceSection(p, 0.25f, 0.5f, 64, 32, true, false);
    if (!r || gCalls != 1 || !gSawFresh || gTex != &p || gW != 64 || gH != 32 || !gA || gB || !frame.FrameData
        || !frame.FrameData->IsInitialised || frame.FrameData->Width != 64 || gX < 0.24f || gX > 0.26f || gY < 0.49f || gY > 0.51f) { std::printf("009fcb60_TEST FAIL\n"); return 1; }
    CGraphicFrameData* first = frame.FrameData;
    r = frame.InitialiseAsSurfaceSection(p, 0.25f, 0.5f, 0, 8, false, true);
    if (r || gCalls != 2 || frame.FrameData == first || gW != 0 || gA || !gB) { std::printf("009fcb60_TEST FAIL\n"); return 1; }
    delete first; delete frame.FrameData;
    std::printf("009fcb60_TEST PASS\n");
    return 0;
}