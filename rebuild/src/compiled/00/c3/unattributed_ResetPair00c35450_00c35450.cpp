// CClass_00c35450::ResetPair_00c35450() -- stores 3 at +0x10 and 0 at +0x8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C35450, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov dword ptr [ecx + 0x10], 3 ; mov dword ptr [ecx + 8], 0 ; ret 
// Tiny-body shape `ResetPair` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00c35450 {
    unsigned char _pad1[0x8];
    long m_second;                          // +0x8
    unsigned char _pad2[0x4];
    long m_first;                          // +0x10
    void ResetPair_00c35450();
};

void CClass_00c35450::ResetPair_00c35450()
{
    m_first = 3;
    m_second = 0;
}