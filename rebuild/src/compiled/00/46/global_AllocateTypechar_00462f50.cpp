#pragma optimize("s",on)
#include "rebuild_abi.h"

// 0x00462f50  AllocateType<char>  (CSimpleMemoryPool)
// Free function; retail cleans 12 bytes (ret 0xc). Three stack params.
// If count != 0, calls an allocator (cdecl, 1 arg) -> esi. Then a
// __fastcall construct/init helper is invoked with (ecx=p2, edx=p3, esi, &local).
// Returns esi (the allocated block, or null).

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (MSVCR71)
extern void FABLE_FASTCALL fable_init_462f50(void* p2, void* p3, void* mem, char* localByte);

extern "C" void* FABLE_STDCALL AllocateType_char_462f50(fable_u32 count, void* p2, void* p3)
{
    char localByte;
    void* mem = 0;
    if (count != 0)
    {
        mem = ((void*)(malloc(count)));
    }
    fable_init_462f50(p2, p3, mem, &localByte);
    return mem;
}