// Behaviour test for CGraphicInfoFrame::CFrameData::ResetDebugMemoryInUse 0x009FD4D0 (CResource slot 9): TouchedMipmapLevel becomes
// 127 (no level touched); the neighbouring mipmap bytes and TouchedFrame are untouched.
#include <cstdio>
struct CResource { virtual ~CResource() {} virtual void ResetDebugMemoryInUse() {} };
struct CFrameData : CResource {
    signed char CurrentMipmapLevel, LoadingMipmapLevel, TouchedMipmapLevel; unsigned long TouchedFrame;
    void ResetDebugMemoryInUse() { TouchedMipmapLevel = 127; }
};
int main()
{
    CFrameData f; f.CurrentMipmapLevel = 2; f.LoadingMipmapLevel = 3; f.TouchedMipmapLevel = 0; f.TouchedFrame = 9;
    CResource& r = f; r.ResetDebugMemoryInUse();
    if (f.TouchedMipmapLevel != 127 || f.CurrentMipmapLevel != 2 || f.LoadingMipmapLevel != 3 || f.TouchedFrame != 9) { std::printf("009fd4d0_TEST FAIL\n"); return 1; }
    std::printf("009fd4d0_TEST PASS\n");
    return 0;
}