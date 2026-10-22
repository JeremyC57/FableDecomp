// _global::CopyTrivialTagged(const char*, const char*, char*, __true_type) -- STLport trivial copy: memmove the byte range [first, last) to result and return result + byte count
// Leaf: CopyTrivialTagged
// Retail: Fable.exe (TLC, VC7.1) @ 0x00410CB0, 30 bytes. Callees: memmove (MSVCR71 import thunk 0x00BFEAE6).
// Unreferenced in the image (recovered from a mis-cut catalog row); the STLport `__copy_ptrs` /
// `__copy_trivial` shape of a by-value-tagged overload: `__fastcall` (the algorithm TUs are /Gr:
// first in ecx, last in edx, result and the empty tag on the stack, `ret 8`).
// Shape: `last == first ? result : (char*)memmove(result, first, n) + n` -- the result is loaded
// before the compare, the byte count lives in esi across the call and is added to memmove's return.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);

struct __true_type {};

char* __fastcall CopyTrivialTagged(const char* first, const char* last, char* result, __true_type)
{
    return (last == first) ? result
                           : (char*)memmove(result, first, last - first) + (last - first);
}