// CClass_00d51450::ClearFlags_00d51450() -- clears the 4 flag bytes at +0x49, +0x4a, +0x54, +0x55
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D51450, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor al, al ; mov byte ptr [ecx + 0x49], al ; mov byte ptr [ecx + 0x4a], al ; mov byte ptr [ecx + 0x54], al ; mov byte ptr [ecx + 0x55], al ; ret 
// Tiny-body shape `ClearFlags` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00d51450 {
    unsigned char _pad0[0x49];
    unsigned char m_flag0;                 // +0x49
    unsigned char m_flag1;                 // +0x4a
    unsigned char _pad2[0x9];
    unsigned char m_flag2;                 // +0x54
    unsigned char m_flag3;                 // +0x55
    void ClearFlags_00d51450();
};

void CClass_00d51450::ClearFlags_00d51450()
{
    m_flag0 = 0;
    m_flag1 = 0;
    m_flag2 = 0;
    m_flag3 = 0;
}