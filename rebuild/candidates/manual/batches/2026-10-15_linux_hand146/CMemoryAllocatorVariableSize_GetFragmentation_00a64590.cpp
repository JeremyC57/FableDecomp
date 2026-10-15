// CMemoryAllocatorVariableSize::GetFragmentation() -- publish the allocator's alignment to the shared alignment globals, then (free areas - 1) / allocated areas as a float (0 when at most one free area)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A64590, length 102 bytes.
// Callees: own virtuals slot 8 GetNoFreeAreas (0x00A64530) and slot 9 GetNoAllocatedAreas (0x00A64560) (vtable 0x0129DB1C).
// Globals: the current alignment shift 0x013D28D8 and alignment 0x013D28D4 (the static alignment state the allocator's size helpers read);
//   float pool 0x0122DEDC = 0.0f, 0x0122DCB4 = 2^32 (the unsigned-to-float fix-up).
// Layout (PDB): CMemoryAllocatorBase Alignment +0xc14, AlignmentShift +0xc18.
// Shape: both unsigned counts converted (`fild ; jge ; fadd 2^32`); the quotient is a named float local so `fdivp` lands before the pops.
extern unsigned long g_AllocatorAlignmentShift;   // 0x013D28D8
extern unsigned long g_AllocatorAlignment;        // 0x013D28D4

struct CMemoryAllocatorVariableSize {
    virtual void Allocate(); virtual void Deallocate(); virtual void v02(); virtual void GetAllocationSize(); virtual void GetAllocationSizeIncludingHeader();   // slots 0-4
    virtual void GetLargestFreeAreaSize(); virtual void GetTotalMemoryControlledByAllocator(); virtual void GetTotalMemoryAllocated();
    virtual unsigned long GetNoFreeAreas();        // slot 8
    virtual unsigned long GetNoAllocatedAreas();   // slot 9
    virtual float GetFragmentation();              // slot 10
    unsigned char _stats[0xc10];                   // +0x04 CPerformanceStats
    unsigned long Alignment;                       // +0xc14
    unsigned long AlignmentShift;                  // +0xc18
};

float CMemoryAllocatorVariableSize::GetFragmentation()
{
    g_AllocatorAlignmentShift = AlignmentShift;
    g_AllocatorAlignment = Alignment;
    unsigned long noFree = GetNoFreeAreas();
    unsigned long noAllocated = GetNoAllocatedAreas();
    if (noFree <= 1)
        return 0.0f;
    float r = (float)(noFree - 1) / (float)noAllocated;
    return r;
}
