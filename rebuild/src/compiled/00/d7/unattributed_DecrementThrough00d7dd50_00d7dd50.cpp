// CClass_00d7dd50::DecrementThrough_00d7dd50() -- decrements the counter at +0x48 of the object at +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D7DD50, oracle length 7 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x14] ; dec dword ptr [eax + 0x48] ; ret 
// Tiny-body shape `DecrementThrough` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x48];
    long m_count;                          // +0x48
};

struct CClass_00d7dd50 {
    unsigned char _pad[0x14];
    CTarget* m_object;                     // +0x14
    void DecrementThrough_00d7dd50();
};

void CClass_00d7dd50::DecrementThrough_00d7dd50()
{
    --m_object->m_count;
}