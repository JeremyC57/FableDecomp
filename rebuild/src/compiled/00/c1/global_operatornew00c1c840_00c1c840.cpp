// operator_new(void*) -- `__fastcall` wrapper over the one-argument `__stdcall` import @ 0x0143FE10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C840, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push ecx ; call dword ptr [0x143fe10] ; ret 
// Tiny-body shape `CallImport`: the register argument is pushed to a `__stdcall` import (callee
// cleans the slot) and its result is returned. The import's identity is a relocation; the name is descriptive.
extern "C" __declspec(dllimport) void* __stdcall Import_0143fe10(void*);   // IAT slot 0x0143FE10

void* __fastcall operator_new_00c1c840(void* argument)
{
    return Import_0143fe10(argument);
}