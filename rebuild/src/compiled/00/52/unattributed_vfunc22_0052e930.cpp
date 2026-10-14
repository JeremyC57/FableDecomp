// CClass_0052e930::vfunc_22(const long&) -- stores the referenced value into the member at +0x90
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052E930, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; mov edx, dword ptr [eax] ; mov dword ptr [ecx + 0x90], edx ; ret 4
// Tiny-body shape `SetFromRef` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_0052e930 {
    unsigned char _pad[0x90];
    long m_value;                          // +0x90
    void vfunc_22(const long& value);
};

void CClass_0052e930::vfunc_22(const long& value)
{
    m_value = value;
}