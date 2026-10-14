// CClass_0071efb0::~CClass_0071efb0() -- frees the owned block at +0 when non-null (free @ 0x00BFEA14); size TU
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071EFB0, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx] ; test eax, eax ; je 0x71efbd ; push eax ; call 0xbfea14 ; pop ecx ; ret 
// Tiny-body shape `DtorFree`: the destructor frees its one owned block when non-null; the size
// TU cleans the argument with `pop ecx`. The class comes from the manifest label when it is a
// plain class name; member names are descriptive.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);

struct CClass_0071efb0 {
    void* m_pData;                         // +0
    ~CClass_0071efb0();
};

CClass_0071efb0::~CClass_0071efb0()
{
    if (m_pData)
        free(m_pData);
}