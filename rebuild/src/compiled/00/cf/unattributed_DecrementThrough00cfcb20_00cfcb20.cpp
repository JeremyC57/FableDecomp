// CClass_00cfcb20::DecrementThrough_00cfcb20() -- decrements the counter at +0x68 of the object at +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFCB20, oracle length 7 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x14] ; dec dword ptr [eax + 0x68] ; ret 
// Tiny-body shape `DecrementThrough` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x68];
    long m_count;                          // +0x68
};

struct CClass_00cfcb20 {
    unsigned char _pad[0x14];
    CTarget* m_object;                     // +0x14
    void DecrementThrough_00cfcb20();
};

void CClass_00cfcb20::DecrementThrough_00cfcb20()
{
    --m_object->m_count;
}