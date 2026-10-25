// IsCpuidSupported() -- report whether the processor implements CPUID: try to toggle the ID bit (bit 21) of EFLAGS and see whether the change sticks
// Leaf: ?IsCpuidSupported@@
// Grade: DIFFER(49v47) -- retail_asm candidate: retail toggles EFLAGS.ID with an inline-asm pushfd/popfd block inside an ebp frame (`[ebp-4]` flag local); the C++ reference calls stand-ins instead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C52AA0, length 47 bytes (+ int3 padding); plain `ret`, no direct callers.
// Retail-asm evidence: the body is an inline-asm block -- `push ebp ; mov ebp,esp ; push ecx` frame (inline asm forces
// ebp), `mov dword ptr [ebp-4],0`, then `pushfd ; pop eax ; mov ecx,eax ; xor eax,200000h ; push eax ; popfd ; pushfd ;
// pop eax ; xor eax,ecx ; je ; mov dword ptr [ebp-4],1`, then C++ `return supported != 0;` (`test ; setne al`). VC7.1
// has no intrinsic for pushfd/popfd (__readeflags arrived in VC8), so no C++ spelling can emit it. The lane rules forbid
// inline asm here; this is the C++ reference body, with out-of-line stand-ins for the two flag instructions.
// The catalog label "CPU_IsCpuidSupported" is descriptive.

unsigned long ReadEFlags();                         // stands for `pushfd ; pop eax`
void WriteEFlags(unsigned long flags);              // stands for `push eax ; popfd`

bool IsCpuidSupported()
{
    long supported = 0;
    unsigned long original = ReadEFlags();
    WriteEFlags(original ^ 0x200000);               // EFLAGS.ID
    if (ReadEFlags() ^ original)
        supported = 1;
    return supported != 0;
}
