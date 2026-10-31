// Behaviour test for 0x009FD3A0 CGraphicDataBank::UpdateAvailableMemory:
// AvailableMemory = Max(MaximumMemory - AllocatedMemory, 0) + ResourceList.GetUnloadableMemory(ReduceSizeFrameDelay).
#include <cstdio>
template <class T> inline T Max(T a, T b) { return a > b ? a : b; }
class CResourceList {
public:
    long AllocatedMemory;
    long MaximumMemory;
    unsigned long LastFrames;            // test probe: argument seen by GetUnloadableMemory
    unsigned long Unloadable;
    long GetFreeMemory() const { return MaximumMemory - AllocatedMemory; }
    unsigned long GetUnloadableMemory(unsigned long minUnusedFrames) const
    { const_cast<CResourceList*>(this)->LastFrames = minUnusedFrames; return Unloadable; }
};
class CGraphicDataBank {
public:
    CResourceList ResourceList;
    unsigned long ReduceSizeFrameDelay;
    long AvailableMemory;
    void Run() { UpdateAvailableMemory(); }
private:
    void UpdateAvailableMemory()
    {
        AvailableMemory = Max(ResourceList.GetFreeMemory(), 0L);
        AvailableMemory += ResourceList.GetUnloadableMemory(ReduceSizeFrameDelay);
    }
};
int main()
{
    CGraphicDataBank b;
    b.ResourceList.AllocatedMemory = 100; b.ResourceList.MaximumMemory = 300;
    b.ResourceList.Unloadable = 50; b.ReduceSizeFrameDelay = 9; b.AvailableMemory = -1;
    b.Run();
    if (b.AvailableMemory != 250 || b.ResourceList.LastFrames != 9) { std::printf("009fd3a0_TEST FAIL\n"); return 1; }
    b.ResourceList.AllocatedMemory = 500;            // over budget: free memory clamps to 0
    b.Run();
    if (b.AvailableMemory != 50) { std::printf("009fd3a0_TEST FAIL\n"); return 1; }
    b.ResourceList.AllocatedMemory = 300;            // exactly zero
    b.ResourceList.Unloadable = 0;
    b.Run();
    if (b.AvailableMemory != 0) { std::printf("009fd3a0_TEST FAIL\n"); return 1; }
    std::printf("009fd3a0_TEST PASS\n");
    return 0;
}