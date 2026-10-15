// CStopWatch::Reset() -- clears the accumulated ticks and, when the watch is running, restarts it from the current tick count
// Leaf: Reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062F8B0, 76 bytes (+4 int3 padding). Callee: QueryPerformanceCounter
// (import @ 0x0143FE00) through the inlined GetTicks().
// Layout: Ego_r.pdb CStopWatch (0x20): m_fTimerPeriod +0x0 (float seconds per tick), m_nStartTick +0x8 (8-aligned
// __int64), m_nPrevElapsedTicks +0x10, m_bIsRunning +0x18. The class has no engine header; the PDB names are used.
// GetTicks() is QueryPerformanceCounter or 0 (0x0062F790), inlined into its callers in this module.
// Shape: the running flag is loaded before the immediate zero stores of the accumulated ticks; the inlined GetTicks()
// paths are duplicated.
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(__int64* count);       // 0x0143FE00 import
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceFrequency(__int64* frequency); // 0x0143FE04 import

class CStopWatch {
public:
    float   m_fTimerPeriod;                     // +0x00 seconds per tick
    __int64 m_nStartTick;                       // +0x08
    __int64 m_nPrevElapsedTicks;                // +0x10
    bool    m_bIsRunning;                       // +0x18

    CStopWatch();
    static __int64 GetTicks()
    {
        __int64 ticks;
        if (QueryPerformanceCounter(&ticks))
            return ticks;
        return 0;
    }
    void  Start();
    void  StartZero();
    void  Stop();
    void  Reset();
    float GetElapsedSeconds() const;
};

void CStopWatch::Reset()
{
    m_nPrevElapsedTicks = 0;
    if (m_bIsRunning)
        m_nStartTick = GetTicks();
}