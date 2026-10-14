// CClass_005419f0::vfunc_88(long, long) -- returns the float literal 0.0f; the 2 argument(s) are not read
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005419F0, oracle length 9 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  fld dword ptr [0x122dedc] ; ret 8
// Tiny-body shape `GetConstant` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_005419f0 {
    float vfunc_88(long, long);                    // the literal is pooled in .rdata (@ 0x0122DEDC)
};

float CClass_005419f0::vfunc_88(long, long)
{
    return 0.0f;
}