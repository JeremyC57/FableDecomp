// Behaviour test for 0x00A64590 CMemoryAllocatorVariableSize::GetFragmentation: (free-1)/allocated, 0 for <=1 free area, globals published.
#include <cstdio>
static unsigned int g_shift = 0, g_align = 0;
struct V {
    unsigned int Alignment, AlignmentShift, freeAreas, allocated;
    virtual unsigned int GetNoFreeAreas() { return freeAreas; }
    virtual unsigned int GetNoAllocatedAreas() { return allocated; }
    float GetFragmentation()
    {
        g_shift = AlignmentShift; g_align = Alignment;
        unsigned int noFree = GetNoFreeAreas(); unsigned int noAllocated = GetNoAllocatedAreas();
        if (noFree <= 1) return 0.0f;
        float r = (float)(noFree - 1) / (float)noAllocated;
        return r;
    }
    virtual ~V() { }
};
int main()
{
    V v; v.Alignment = 16; v.AlignmentShift = 4; v.freeAreas = 5; v.allocated = 8;
    int bad = !(v.GetFragmentation() == 0.5f && g_shift == 4 && g_align == 16);
    v.freeAreas = 1;
    bad |= !(v.GetFragmentation() == 0.0f);
    if (bad) { std::printf("00a64590_TEST FAIL\n"); return 1; }
    std::printf("00a64590_TEST PASS\n");
    return 0;
}
