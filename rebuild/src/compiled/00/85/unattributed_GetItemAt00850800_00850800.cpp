// CClass_00850800::GetItemAt_00850800(long index) -- indexes the pointer array at +0x1f0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00850800, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x1f0] ; mov ecx, dword ptr [esp + 4] ; mov eax, dword ptr [eax + ecx*4] ; ret 4
// Tiny-body shape `GetItemAt` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00850800 {
    unsigned char _pad[0x1f0];
    void** m_pItems;                       // +0x1f0
    void* GetItemAt_00850800(long index);
};

void* CClass_00850800::GetItemAt_00850800(long index)
{
    return m_pItems[index];
}