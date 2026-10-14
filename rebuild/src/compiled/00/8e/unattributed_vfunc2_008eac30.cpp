// CClass_008eac30::vfunc_2(long) -- calls the member method @ 0x008EA9F0; the 1 argument(s) are not forwarded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EAC30, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  call 0x8ea9f0 ; ret 4
// Tiny-body shape `ForwardIgnoringArgs` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_008eac30 {
    void Method();                         // 0x008EA9F0
    void vfunc_2(long);
};

void CClass_008eac30::vfunc_2(long)
{
    Method();
}