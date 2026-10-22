// Behaviour test for 00a643b0 CMemoryAllocatorBase::PublishAlignment: copies the allocator's alignment and shift into the
// two current-alignment globals.
#include <cstdio>
static unsigned long g_CurrentAlignmentShift;
static unsigned long g_CurrentAlignment;
struct CMemoryAllocatorBase {
    unsigned char _pad[0x10];
    unsigned long Alignment;
    unsigned long AlignmentShift;
    void PublishAlignment() { g_CurrentAlignmentShift = AlignmentShift; g_CurrentAlignment = Alignment; }
};
int main()
{
    CMemoryAllocatorBase a; a.Alignment = 16; a.AlignmentShift = 4;
    a.PublishAlignment();
    if (g_CurrentAlignment != 16 || g_CurrentAlignmentShift != 4) { std::printf("00a643b0_TEST FAIL\n"); return 1; }
    a.Alignment = 8; a.AlignmentShift = 3;
    a.PublishAlignment();
    if (g_CurrentAlignment != 8 || g_CurrentAlignmentShift != 3) { std::printf("00a643b0_TEST FAIL\n"); return 1; }
    std::printf("00a643b0_TEST PASS\n");
    return 0;
}