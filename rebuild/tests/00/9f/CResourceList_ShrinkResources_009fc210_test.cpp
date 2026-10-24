// Behaviour test for 0x009FC210 CResourceList::ShrinkResources: unload resources while AllocatedMemory + size exceeds
// MaximumMemory, at most ResourceCount times, stopping early when UnloadNextResource fails.
#include <cstdio>

static int gUnloads = 0;
static int gFailAfter = 1000;
static long gFreedPerUnload = 100;

struct CResourceList {
    void* __vftable;
    unsigned char Head[0x1c];
    unsigned long ResourceCount;
    long AllocatedMemory;
    long MaximumMemory;
    bool UnloadNextResource(unsigned long frames)
    {
        if (frames != 0 || gUnloads >= gFailAfter)
            return false;
        ++gUnloads;
        AllocatedMemory -= gFreedPerUnload;
        return true;
    }
    void ShrinkResources(unsigned long size)
    {
        unsigned long count = 0;
        while (AllocatedMemory + size > MaximumMemory) {
            if (count++ >= ResourceCount)
                break;
            if (!UnloadNextResource(0))
                break;
        }
    }
};

int main()
{
    CResourceList r;
    r.ResourceCount = 10; r.AllocatedMemory = 900; r.MaximumMemory = 1000;
    r.ShrinkResources(50);                                   // already fits
    bool ok = gUnloads == 0 && r.AllocatedMemory == 900;
    r.ShrinkResources(350);                                  // needs 250 more: three unloads
    ok = ok && gUnloads == 3 && r.AllocatedMemory == 600;
    gUnloads = 0; r.ResourceCount = 2; r.AllocatedMemory = 900;
    r.ShrinkResources(1000);                                 // bounded by ResourceCount
    ok = ok && gUnloads == 2 && r.AllocatedMemory == 700;
    gUnloads = 0; gFailAfter = 1; r.ResourceCount = 10; r.AllocatedMemory = 900;
    r.ShrinkResources(1000);                                 // stops when nothing more unloads
    ok = ok && gUnloads == 1 && r.AllocatedMemory == 800;
    if (!ok) { std::printf("009fc210_TEST FAIL\n"); return 1; }
    std::printf("009fc210_TEST PASS\n");
    return 0;
}