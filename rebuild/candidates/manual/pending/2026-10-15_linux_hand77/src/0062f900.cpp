// CStopWatch::GetElapsedSeconds() const -- the accumulated ticks plus, while running, the ticks since the start, converted to seconds with the timer period
// Leaf: GetElapsedSeconds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062F900, 93 bytes. Callee: QueryPerformanceCounter (import @ 0x0143FE00)
// through the inlined GetTicks().
// Layout: Ego_r.pdb CStopWatch (0x20): m_fTimerPeriod +0x0 (float seconds per tick), m_nStartTick +0x8 (8-aligned
// __int64), m_nPrevElapsedTicks +0x10, m_bIsRunning +0x18. The class has no engine header; the PDB names are used.
// GetTicks() is QueryPerformanceCounter or 0 (0x0062F790), inlined into its callers in this module.
// Shape: the running total is an 8-byte frame local seeded from m_nPrevElapsedTicks, `fild qword ; fmul [esi]` is
// `ticks * m_fTimerPeriod`.
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

float CStopWatch::GetElapsedSeconds() const
{
    __int64 ticks = m_nPrevElapsedTicks;
    if (m_bIsRunning)
        ticks += GetTicks() - m_nStartTick;
    return ticks * m_fTimerPeriod;
}
