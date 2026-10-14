// CClass_0084cde0::vfunc_12() -- returns the byte at +0x150 of the object at +0x40
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084CDE0, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x40] ; mov al, byte ptr [eax + 0x150] ; ret 
// Tiny-body shape `GetByteThrough` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x150];
    unsigned char m_value;                 // +0x150
};

struct CClass_0084cde0 {
    unsigned char _pad[0x40];
    CTarget* m_object;                     // +0x40
    unsigned char vfunc_12();
};

unsigned char CClass_0084cde0::vfunc_12()
{
    return m_object->m_value;
}