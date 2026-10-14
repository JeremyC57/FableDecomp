// CClass_00c34a70::ApplyMemberArg_00c34a70(long unused0, void* value) -- calls the member at +0x4's method @ 0x00C42AB0 with argument 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34A70, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 8] ; push eax ; add ecx, 4 ; call 0xc42ab0 ; ret 8
// Tiny-body shape `ApplyMemberArg` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CMember {
    void Apply(void* value);               // 0x00C42AB0
};

struct CClass_00c34a70 {
    unsigned char _pad[0x4];
    CMember m_member;                      // +0x4
    void ApplyMemberArg_00c34a70(long unused0, void* value);
};

void CClass_00c34a70::ApplyMemberArg_00c34a70(long unused0, void* value)
{
    m_member.Apply(value);
}