// CClass_00c44c40::ForwardMember_00c44c40(CTarget*) -- calls the target's method @ 0x00C61F60 with the member at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44C40, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 4] ; mov ecx, dword ptr [esp + 4] ; push eax ; call 0xc61f60 ; ret 4
// Tiny-body shape `ForwardMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    void Receive_00c61f60(long value);              // 0x00C61F60
};

struct CClass_00c44c40 {
    unsigned char _pad[0x4];
    long m_value;                          // +0x4
    void ForwardMember_00c44c40(CTarget* target);
};

void CClass_00c44c40::ForwardMember_00c44c40(CTarget* target)
{
    target->Receive_00c61f60(m_value);
}