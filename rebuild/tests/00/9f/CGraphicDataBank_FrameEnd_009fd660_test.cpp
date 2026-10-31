// Behaviour test for CGraphicDataBank::FrameEnd (0x009FD660): frozen -> nothing; else bank FrameEnd, texture FrameEnd, then
// AvailableMemory = max(Maximum - Allocated, 0) + unloadable memory for ReduceSizeFrameDelay.
#include <cstdio>
template <class T> inline T Max(T a, T b) { return a > b ? a : b; }
static int g_calls[3];
class CResourceList {
public:
    long AllocatedMemory, MaximumMemory;
    unsigned long Unloadable;
    unsigned long GetUnloadableMemory(unsigned long minUnusedFrames) const { g_calls[2]++; return Unloadable + minUnusedFrames; }
    long GetFreeMemory() const { return MaximumMemory - AllocatedMemory; }
};
class CGraphicDataBank {
public:
    CResourceList ResourceList;
    unsigned long ReduceSizeFrameDelay;
    long AvailableMemory;
    bool FreezeGraphics;
    void ResourceBankFrameEnd() { g_calls[0]++; }
    void TextureManagerFrameEnd() { g_calls[1]++; }
    void UpdateAvailableMemory()
    {
        AvailableMemory = Max(ResourceList.GetFreeMemory(), 0L);
        AvailableMemory += ResourceList.GetUnloadableMemory(ReduceSizeFrameDelay);
    }
    void FrameEnd()
    {
        if (!FreezeGraphics) {
            ResourceBankFrameEnd();
            TextureManagerFrameEnd();
            UpdateAvailableMemory();
        }
    }
};
int main()
{
    CGraphicDataBank b;
    b.ResourceList.AllocatedMemory = 100; b.ResourceList.MaximumMemory = 160; b.ResourceList.Unloadable = 30;
    b.ReduceSizeFrameDelay = 2; b.AvailableMemory = -1; b.FreezeGraphics = true;
    b.FrameEnd();
    bool ok = b.AvailableMemory == -1 && g_calls[0] == 0 && g_calls[1] == 0 && g_calls[2] == 0;
    b.FreezeGraphics = false;
    b.FrameEnd();
    ok = ok && b.AvailableMemory == 60 + 32 && g_calls[0] == 1 && g_calls[1] == 1 && g_calls[2] == 1;
    b.ResourceList.AllocatedMemory = 200;           // over budget: free memory clamps to 0
    b.FrameEnd();
    ok = ok && b.AvailableMemory == 32;
    b.ResourceList.AllocatedMemory = 160;           // exactly full: 0
    b.FrameEnd();
    ok = ok && b.AvailableMemory == 32;
    if (!ok) { std::printf("009fd660_TEST FAIL\n"); return 1; }
    std::printf("009fd660_TEST PASS\n");
    return 0;
}