// CClass_00e929a0::ClearFlags_00e929a0() -- clears the 4 flag bytes at +0x48, +0x49, +0x4a, +0x4b
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E929A0, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor al, al ; mov byte ptr [ecx + 0x48], al ; mov byte ptr [ecx + 0x49], al ; mov byte ptr [ecx + 0x4a], al ; mov byte ptr [ecx + 0x4b], al ; ret 
// Tiny-body shape `ClearFlags` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00e929a0 {
    unsigned char _pad0[0x48];
    unsigned char m_flag0;                 // +0x48
    unsigned char m_flag1;                 // +0x49
    unsigned char m_flag2;                 // +0x4a
    unsigned char m_flag3;                 // +0x4b
    void ClearFlags_00e929a0();
};

void CClass_00e929a0::ClearFlags_00e929a0()
{
    m_flag0 = 0;
    m_flag1 = 0;
    m_flag2 = 0;
    m_flag3 = 0;
}