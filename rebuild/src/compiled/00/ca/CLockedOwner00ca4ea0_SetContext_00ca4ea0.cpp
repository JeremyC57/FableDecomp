// CLockedOwner_00ca4ea0::SetContext(long) -- under the object's critical section at +0x94, store the argument at +0xb0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4EA0, length 37 bytes.
// Callees: KERNEL32 EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10, one use each). Shape: the argument is read after the enter
// call and stored between the section push and the leave call; `ret 4`. The chunk label "CEngine::ClipBox" is propagated; the class is named by address.
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE10

struct CLockedOwner_00ca4ea0 {
    unsigned char         _pad[0x94];
    _RTL_CRITICAL_SECTION m_section;               // +0x94
    unsigned char         _padac[4];
    long                  m_context;               // +0xb0
    void SetContext(long context);
};

void CLockedOwner_00ca4ea0::SetContext(long context)
{
    EnterCriticalSection(&m_section);
    m_context = context;
    LeaveCriticalSection(&m_section);
}