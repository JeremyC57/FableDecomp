// _global::CopyTrivialBackward(const char*, const char*, char*) -- STLport `__copy_trivial_backward`: memmove [first, last) so that it ends at result; returns the new start
// Leaf: CopyTrivialBackward
// Retail: Fable.exe (TLC, VC7.1) @ 0x00410D60, 26 bytes. Callees: memmove (MSVCR71 import thunk 0x00BFEAE6).
// Unreferenced in the image (recovered from a mis-cut catalog row). `__fastcall` (algorithm TU built
// with /Gr: first in ecx, last in edx, result on the stack, `ret 4`).
// Shape: `n > 0 ? memmove(result - n, first, n) : result` -- eax holds result on entry, the count is
// computed in edx in place, and the taken branch returns memmove's own result.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);

char* __fastcall CopyTrivialBackward(const char* first, const char* last, char* result)
{
    const int n = last - first;
    return (n > 0) ? (char*)memmove(result - n, first, n) : result;
}