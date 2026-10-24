// _global::CopyTrivialBackward(const char* first, const char* last, char* result) -- STLport `__copy_trivial_backward`: memmove [first, last) so that it ends at result, return the new start
// Leaf: CopyTrivialBackward
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00405BC0, 26 bytes. Callee: memmove (MSVCR71 import
// thunk 0x00BFEAE6).
//
// Retail bytes:  mov eax,[result] ; sub edx,ecx ; test edx,edx ; jle R ; push edx ; push ecx ;
//   sub eax,edx ; push eax ; call memmove ; add esp,0xc ; R: ret 4
// The manifest label make_heap<CThing**,CThingCompare_BestForTargeting2> is propagated: the body
// is the byte-identical twin of the landed CopyTrivialBackward 0x00410D60 (only the memmove call
// displacement differs), the STLport trivial-type copy_backward helper. `__fastcall` (first in
// ecx, last in edx, result on the stack, `ret 4`); 14 call/jmp sites (0x00485276 .. 0x006560DF),
// i.e. a shared COMDAT that many pointer-vector inserts reach. Shape: `n > 0 ? memmove(result - n,
// first, n) : result` -- eax holds result from entry, the taken branch returns memmove's result.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);

char* __fastcall CopyTrivialBackward(const char* first, const char* last, char* result)
{
    const int n = last - first;
    return (n > 0) ? (char*)memmove(result - n, first, n) : result;
}