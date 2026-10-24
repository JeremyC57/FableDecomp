// CClass_0052e9b0::vfunc_30(const long&) -- stores the referenced value into the member at +0x94
// Leaf: vfunc_30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052E9B0, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; mov edx, dword ptr [eax] ; mov dword ptr [ecx + 0x94], edx ; ret 4
// Tiny-body shape `SetFromRef` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_0052e9b0 {
    unsigned char _pad[0x94];
    long m_value;                          // +0x94
    void vfunc_30(const long& value);
};

void CClass_0052e9b0::vfunc_30(const long& value)
{
    m_value = value;
}