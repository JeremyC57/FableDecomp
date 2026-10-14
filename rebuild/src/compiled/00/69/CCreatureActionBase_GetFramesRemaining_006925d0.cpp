// CCreatureActionBase::GetFramesRemaining() -- the member at +0x10 minus the member at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006925D0, oracle length 7 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x10] ; sub eax, dword ptr [ecx + 0x1c] ; ret 
// Tiny-body shape `GetDifference` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CCreatureActionBase {
    unsigned char _pad1[0x10];
    long m_start;                          // +0x10
    unsigned char _pad2[0x8];
    long m_end;                            // +0x1c
    long GetFramesRemaining();
};

long CCreatureActionBase::GetFramesRemaining()
{
    return m_start - m_end;
}