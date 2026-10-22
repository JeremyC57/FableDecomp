// CMemoryAllocatorBase::PublishAlignment() -- copy this allocator's AlignmentShift and Alignment into the two allocator-module globals the area helpers read
// Leaf: PublishAlignment
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A643B0, 24 bytes, `ret`. Unreferenced out-of-line copy of an inline helper:
//   the same two stores open many CMemoryAllocatorVariableSize bodies (the globals 0x013D28D8 / 0x013D28D4 have 37
//   absolute references each in 0x00A64xxx-0x00A65xxx, e.g. the landed GetLargestFreeAreaSize 0x00A64500).
// Name: descriptive (catalogued FUN_00a643b0); the globals carry the alignment shift and the alignment of the
//   allocator currently being worked on (the landed sources model them as g_LastAllocatorChunkCounter_013d28d8 /
//   g_LastAllocatorUsage_013d28d4; the engine header names the source members Alignment +0xC14 / AlignmentShift +0xC18).
// Shape: AlignmentShift is stored first, then Alignment, each through its own register.
#include "engine/CMemoryAllocatorBase.h"

extern unsigned long g_CurrentAlignmentShift;     // 0x013D28D8
extern unsigned long g_CurrentAlignment;          // 0x013D28D4

struct CMemoryAllocatorBaseMethods : CMemoryAllocatorBase {
    void PublishAlignment();
};

void CMemoryAllocatorBaseMethods::PublishAlignment()
{
    g_CurrentAlignmentShift = AlignmentShift;
    g_CurrentAlignment = Alignment;
}