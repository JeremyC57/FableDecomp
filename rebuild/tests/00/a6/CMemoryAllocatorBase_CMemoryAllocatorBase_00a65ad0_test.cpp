// Behaviour test for 0x00A65AD0 CMemoryAllocatorBase constructor: counters cleared, alignment stored, shift derived.
#include <cstdio>
struct CPerformanceStats {
    unsigned int AllocsThisFrame, TotalAllocs, TotalFrames, MaxAllocsPerFrame;
    unsigned int TotalAllocsPerID[256];
    CPerformanceStats() : AllocsThisFrame(0), TotalAllocs(0), TotalFrames(0), MaxAllocsPerFrame(0) { for (int i = 0; i < 256; ++i) TotalAllocsPerID[i] = 0; }
};
struct CMemoryAllocatorBase {
    CPerformanceStats PerformanceStats; unsigned int Alignment, AlignmentShift;
    CMemoryAllocatorBase(unsigned int alignment) : Alignment(alignment)
    {
        switch (alignment) { case 4: AlignmentShift = 2; break; case 8: AlignmentShift = 3; break; case 16: AlignmentShift = 4; break; default: AlignmentShift = 9999999; break; }
    }
};
int main()
{
    CMemoryAllocatorBase a(4), b(8), c(16), d(32);
    int bad = !(a.AlignmentShift == 2 && b.AlignmentShift == 3 && c.AlignmentShift == 4 && d.AlignmentShift == 9999999 && d.Alignment == 32);
    bad |= !(a.PerformanceStats.TotalAllocs == 0 && a.PerformanceStats.TotalAllocsPerID[255] == 0);
    if (bad) { std::printf("00a65ad0_TEST FAIL\n"); return 1; }
    std::printf("00a65ad0_TEST PASS\n");
    return 0;
}