// _global::CopyTrivial(const char*, const char*, char*) -- STLport `__copy_trivial`: memmove the byte range [first, last) to result and return result + byte count
// Leaf: CopyTrivial
// Retail: Fable.exe (TLC, VC7.1) @ 0x00410D40, 30 bytes. Callees: memmove (MSVCR71 import thunk 0x00BFEAE6).
// Unreferenced in the image (recovered from a mis-cut catalog row). `__fastcall` (algorithm TU built
// with /Gr: first in ecx, last in edx, result on the stack, `ret 4`).
// Shape: `last == first ? result : (char*)memmove(result, first, n) + n` -- the result is loaded
// before the compare, the byte count lives in esi across the call and is added to memmove's return.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);

char* __fastcall CopyTrivial(const char* first, const char* last, char* result)
{
    return (last == first) ? result
                           : (char*)memmove(result, first, last - first) + (last - first);
}