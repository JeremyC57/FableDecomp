// CClass_008000b0::GetFirstAsHandle_008000b0() -- returns the pointer at +0 wrapped in a one-pointer handle class (hidden return slot)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008000B0, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [ecx] ; mov eax, dword ptr [esp + 4] ; mov dword ptr [eax], ecx ; ret 4
// Tiny-body shape `GetFirstAsHandle` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CHandle {                           // non-POD one-pointer wrapper: returned through the hidden pointer
    void* m_p;
    CHandle(void* p) : m_p(p) { }
};

struct CClass_008000b0 {
    void* m_pFirst;                        // +0
    CHandle GetFirstAsHandle_008000b0();
};

CHandle CClass_008000b0::GetFirstAsHandle_008000b0()
{
    return CHandle(m_pFirst);
}