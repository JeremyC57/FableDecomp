// CClass_0053ce50::vfunc_172() -- returns bit 5 of the flag byte at +0x12e as a dword
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053CE50, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor eax, eax ; mov al, byte ptr [ecx + 0x12e] ; shr eax, 5 ; and eax, 1 ; ret 
// Tiny-body shape `GetFlagBit` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_0053ce50 {
    unsigned char _pad[0x12e];
    unsigned char m_flags;                 // +0x12e
    long vfunc_172();
};

long CClass_0053ce50::vfunc_172()
{
    return (m_flags >> 5) & 1;
}