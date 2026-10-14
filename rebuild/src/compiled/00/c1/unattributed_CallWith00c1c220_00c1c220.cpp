// CClass_00c1c220::CallWith_00c1c220() -- calls the member method @ 0x00C1C210 with the constant 19608120
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C220, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push 0x12b3238 ; call 0xc1c210 ; ret 
// Tiny-body shape `CallWith` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00c1c220 {
    void Method(long value);               // 0x00C1C210
    void CallWith_00c1c220();
};

void CClass_00c1c220::CallWith_00c1c220()
{
    Method(19608120);
}