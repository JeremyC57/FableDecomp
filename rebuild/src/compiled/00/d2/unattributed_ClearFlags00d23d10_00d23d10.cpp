// CClass_00d23d10::ClearFlags_00d23d10() -- clears the 3 flag bytes at +0x48, +0x49, +0x4a
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D23D10, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor al, al ; mov byte ptr [ecx + 0x48], al ; mov byte ptr [ecx + 0x49], al ; mov byte ptr [ecx + 0x4a], al ; ret 
// Tiny-body shape `ClearFlags` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00d23d10 {
    unsigned char _pad0[0x48];
    unsigned char m_flag0;                 // +0x48
    unsigned char m_flag1;                 // +0x49
    unsigned char m_flag2;                 // +0x4a
    void ClearFlags_00d23d10();
};

void CClass_00d23d10::ClearFlags_00d23d10()
{
    m_flag0 = 0;
    m_flag1 = 0;
    m_flag2 = 0;
}