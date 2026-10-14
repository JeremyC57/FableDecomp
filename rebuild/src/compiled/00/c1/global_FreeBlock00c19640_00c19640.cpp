// FreeBlock_00c19640(void*) -- deletes the argument (operator delete @ 0x00BFE9BC); stdcall
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19640, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; push eax ; call 0xbfe9bc ; pop ecx ; ret 4
// Tiny-body shape `FreeBlock`: a `__stdcall` wrapper that deletes its argument as a raw block (a
// delete-expression over a trivially destructible pointee: the block is loaded into eax, pushed to
// `operator delete` @ 0x00BFE9BC -- the FSE label `Game_free` -- and the slot is cleaned with `pop ecx`;
// a plain `free` call pushes the stack slot directly). The manifest label is propagated; the name is descriptive.
void __stdcall FreeBlock_00c19640(void* block)
{
    delete static_cast<char*>(block);
}