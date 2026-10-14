// CClass_00b1ff70::vfunc_23(void**) -- stores the member at +0xc through the out-pointer and returns true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1FF70, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0xc] ; mov ecx, dword ptr [esp + 4] ; mov dword ptr [ecx], eax ; mov al, 1 ; ret 4
// Tiny-body shape `GetOut` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00b1ff70 {
    unsigned char _pad[0xc];
    void* m_value;                         // +0xc
    bool vfunc_23(void** out);
};

bool CClass_00b1ff70::vfunc_23(void** out)
{
    *out = m_value;
    return true;
}