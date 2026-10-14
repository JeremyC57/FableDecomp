// CClass_006e4b80::~CClass_006e4b80() -- frees the owned block at +0 when non-null (free @ 0x00BFEA14); size TU
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E4B80, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx] ; test eax, eax ; je 0x6e4b8d ; push eax ; call 0xbfea14 ; pop ecx ; ret 
// Tiny-body shape `DtorFree`: the destructor frees its one owned block when non-null; the size
// TU cleans the argument with `pop ecx`. The class comes from the manifest label when it is a
// plain class name; member names are descriptive.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);

struct CClass_006e4b80 {
    void* m_pData;                         // +0
    ~CClass_006e4b80();
};

CClass_006e4b80::~CClass_006e4b80()
{
    if (m_pData)
        free(m_pData);
}