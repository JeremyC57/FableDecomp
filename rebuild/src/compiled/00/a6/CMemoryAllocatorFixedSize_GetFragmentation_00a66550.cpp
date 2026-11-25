#include "candidates/fixed_size_allocator_accessors_compiled.h"
// Literal (NF415, 2026-11-25): was `return *reinterpret_cast<const float*>(0x0122DEDC);` (a load from the baked retail VA). Retail
//   0x00A66550 is `fld dword ptr [0x0122DEDC] ; ret`, and 0x0122DEDC holds 0.0f (00 00 00 00) in retail's float-constant pool (3.0f at
//   0x0122DED4, 1.0f at 0x0122DED8, -1.0f at 0x0122DEE0): VC7.1 emits exactly that load for `return 0.0f;` (fld dword ptr [__real@00000000]).

float CMemoryAllocatorFixedSize::GetFragmentation()
{
    return 0.0f;
}
