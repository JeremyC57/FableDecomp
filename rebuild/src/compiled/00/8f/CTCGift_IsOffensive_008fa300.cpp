// CClass_008fa300::CallWith_008fa300() -- calls the member method @ 0x008FA280 with the constant 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FA300, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push 2 ; call 0x8fa280 ; ret 
// Tiny-body shape `CallWith` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_008fa300 {
    void Method(long value);               // 0x008FA280
    void CallWith_008fa300();
};

void CClass_008fa300::CallWith_008fa300()
{
    Method(2);
}