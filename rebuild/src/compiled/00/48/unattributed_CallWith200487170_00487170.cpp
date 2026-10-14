// CClass_00487170::CallWith2_00487170() -- calls the member method @ 0x006345C0 with the constants 18, 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487170, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push 0 ; push 0x12 ; call 0x6345c0 ; ret 
// Tiny-body shape `CallWith2` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00487170 {
    void Method(long first, long second);  // 0x006345C0
    void CallWith2_00487170();
};

void CClass_00487170::CallWith2_00487170()
{
    Method(18, 0);
}