// CTCDRegionExit::vfunc_28(long) -- true when the argument equals 25
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00779940, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [esp + 4] ; xor eax, eax ; cmp ecx, 0x19 ; sete al ; ret 4
// Tiny-body shape `IsKind` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CTCDRegionExit { public:
    bool vfunc_28(long kind);
};

bool CTCDRegionExit::vfunc_28(long kind)
{
    return kind == 25;
}