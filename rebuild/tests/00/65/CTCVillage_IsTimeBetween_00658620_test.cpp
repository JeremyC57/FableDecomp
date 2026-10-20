// behaviour test for 00658620 CTCVillage::IsTimeBetween: HHMM start times and an HHMM duration against the time of day
// (a 0..1 day fraction), including a window that wraps past midnight
#include <cstdio>
struct CGameTimeManager { void* World; void* GameComponent; float Time; };
struct CWorld { CGameTimeManager* PGameTimeManager; };
static CGameTimeManager g_time; static CWorld g_world;
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    const long* begin() const { return _M_start; }
    const long* end() const { return _M_finish; }
};
struct CTCVillage {
    virtual void v00() {}
    const CWorld* GetWorld() const { return &g_world; }
    bool IsTimeBetween(const CLongVector& startTimes, long duration) const;
};
bool CTCVillage::IsTimeBetween(const CLongVector& startTimes, long duration) const
{
    float time = GetWorld()->PGameTimeManager->Time;
    duration = duration / 100 * 60 + duration % 100;
    float length = duration * (1.0f / 1440.0f);
    for (const long* it = startTimes.begin(); it != startTimes.end(); ++it) {
        long minutes = *it / 100 * 60 + *it % 100;
        float start = minutes * (1.0f / 1440.0f);
        float end = length + start;
        if (end <= 1.0f && time > start && time < end)
            return true;
        if (end > 1.0f) {
            float wrapped = end - (long)end;
            if (time > start && time < 1.0f)
                return true;
            if (time > 0.0f && time < wrapped)
                return true;
        }
    }
    return false;
}
static float At(int h, int m) { return (h * 60 + m) / 1440.0f; }
int main()
{
    g_world.PGameTimeManager = &g_time;
    long starts[2] = { 900, 2300 };              // 09:00 and 23:00
    CLongVector v; v._M_start = starts; v._M_finish = starts + 2;
    CTCVillage village;
    g_time.Time = At(10, 0);  bool ok = village.IsTimeBetween(v, 130);   // 09:00-10:30
    g_time.Time = At(11, 0);  ok = ok && !village.IsTimeBetween(v, 130);
    g_time.Time = At(23, 30); ok = ok && village.IsTimeBetween(v, 200);  // 23:00-01:00 wraps
    g_time.Time = At(0, 30);  ok = ok && village.IsTimeBetween(v, 200);
    g_time.Time = At(2, 0);   ok = ok && !village.IsTimeBetween(v, 200);
    CLongVector empty; empty._M_start = starts; empty._M_finish = starts;
    ok = ok && !village.IsTimeBetween(empty, 2400);
    if (!ok) { std::printf("00658620_TEST FAIL\n"); return 1; }
    std::printf("00658620_TEST PASS\n");
    return 0;
}