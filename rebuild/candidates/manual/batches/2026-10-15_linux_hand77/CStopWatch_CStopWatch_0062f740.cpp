// CStopWatch::CStopWatch() -- zeroes the watch and sets the timer period to 1/QueryPerformanceFrequency (1.0 when the performance counter is unavailable)
// Leaf: ??0CStopWatch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062F740, 70 bytes (+10 int3 padding). Callees: QueryPerformanceFrequency
// (import @ 0x0143FE04); the 1.0f constant @ 0x0122DED8.
// Layout: Ego_r.pdb CStopWatch (0x20): m_fTimerPeriod +0x0 (float seconds per tick), m_nStartTick +0x8 (8-aligned
// __int64), m_nPrevElapsedTicks +0x10, m_bIsRunning +0x18. The class has no engine header; the PDB names are used.
// GetTicks() is QueryPerformanceCounter or 0 (0x0062F790), inlined into its callers in this module.
// Shape: every member is zeroed from one register in the init list (the float too), the frequency lands in an 8-byte
// frame local, `fild ; fdivr [1.0f]` is `1.0f / frequency`; the failure arm stores the 1.0f bit pattern as an immediate.
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

CStopWatch::CStopWatch()
    : m_fTimerPeriod(0), m_nStartTick(0), m_nPrevElapsedTicks(0), m_bIsRunning(false)
{
    __int64 frequency;
    if (QueryPerformanceFrequency(&frequency))
        m_fTimerPeriod = 1.0f / frequency;
    else
        m_fTimerPeriod = 1.0f;
}
