// CClass_00c5d690::IsBelow_00c5d690() -- 1 when the unsigned member at +0x14 is below the one at +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D690, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x14] ; cmp eax, dword ptr [ecx + 0x10] ; sbb eax, eax ; neg eax ; ret 
// Tiny-body shape `IsBelow` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00c5d690 {
    unsigned char _pad[0x10];
    unsigned long m_limit;                 // +0x10
    unsigned long m_current;                   // +0x14
    long IsBelow_00c5d690();
};

long CClass_00c5d690::IsBelow_00c5d690()
{
    return m_current < m_limit;
}