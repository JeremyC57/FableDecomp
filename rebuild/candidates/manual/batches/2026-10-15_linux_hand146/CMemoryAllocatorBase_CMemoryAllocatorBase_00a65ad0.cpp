// CMemoryAllocatorBase::CMemoryAllocatorBase(unsigned long alignment) -- base constructor of every engine allocator: zero the performance counters and the per-ID allocation totals, store the alignment and derive its shift (4->2, 8->3, 16->4, anything else 9999999)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65AD0, length 113 bytes. No callees; vtable 0x0129DADC.
// Layout (PDB): vptr, PerformanceStats (CPerformanceStats, 0xc10) +0x04, Alignment +0xc14, AlignmentShift +0xc18.
//   CPerformanceStats: AllocsThisFrame, TotalAllocs, TotalFrames, MaxAllocsPerFrame, TotalAllocsPerID[256], TotalAllocsPerIDLastFrame[256],
//   TotalAllocsPerIDThisFrame[256]; its inline constructor clears only the four counters and TotalAllocsPerID.
//   (engine/CMemoryAllocatorBase.h flattens PerformanceStats into bytes, so the constructor-bearing member is modelled locally.)
// Shape: the per-ID clear is a plain zeroing loop -- VC turns it into `lea edi ; mov ecx,100h ; rep stosd` (a memset intrinsic loads
//   ecx first); the switch default folds into `sete ; dec ; and ; add`.
struct CPerformanceStats {
    unsigned long AllocsThisFrame;
    unsigned long TotalAllocs;
    unsigned long TotalFrames;
    unsigned long MaxAllocsPerFrame;
    unsigned long TotalAllocsPerID[256];
    unsigned long TotalAllocsPerIDLastFrame[256];
    unsigned long TotalAllocsPerIDThisFrame[256];
    CPerformanceStats() : AllocsThisFrame(0), TotalAllocs(0), TotalFrames(0), MaxAllocsPerFrame(0)
    {
        for (int i = 0; i < 256; ++i) TotalAllocsPerID[i] = 0;
    }
};
struct CMemoryAllocatorBase {
    virtual void ConsolidateFreeAreas();           // the class is polymorphic (vtable 0x0129DADC)
    CPerformanceStats PerformanceStats;
    unsigned long Alignment;
    unsigned long AlignmentShift;
    CMemoryAllocatorBase(unsigned long alignment);
};
CMemoryAllocatorBase::CMemoryAllocatorBase(unsigned long alignment)
    : Alignment(alignment)
{
    switch (alignment) {
    case 4:  AlignmentShift = 2; break;
    case 8:  AlignmentShift = 3; break;
    case 16: AlignmentShift = 4; break;
    default: AlignmentShift = 9999999; break;
    }
}
