// CClass_00c34b60::GetItemAt_00c34b60(long index, long unused1) -- indexes the pointer array at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34B60, oracle length 13 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 4] ; mov ecx, dword ptr [esp + 4] ; mov eax, dword ptr [eax + ecx*4] ; ret 8
// Tiny-body shape `GetItemAt` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00c34b60 {
    unsigned char _pad[0x4];
    void** m_pItems;                       // +0x4
    void* GetItemAt_00c34b60(long index, long unused1);
};

void* CClass_00c34b60::GetItemAt_00c34b60(long index, long unused1)
{
    return m_pItems[index];
}