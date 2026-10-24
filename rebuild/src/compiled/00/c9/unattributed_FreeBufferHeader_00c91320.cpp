// _unattributed::FreeBufferHeader(SBufferHeader14* header) -- free a 20-byte header's data block (the word at +8) and wipe the whole header to zero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C91320, length 34 bytes.
// Leaf: FreeBufferHeader
// Label correction: the catalog "_Dest_val<..., std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32>>>"
// is propagated. The body frees the pointer at +8 through the MSVCR71 import (no null test: free(0) is legal) and zeroes
// five dwords. It belongs to the C-style library at 0x00C8xxxx-0x00CAxxxx that calls `free` through its import slot
// 0x01440154 (as the landed ClearAndFreeBlock 0x00CA1750); six callers (0x00C8EC53, 0x00C9CF7D ..., and the tail-jump
// thunk 0x00C91350). The header type is not identified; its fields are named descriptively.
// Convention: `__fastcall` (header in ecx, plain `ret`).
// Callees: free (MSVCR71 import 0x01440154).
// Shape: `memset(header, 0, 20)` after the free inlines to one zero register and five stores, with the free's
// `add esp,4` scheduled among them (explicit member stores schedule the `add esp,4` first).
#include <string.h>
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // MSVCR71 0x01440154
struct SBufferHeader14 {
    long  m_count;                          // +0x00
    long  m_capacity;                       // +0x04
    void* m_pData;                          // +0x08
    long  m_c;                              // +0x0c
    long  m_10;                             // +0x10
};
void __fastcall FreeBufferHeader(SBufferHeader14* header)
{
    free(header->m_pData);
    memset(header, 0, sizeof(SBufferHeader14));
}