// CTCHitOnCollision::vfunc_2() -- calls the method @ 0x004C9840 of the object at +0x4 with the constant 77
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C81A0, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [ecx + 4] ; push 0x4d ; call 0x4c9840 ; ret 
// Tiny-body shape `ForwardConstant` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    void Method(long value);               // 0x004C9840
};

struct CTCHitOnCollision {
    unsigned char _pad[0x4];
    CTarget* m_pObject;                    // +0x4
    void vfunc_2();
};

void CTCHitOnCollision::vfunc_2()
{
    m_pObject->Method(77);
}