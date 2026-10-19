// Behaviour test for 0x009FDCB0 CGraphicInfoFrame::CFrameData::GetDebugMemoryInUse: untouched/uninitialised frames,
// the size sum and the subtraction of the non-resident mip levels.
#include <cstdio>
static int g_uninit = 0;
class CSurface {
public:
    unsigned long bytes;
    CSurface(unsigned long b) : bytes(b) {}
    CSurface(const CSurface& o) : bytes(o.bytes) {}
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise() { ++g_uninit; }
    unsigned long GetByteLength() const { return bytes; }
};
class CTexture {
public:
    long levels; unsigned long top;
    long GetNoLevels() const { return levels; }
    CSurface GetMipmapSurface(long level) const { return CSurface(level == 0 ? top : 0); }
};
class CGraphicFrameData {
public:
    CTexture* Texture;
    unsigned long Sizeof() const { return 100; }
};
class CGraphicFrame {
public:
    CGraphicFrameData* FrameData;
    CTexture* GetTexture() const { return FrameData->Texture; }
    unsigned long Sizeof() const { return FrameData ? FrameData->Sizeof() + 4 : 4; }
};
class CManagedTexture {
public:
    void* Data;
    unsigned long Sizeof() const { return 1000; }
};
template <class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }
class CGraphicInfoFrame {
public:
    signed char SmallMipmapLevel;
    class CFrameData {
    public:
        CGraphicFrame Frame;
        CManagedTexture LoadingManagedTexture;
        signed char TouchedMipmapLevel;
        CGraphicInfoFrame* GraphicInfoFrame;
        unsigned long Sizeof() const
        {
            unsigned long size = Frame.Sizeof() + 0x44;
            if (LoadingManagedTexture.Data)
                size += LoadingManagedTexture.Sizeof();
            return size;
        }
        long GetDebugMemoryInUse() const;
    };
};
long CGraphicInfoFrame::CFrameData::GetDebugMemoryInUse() const
{
    if (TouchedMipmapLevel == 0x7f || Frame.FrameData == 0)
        return 0;
    long memory = Sizeof();
    long level = Min<long>(TouchedMipmapLevel, GraphicInfoFrame->SmallMipmapLevel);
    level = Min<long>(level, Frame.GetTexture()->GetNoLevels());
    long levelSize = Frame.GetTexture()->GetMipmapSurface(0).GetByteLength();
    for (; level > 0; --level) {
        memory -= levelSize;
        levelSize /= 4;
    }
    return memory;
}
int main()
{
    CTexture tex = { 5, 4096 };
    CGraphicFrameData fd = { &tex };
    CGraphicInfoFrame gif; gif.SmallMipmapLevel = 3;
    CGraphicInfoFrame::CFrameData d; d.Frame.FrameData = 0; d.LoadingManagedTexture.Data = 0; d.TouchedMipmapLevel = 2; d.GraphicInfoFrame = &gif;
    int fail = 0;
    if (d.GetDebugMemoryInUse() != 0) fail |= 1;                 // no frame data
    d.Frame.FrameData = &fd; d.TouchedMipmapLevel = 0x7f;
    if (d.GetDebugMemoryInUse() != 0) fail |= 2;                 // never touched
    d.TouchedMipmapLevel = 2;                                    // level 2: minus 4096 and 1024
    if (d.GetDebugMemoryInUse() != (long)(104 + 0x44) - 4096 - 1024 || g_uninit != 1) fail |= 4;
    d.LoadingManagedTexture.Data = &tex; d.TouchedMipmapLevel = 9; tex.levels = 1;   // capped by the texture's levels
    if (d.GetDebugMemoryInUse() != (long)(104 + 0x44 + 1000) - 4096) fail |= 8;
    d.TouchedMipmapLevel = 0;
    if (d.GetDebugMemoryInUse() != (long)(104 + 0x44 + 1000)) fail |= 16;
    if (fail) { std::printf("009fdcb0_TEST FAIL\n"); return 1; }
    std::printf("009fdcb0_TEST PASS\n");
    return 0;
}