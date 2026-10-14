// CClass_00c36c90::ForwardMember_00c36c90(CTarget*) -- calls the target's method @ 0x00C361F0 with the member at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36C90, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 4] ; mov ecx, dword ptr [esp + 4] ; push eax ; call 0xc361f0 ; ret 4
// Tiny-body shape `ForwardMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    void Receive(long value);              // 0x00C361F0
};

struct CClass_00c36c90 {
    unsigned char _pad[0x4];
    long m_value;                          // +0x4
    void ForwardMember_00c36c90(CTarget* target);
};

void CClass_00c36c90::ForwardMember_00c36c90(CTarget* target)
{
    target->Receive(m_value);
}