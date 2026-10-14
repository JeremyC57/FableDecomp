// CTCPhysicsStandard::CanCollideWithThingsOneWay() -- the flag byte at +0x44 masked with 0x1, as a dword
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27B70, oracle length 9 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor eax, eax ; mov al, byte ptr [ecx + 0x44] ; and eax, 1 ; ret 
// Tiny-body shape `GetFlagMask` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTCPhysicsStandard {
    unsigned char _pad[0x44];
    unsigned char m_flags;                 // +0x44
    long CanCollideWithThingsOneWay();
};

long CTCPhysicsStandard::CanCollideWithThingsOneWay()
{
    return m_flags & 0x1;
}