// CClass_0041db74::vfunc_24(long) -- passes the by-value argument by reference to the member at +0x5c's setter @ 0x0042B687
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041DB74, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  lea eax, [esp + 4] ; push eax ; add ecx, 0x5c ; call 0x42b687 ; ret 4
// Tiny-body shape `SetMemberValue` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CMember {
    void Set_0042b687(const long& value);           // 0x0042B687
};

struct CClass_0041db74 {
    unsigned char _pad[0x5c];
    CMember m_member;                      // +0x5c
    void vfunc_24(long value);
};

void CClass_0041db74::vfunc_24(long value)
{
    m_member.Set_0042b687(value);
}