// CCountedSection_00c5d700::Decrement() -- decrement the count at +0x14 inside the object's critical section at +0x20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D700, length 31 bytes.
// Callees: KERNEL32 EnterCriticalSection (IAT 0x0143FE0C) and LeaveCriticalSection (IAT 0x0143FE10), each used once (`call dword ptr [__imp]`:
// `__declspec(dllimport)`). Shape: the section address stays in edi for both calls, the decrement is scheduled between them. The chunk label
// "CEngine::ClipBox" is propagated; the class is named by address.
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE10

struct CCountedSection_00c5d700 {
    unsigned char         _pad[0x14];
    long                  m_count;                 // +0x14
    unsigned char         _pad18[8];
    _RTL_CRITICAL_SECTION m_section;               // +0x20
    void Decrement();
};

void CCountedSection_00c5d700::Decrement()
{
    EnterCriticalSection(&m_section);
    --m_count;
    LeaveCriticalSection(&m_section);
}
