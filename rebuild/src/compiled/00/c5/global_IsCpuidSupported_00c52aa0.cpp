// IsCpuidSupported() -- libpk CPU detection: whether the processor implements CPUID, by toggling the ID bit (bit 21) of EFLAGS and checking that the change sticks
// Leaf: ?IsCpuidSupported@@
// Retail-asm: 0x00C52AAB..0x00C52AC3 `pushfd ; pop eax ; mov ecx,eax ; xor eax,200000h ; push eax ; popfd ; pushfd ; pop eax ; xor eax,ecx ; je ; mov dword ptr [ebp-4],1` -- the classic EFLAGS.ID probe as an inline-assembly block over a named long local (11 instructions, one je: rasm1 family (d)). VC7.1 has no pushfd/popfd intrinsic (__readeflags arrived in VC8), so no C++ spelling emits it; the zero-initialisation and the `test ; setne` return are C++ (an `unsigned char` result: no `xor eax,eax` before the test).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C52AA0, 47 bytes (+ int3 padding); plain `ret`, no direct callers. The asm block over a
// local forces the `push ebp ; mov ebp,esp ; push ecx` frame. The catalog label "CPU_IsCpuidSupported" is descriptive.
#pragma optimize("y", off)
unsigned char IsCpuidSupported()
{
    long supported = 0;
    __asm {
        pushfd
        pop   eax
        mov   ecx, eax
        xor   eax, 200000h
        push  eax
        popfd
        pushfd
        pop   eax
        xor   eax, ecx
        je    done
        mov   supported, 1
    done:
    }
    return supported != 0;
}