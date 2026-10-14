// CClass_00c5e2a0::IsBelow_00c5e2a0() -- 1 when the unsigned member at +0x24 is below the one at +0x20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5E2A0, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x24] ; cmp eax, dword ptr [ecx + 0x20] ; sbb eax, eax ; neg eax ; ret 
// Tiny-body shape `IsBelow` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00c5e2a0 {
    unsigned char _pad[0x20];
    unsigned long m_limit;                 // +0x20
    unsigned long m_current;                   // +0x24
    long IsBelow_00c5e2a0();
};

long CClass_00c5e2a0::IsBelow_00c5e2a0()
{
    return m_current < m_limit;
}