// CClass_008543b0::vfunc_5(void**) -- stores the member at +0x50 through the out-pointer and returns true
// Leaf: vfunc_5
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008543B0, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x50] ; mov ecx, dword ptr [esp + 4] ; mov dword ptr [ecx], eax ; mov al, 1 ; ret 4
// Tiny-body shape `GetOut` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_008543b0 {
    unsigned char _pad[0x50];
    void* m_value;                         // +0x50
    bool vfunc_5(void** out);
};

bool CClass_008543b0::vfunc_5(void** out)
{
    *out = m_value;
    return true;
}