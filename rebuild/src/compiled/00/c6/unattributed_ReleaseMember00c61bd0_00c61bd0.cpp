// CClass_00c61bd0::ReleaseMember_00c61bd0() -- passes the member at +0x4 to the `__cdecl` function @ 0x00BFEB1C
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61BD0, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 4] ; push eax ; call 0xbfeb1c ; pop ecx ; ret 
// Tiny-body shape `ReleaseMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 

void __cdecl operator delete[](void* p);   // 0x00BFEB1C (MSVCR71)
struct CClass_00c61bd0 {
    unsigned char _pad[0x4];
    void* m_pBlock;                        // +0x4
    void ReleaseMember_00c61bd0();
};

void CClass_00c61bd0::ReleaseMember_00c61bd0()
{
    operator delete[](m_pBlock);
}