// __stdcall forwarder to the __cdecl routine @ 0x00C3CDE0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36090, oracle length 14 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,[esp+4] ; push eax ; call Callback_00C3CDE0 ; pop ecx ; ret 4
// A one-argument __stdcall thunk around a __cdecl routine: the argument is re-pushed, the callee's
// stack slot is popped (`pop ecx`) and the thunk cleans its own argument (`ret 4`). Such thunks are
// what the engine hands to C libraries (zlib/lua allocator hooks, CRT callbacks).
extern "C" void __cdecl Callback_00C3CDE0(void* block);

void __stdcall StdcallForward_00c3cde0(void* block)
{
    Callback_00C3CDE0(block);
}