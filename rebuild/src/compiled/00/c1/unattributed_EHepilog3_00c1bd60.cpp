// CClass_00c1bd60::__EH_epilog3(long) -- stores the argument into the member at +0
// Leaf: __EH_epilog3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BD60, oracle length 9 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; mov dword ptr [ecx], eax ; ret 4
// Tiny-body shape `SetFirst` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00c1bd60 {
    long m_value;                          // +0
    void __EH_epilog3(long value);
};

void CClass_00c1bd60::__EH_epilog3(long value)
{
    m_value = value;
}