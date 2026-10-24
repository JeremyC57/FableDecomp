// vfunc_2(void*) -- deletes the argument (free @ 0x00BFEA14 (MSVCR71 free thunk, not operator delete)); stdcall
// Leaf: vfunc_2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3F9A0, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; push eax ; call 0xbfea14 ; pop ecx ; ret 4
// Tiny-body shape `FreeBlock`: a `__stdcall` wrapper that deletes its argument as a raw block (a
// delete-expression over a trivially destructible pointee: the block is loaded into eax, pushed to
// `free` @ 0x00BFEA14 -- the FSE label `Game_free` -- and the slot is cleaned with `pop ecx`;
// a plain `free` call pushes the stack slot directly). The manifest label is propagated; the name is descriptive.
extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14
void __stdcall vfunc_2(void* block)
{
    free(block);
}