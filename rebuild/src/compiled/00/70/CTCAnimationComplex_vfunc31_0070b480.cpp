// CTCAnimationComplex::vfunc_31() -- calls the method @ 0x004C7590 of the object at +0x4 with the constant 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070B480, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [ecx + 4] ; push 2 ; call 0x4c7590 ; ret 
// Tiny-body shape `ForwardConstant` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    void Method_004c7590(long value);               // 0x004C7590
};

class CTCAnimationComplex { public:
    unsigned char _pad[0x4];
    CTarget* m_pObject;                    // +0x4
    void vfunc_31();
};

void CTCAnimationComplex::vfunc_31()
{
    m_pObject->Method_004c7590(2);
}