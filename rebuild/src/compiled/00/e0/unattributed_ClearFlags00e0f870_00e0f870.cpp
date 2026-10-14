// CClass_00e0f870::ClearFlags_00e0f870() -- clears the 3 flag bytes at +0x1c, +0x1d, +0x1e
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0F870, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor al, al ; mov byte ptr [ecx + 0x1c], al ; mov byte ptr [ecx + 0x1d], al ; mov byte ptr [ecx + 0x1e], al ; ret 
// Tiny-body shape `ClearFlags` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00e0f870 {
    unsigned char _pad0[0x1c];
    unsigned char m_flag0;                 // +0x1c
    unsigned char m_flag1;                 // +0x1d
    unsigned char m_flag2;                 // +0x1e
    void ClearFlags_00e0f870();
};

void CClass_00e0f870::ClearFlags_00e0f870()
{
    m_flag0 = 0;
    m_flag1 = 0;
    m_flag2 = 0;
}