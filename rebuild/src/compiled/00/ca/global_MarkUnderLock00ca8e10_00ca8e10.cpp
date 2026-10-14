// MarkUnderLock_00ca8e10(CLockedFlag* object) -- set the byte at +0x95 inside the object's critical section (+0x14): EnterCriticalSection, the store, LeaveCriticalSection; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA8E10, length 48 bytes.
// Callees: KERNEL32 EnterCriticalSection (IAT 0x0143FE0C) and LeaveCriticalSection (IAT 0x0143FE10), each through its import slot. Shape: static `__stdcall` (no `this`,
// `ret 4`), the section pointer kept in esi, `xor eax,eax`. The chunk label "CGraphicDataBank::ChangeScene" is propagated; named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10

struct CLockedFlag_00ca8e10 {
    unsigned char _pad[0x14];
    void*         m_pSection;                      // +0x14
    unsigned char _pad18[0x7d];
    bool          m_marked;                        // +0x95
};

long __stdcall MarkUnderLock_00ca8e10(CLockedFlag_00ca8e10* object)
{
    void* section = object->m_pSection;            // read once (esi) for both calls
    EnterCriticalSection(section);
    object->m_marked = true;
    LeaveCriticalSection(section);
    return 0;
}