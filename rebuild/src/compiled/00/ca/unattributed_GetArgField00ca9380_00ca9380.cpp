// CClass_00ca9380::GetArgField_00ca9380(const CTarget*) -- returns the dword at +0x10 of the argument object
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA9380, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; mov eax, dword ptr [eax + 0x10] ; ret 4
// Tiny-body shape `GetArgField` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x10];
    void* m_value;                         // +0x10
};

struct CClass_00ca9380 {
    void* GetArgField_00ca9380(const CTarget* target);
};

void* CClass_00ca9380::GetArgField_00ca9380(const CTarget* target)
{
    return target->m_value;
}