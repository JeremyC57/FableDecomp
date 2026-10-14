// CClass_0041db64::vfunc_27(long) -- passes the by-value argument by reference to the member at +0x60's setter @ 0x0042B646
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041DB64, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  lea eax, [esp + 4] ; push eax ; add ecx, 0x60 ; call 0x42b646 ; ret 4
// Tiny-body shape `SetMemberValue` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CMember {
    void Set(const long& value);           // 0x0042B646
};

struct CClass_0041db64 {
    unsigned char _pad[0x60];
    CMember m_member;                      // +0x60
    void vfunc_27(long value);
};

void CClass_0041db64::vfunc_27(long value)
{
    m_member.Set(value);
}