// operator_new(void*) -- `__fastcall` wrapper over the one-argument `__stdcall` import @ 0x0143FDE4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4870, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push ecx ; call dword ptr [0x143fde4] ; ret 
// Tiny-body shape `CallImport`: the register argument is pushed to a `__stdcall` import (callee
// cleans the slot) and its result is returned. The import's identity is a relocation; the name is descriptive.
extern "C" __declspec(dllimport) void* __stdcall Import_0143fde4(void*);   // IAT slot 0x0143FDE4

void* __fastcall operator_new_00ca4870(void* argument)
{
    return Import_0143fde4(argument);
}