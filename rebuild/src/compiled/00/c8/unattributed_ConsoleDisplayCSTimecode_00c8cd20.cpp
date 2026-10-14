// CClass_00c8cd20::ConsoleDisplayCSTimecode() -- the flag byte at +0x5 of the object at +0, masked with 0x4 (as a dword)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8CD20, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx] ; movzx eax, byte ptr [eax + 5] ; and eax, 4 ; ret 
// Tiny-body shape `GetTargetFlag` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x5];
    unsigned char m_flags;                 // +0x5
};

struct CClass_00c8cd20 {
    CTarget* m_pTarget;                    // +0
    long ConsoleDisplayCSTimecode();
};

long CClass_00c8cd20::ConsoleDisplayCSTimecode()
{
    return m_pTarget->m_flags & 0x4;
}