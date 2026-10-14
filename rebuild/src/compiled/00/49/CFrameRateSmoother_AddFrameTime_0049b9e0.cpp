// CFrameRateSmoother::AddFrameTime(double time) -- when the ring (+8, capacity +0x14) is full, advance the start index (+0x20) and drop the count (+0x18); store the time as a float at the next index (+0x24), advance it, bump the count and recompute the smoothed time
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049B9E0, length 64 bytes.
// Callees: CalculateSmoothedTime @ 0x0049B9A0 (on `this`). Shape: both ring advances through `inc ; cdq ; idiv`, the double argument narrowed with `fld qword ;
// fstp dword`, the count incremented in memory before the index store, `ret 8`. No size pragma: `optimize("s")` makes the lane copy the double through `fld st(0)`.
struct CFrameRateSmoother {
    unsigned char _pad[8];
    float*        m_pSamples;                      // +0x08
    long          m_c;
    long          m_10;
    long          m_capacity;                      // +0x14
    long          m_count;                         // +0x18
    long          m_1c;
    long          m_start;                         // +0x20
    long          m_next;                          // +0x24
    double        m_smoothedTime;                  // +0x28
    void CalculateSmoothedTime();                  // 0x0049B9A0
    void AddFrameTime(double time);
};

void CFrameRateSmoother::AddFrameTime(double time)
{
    if (m_count == m_capacity) {
        m_start = (m_start + 1) % m_capacity;
        --m_count;
    }
    m_pSamples[m_next] = (float)time;
    m_next = (m_next + 1) % m_capacity;
    ++m_count;
    CalculateSmoothedTime();
}