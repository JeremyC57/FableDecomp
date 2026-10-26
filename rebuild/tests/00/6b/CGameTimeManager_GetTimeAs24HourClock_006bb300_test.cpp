// Behaviour test for 0x006BB300 CGameTimeManager::GetTimeAs24HourClock() const.
#include <cstdio>
#include <math.h>

class CGameTimeManager { public:
    void* World;
    void* GameComponent;
    float Time;
    unsigned int GetTimeAs24HourClock() const
    {
        float hours = Time * 24.0f;
        long minutes = (long)((hours - (float)floor((double)hours)) * 60.0f);
        return (unsigned int)((float)floor((double)hours) * 100.0f + minutes);
    }
};

int main()
{
    CGameTimeManager m;
    m.World = 0; m.GameComponent = 0;
    int ok = 1;
    m.Time = 0.0f;   if (m.GetTimeAs24HourClock() != 0) ok = 0;
    m.Time = 0.5f;   if (m.GetTimeAs24HourClock() != 1200) ok = 0;
    m.Time = 0.75f;  if (m.GetTimeAs24HourClock() != 1800) ok = 0;
    m.Time = 13.5f / 24.0f; if (m.GetTimeAs24HourClock() != 1330) ok = 0;    // 13:30
    m.Time = 23.75f / 24.0f; if (m.GetTimeAs24HourClock() != 2345) ok = 0;   // 23:45
    if (!ok) { std::printf("006bb300_TEST FAIL\n"); return 1; }
    std::printf("006bb300_TEST PASS\n");
    return 0;
}