// CClass_0053cde8::vfunc_149() -- copies the member at +0x30 into the member at +0x160
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053CDE8, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x30] ; mov dword ptr [ecx + 0x160], eax ; ret 
// Tiny-body shape `CopyMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_0053cde8 {
    unsigned char _pad1[0x30];
    long m_source;                         // +0x30
    unsigned char _pad2[0x12c];
    long m_target;                         // +0x160
    void vfunc_149();
};

void CClass_0053cde8::vfunc_149()
{
    m_target = m_source;
}