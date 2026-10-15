// CAppearance_00caddc0::Rebuild() -- under the critical section at +0x1c clear the four counters at +0x38..+0x44 and set the dirty word at +0x48 to 1; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CADDC0, length 47 bytes (48 with padding).
// Leaf: Rebuild
// Callees: KERNEL32 EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10).
// The chunk label "CTCGraphicAppearanceMultipleStaticMeshes::CTCGraphicAppearanceMultipleStaticMeshes" is propagated and wrong:
// third-party media code called only from 0x00CA499A and 0x00CA49CA (landed CAppearanceOwner_00ca4990::Rebuild, which
// declares this callee as CAppearance_00caddc0::Rebuild on the object at +0x50); the class keeps that name.
// Shape: the section address lives in edi for both calls; the four zero stores share the `xor eax,eax` register and are
// scheduled after the second call's `push edi`; the result needs a second `xor eax,eax` after the leave call.
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE10

struct CAppearance_00caddc0 {
    unsigned char         _pad00[0x1c];
    _RTL_CRITICAL_SECTION m_lock;                 // +0x1c
    unsigned char         _pad34[0x04];
    long                  m_count0;               // +0x38
    long                  m_count1;               // +0x3c
    long                  m_count2;               // +0x40
    long                  m_count3;               // +0x44
    long                  m_dirty;                // +0x48
    long Rebuild();
};

long CAppearance_00caddc0::Rebuild()
{
    EnterCriticalSection(&m_lock);
    m_count0 = 0;
    m_count1 = 0;
    m_count2 = 0;
    m_count3 = 0;
    m_dirty = 1;
    LeaveCriticalSection(&m_lock);
    return 0;
}