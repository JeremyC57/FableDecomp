// CClass_00e7bb00::DecrementThrough_00e7bb00() -- decrements the counter at +0x4c of the object at +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7BB00, oracle length 7 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x14] ; dec dword ptr [eax + 0x4c] ; ret 
// Tiny-body shape `DecrementThrough` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x4c];
    long m_count;                          // +0x4c
};

struct CClass_00e7bb00 {
    unsigned char _pad[0x14];
    CTarget* m_object;                     // +0x14
    void DecrementThrough_00e7bb00();
};

void CClass_00e7bb00::DecrementThrough_00e7bb00()
{
    --m_object->m_count;
}