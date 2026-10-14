// CLockedQueue_00ca9a40::HasPending() -- under the object's critical section at +0xa4, whether the unsigned count at +0xcc is above zero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA9A40, length 42 bytes.
// Callees: KERNEL32 EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10, one use each). Shape: `test ecx,ecx ; seta bl` is the unsigned
// `count > 0` into a callee-saved byte that survives the leave call (`mov al,bl`). The chunk label "CEngine::ClipBox" is propagated; the class is
// named by address.
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE10

struct CLockedQueue_00ca9a40 {
    unsigned char         _pad[0xa4];
    _RTL_CRITICAL_SECTION m_section;               // +0xa4
    unsigned char         _padbc[0x10];
    unsigned long         m_pendingCount;          // +0xcc
    bool HasPending();
};

bool CLockedQueue_00ca9a40::HasPending()
{
    EnterCriticalSection(&m_section);
    bool result = m_pendingCount > 0;
    LeaveCriticalSection(&m_section);
    return result;
}
