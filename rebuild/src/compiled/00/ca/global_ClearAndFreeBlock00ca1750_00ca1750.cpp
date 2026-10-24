// _global::ClearAndFreeBlock_00ca1750(SBlock0x460* block) -- wipe a 0x460-byte heap block to zero and free it (null-safe)
// Flags: /Gr
// Leaf: ClearAndFreeBlock_00ca1750
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ca1750, 30 bytes (+ int3 padding).
// Callee: MSVCR71 free through its import slot 0x01440154 (`call dword ptr [__imp__free]`, cdecl
// `add esp,4`).
// Identity: the catalog label `_Cons_val<allocator<C3DGeneratorObject>,..>` is propagated over this
// body and wrong: nothing is constructed.  It is a __fastcall (block in ecx) destroy helper: when the
// block is non-null, `rep stosd` zeroes 0x118 dwords (0x460 bytes) and the block is freed -- the same
// shape as the landed 0x00CA3580 (96-byte block).  The block type is not identified.
// Shape: memset to zero of a constant multiple-of-4 size inlines to `xor eax,eax ; mov ecx,118h ;
// rep stosd`; the import is reached through the dllimport declaration.
#include <string.h>

extern "C" __declspec(dllimport) void __cdecl free(void* block);   // MSVCR71 0x01440154

struct SBlock0x460 {
    unsigned char m_bytes[0x460];
};

void __fastcall ClearAndFreeBlock_00ca1750(SBlock0x460* block)
{
    if (block) {
        memset(block, 0, sizeof(SBlock0x460));
        free(block);
    }
}