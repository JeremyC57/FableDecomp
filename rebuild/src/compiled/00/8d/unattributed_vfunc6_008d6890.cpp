// CClass_008d6890::vfunc_6() -- calls the method @ 0x004C9B80 of the object at +0x4 with the constant 1
// Leaf: vfunc_6
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6890, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [ecx + 4] ; push 1 ; call 0x4c9b80 ; ret 
// Tiny-body shape `ForwardConstant` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    void Method_004c9b80(long value);               // 0x004C9B80
};

struct CClass_008d6890 {
    unsigned char _pad[0x4];
    CTarget* m_pObject;                    // +0x4
    void vfunc_6();
};

void CClass_008d6890::vfunc_6()
{
    m_pObject->Method_004c9b80(1);
}