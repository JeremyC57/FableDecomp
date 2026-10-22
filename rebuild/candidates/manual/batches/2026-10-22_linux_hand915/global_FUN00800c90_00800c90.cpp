// _global::FUN_00800c90() -- release a block owned through the first member: free(p) when non-null
// Retail: Fable.exe (TLC, VC7.1) @ 0x00800C90, 14 bytes: `mov eax,[ecx] ; test ; je ; push eax ; call free ; pop ecx`.
//   Unreferenced; the shape of an STLport _Vector_base / _String_base storage release (the CRT `free` import
//   thunk 0x00BFEA14, as the retail STLport allocator uses), next to std_vector_Reserve 0x00800CA0.
#include <stdlib.h>

struct CStorage_00800c90 {
    void* _M_start;                              // +0x00
    void FUN_00800c90();
};

void CStorage_00800c90::FUN_00800c90()
{
    if (_M_start)
        free(_M_start);
}
