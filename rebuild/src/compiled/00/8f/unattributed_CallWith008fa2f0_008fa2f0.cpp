// CClass_008fa2f0::CallWith_008fa2f0() -- calls the member method @ 0x008FA280 with the constant 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FA2F0, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push 0 ; call 0x8fa280 ; ret 
// Tiny-body shape `CallWith` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_008fa2f0 {
    void Method(long value);               // 0x008FA280
    void CallWith_008fa2f0();
};

void CClass_008fa2f0::CallWith_008fa2f0()
{
    Method(0);
}