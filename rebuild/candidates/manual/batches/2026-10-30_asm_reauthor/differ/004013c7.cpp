// __SEH_epilog() -- VC7.1 CRT sehprolg.asm: unlink the SEH frame, restore the caller's saved registers and frame
// Grade: DIFFER(23v17) -- CRT sehprolg.asm __SEH_epilog (unlinks fs:[0], pops the caller-saved registers, `leave ; push ecx ; ret`): a non-naked C++ body adds its own push/pop of ebx/esi/edi. Should come from the CRT library, not reconstruction. 1 grade.
// Leaf: __SEH_epilog
// Retail-asm: 0x004013C7 `mov dword ptr fs:[0], ecx` (TIB SEH-chain write) and `leave`/`push ecx` re-pushing the return address
extern "C" void __cdecl _SEH_epilog()
{
    __asm
    {
        mov ecx, dword ptr [ebp - 0x10]
        mov dword ptr fs:[0], ecx
        pop ecx
        pop edi
        pop esi
        pop ebx
        leave
        push ecx
    }
}
