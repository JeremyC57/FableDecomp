// CClass_00985420::CallWith2_00985420() -- calls the member method @ 0x009852C0 with the constants 1073741824, 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00985420, oracle length 13 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push 1 ; push 0x40000000 ; call 0x9852c0 ; ret 
// Tiny-body shape `CallWith2` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00985420 {
    void Method(long first, long second);  // 0x009852C0
    void CallWith2_00985420();
};

void CClass_00985420::CallWith2_00985420()
{
    Method(1073741824, 1);
}