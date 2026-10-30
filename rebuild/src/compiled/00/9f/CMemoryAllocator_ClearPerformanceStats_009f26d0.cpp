// CMemoryAllocator::ClearPerformanceStats @ 009f26b0
class CMemoryAllocator;

// stdcall so the callee cleans up its own 4-byte arg (no "add esp,4" after call).

// The object whose address (0x13caa70) is pushed to both calls.
extern int g_perfStatsObject;

// The IAT slots [0x143fe0c] and [0x143fe10]: KERNEL32.dll!EnterCriticalSection / LeaveCriticalSection.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void*);

void __fastcall ClearPerformanceStats(CMemoryAllocator* self)
{
    (void)self;
    EnterCriticalSection(&g_perfStatsObject);
    LeaveCriticalSection(&g_perfStatsObject);
}