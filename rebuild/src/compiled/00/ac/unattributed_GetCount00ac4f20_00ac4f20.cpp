// CClass_00ac4f20::GetCount_00ac4f20() -- the element count of the 4-byte-element range at +0x10/+0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC4F20, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x14] ; sub eax, dword ptr [ecx + 0x10] ; sar eax, 2 ; ret 
// Tiny-body shape `GetCount` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct SElement { unsigned char _body[0x4]; };

struct CClass_00ac4f20 {
    unsigned char _pad[0x10];
    SElement* m_first;                     // +0x10
    SElement* m_last;                      // +0x14
    long GetCount_00ac4f20();
};

long CClass_00ac4f20::GetCount_00ac4f20()
{
    return m_last - m_first;
}