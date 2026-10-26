// CMemoryAllocator::ConsolidateFreeAreas() -- under the allocator lock, ask every sub-allocator of the global table (0x01396880, 0x1c-byte entries, count at +0x410) to consolidate its free areas (virtual slot 13, +0x34)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2660, length 80 bytes.
// Leaf: ConsolidateFreeAreas
// Callees: EnterCriticalSection / LeaveCriticalSection on the allocator lock @ 0x013CAA70, the sub-allocators' virtual slot 13. Shape: the table pointer stepped by 0x1c in edi, the count re-read after every call, `ret`.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10
extern unsigned char g_allocatorLock_013caa70[24];   // 0x013CAA70: the allocator critical section

struct IMemoryAllocator_009f2450 {                 // the per-size allocators: virtual statistics slots
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual unsigned long GetHeaderOverhead();     // slot 7 (+0x1c)
    virtual void v008();
    virtual unsigned long GetWastedSpace();        // slot 9 (+0x24)
    virtual void v010(); virtual void v011(); virtual void v012();
    virtual void ConsolidateFreeAreas();           // slot 13 (+0x34)
};
struct SAllocatorEntry_009f2450 {                  // 0x1c bytes per sub-allocator in the global table
    IMemoryAllocator_009f2450* m_pAllocator;       // +0x00
    unsigned char              _pad04[0x18];
};
extern SAllocatorEntry_009f2450 g_subAllocators_01396880[];   // 0x01396880

class CMemoryAllocator { public:
    unsigned char _pad00[0x410];
    unsigned long m_noSubAllocators;               // +0x410
    unsigned long GetHeaderOverhead();
    unsigned long GetWastedSpace();
    void ConsolidateFreeAreas();
};

void CMemoryAllocator::ConsolidateFreeAreas()
{
    EnterCriticalSection(g_allocatorLock_013caa70);
    for (unsigned long i = 0; i < m_noSubAllocators; ++i)
        g_subAllocators_01396880[i].m_pAllocator->ConsolidateFreeAreas();
    LeaveCriticalSection(g_allocatorLock_013caa70);
}