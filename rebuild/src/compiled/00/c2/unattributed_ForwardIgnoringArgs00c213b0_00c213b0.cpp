// CClass_00c213b0::ForwardIgnoringArgs_00c213b0(long) -- calls the member method @ 0x00C206E0; the 1 argument(s) are not forwarded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C213B0, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  call 0xc206e0 ; ret 4
// Tiny-body shape `ForwardIgnoringArgs` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00c213b0 {
    void Method();                         // 0x00C206E0
    void ForwardIgnoringArgs_00c213b0(long);
};

void CClass_00c213b0::ForwardIgnoringArgs_00c213b0(long)
{
    Method();
}