// CClass_00c36cb0::ForwardMember_00c36cb0(CTarget*) -- calls the target's method @ 0x00C36430 with the member at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36CB0, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 4] ; mov ecx, dword ptr [esp + 4] ; push eax ; call 0xc36430 ; ret 4
// Tiny-body shape `ForwardMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    void Receive_00c36430(long value);              // 0x00C36430
};

struct CClass_00c36cb0 {
    unsigned char _pad[0x4];
    long m_value;                          // +0x4
    void ForwardMember_00c36cb0(CTarget* target);
};

void CClass_00c36cb0::ForwardMember_00c36cb0(CTarget* target)
{
    target->Receive_00c36430(m_value);
}