// CLockedOwner_00ca4b60::HasOwner() -- under the object's critical section at +0x94, whether the pointer at +0x6c is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4B60, length 41 bytes.
// Callees: KERNEL32 EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10, one use each: `call dword ptr [__imp]`). Shape: the result is a
// long formed through the full register (`xor eax,eax ; test ; setne al`), kept in esi across the leave call and moved to eax. The chunk
// label "CEngine::ClipBox" is propagated (the 0x00CAxxxx module); the class is named by address.
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE10

struct CLockedOwner_00ca4b60 {
    unsigned char         _pad[0x6c];
    void*                 m_pOwner;                // +0x6c
    unsigned char         _pad70[0x24];
    _RTL_CRITICAL_SECTION m_section;               // +0x94
    long HasOwner();                               // a long result (`xor eax,eax ; setne al`, kept whole in esi)
};

long CLockedOwner_00ca4b60::HasOwner()
{
    EnterCriticalSection(&m_section);
    long result = m_pOwner != 0;
    LeaveCriticalSection(&m_section);
    return result;
}