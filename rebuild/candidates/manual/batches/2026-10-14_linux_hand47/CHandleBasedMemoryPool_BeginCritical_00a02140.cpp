// CHandleBasedMemoryPool::BeginCritical(bool tryOnly) -- enter the pool's critical section when the pool is multi-threaded (TryEnterCriticalSection when `tryOnly`, reporting its failure), else nothing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02140, length 48 bytes.
// Callees: KERNEL32 TryEnterCriticalSection (IAT 0x0143FD30), EnterCriticalSection (IAT 0x0143FE0C). Shape: the MultiThreaded byte (+0x24) first, the section's address (+0x44c)
// formed once before the flag test, `xor al,al` on the failed try, `mov al,1` shared by the other exits, `ret 4`.
// Layout: engine/CHandleBasedMemoryPool.h puts the critical section at +0x450 behind a 16-byte Handles CArray; retail's CArray is 12 bytes, so the section sits at +0x44c (a local model).
struct _RTL_CRITICAL_SECTION_Retail { long m_words[6]; };   // 24 bytes
extern "C" __declspec(dllimport) int  __stdcall TryEnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);   // IAT 0x0143FD30
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);      // IAT 0x0143FE0C

struct CHandleBasedMemoryPool {
    unsigned char                _pad[0x24];
    bool                         MultiThreaded;    // +0x24
    unsigned char                _pad25[0x44c - 0x25];
    _RTL_CRITICAL_SECTION_Retail CriticalSection;  // +0x44c (PDB +0x450: retail CArray<CHandleInfo> is 12 bytes)
    bool BeginCritical(bool tryOnly);
};

bool CHandleBasedMemoryPool::BeginCritical(bool tryOnly)
{
    if (!MultiThreaded)
        return true;
    if (tryOnly) {
        if (!TryEnterCriticalSection(&CriticalSection))
            return false;
    } else {
        EnterCriticalSection(&CriticalSection);
    }
    return true;
}
