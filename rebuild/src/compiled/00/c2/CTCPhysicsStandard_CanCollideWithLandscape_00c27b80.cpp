// CTCPhysicsStandard::CanCollideWithLandscape() -- returns bit 1 of the flag byte at +0x44 as a dword
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27B80, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor eax, eax ; mov al, byte ptr [ecx + 0x44] ; shr eax, 1 ; and eax, 1 ; ret 
// Tiny-body shape `GetFlagBit` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTCPhysicsStandard {
    unsigned char _pad[0x44];
    unsigned char m_flags;                 // +0x44
    long CanCollideWithLandscape();
};

long CTCPhysicsStandard::CanCollideWithLandscape()
{
    return (m_flags >> 1) & 1;
}