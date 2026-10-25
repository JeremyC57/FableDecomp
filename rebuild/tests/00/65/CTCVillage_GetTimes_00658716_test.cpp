// Behaviour test for 0x00658716: CTCVillage::GetTimes(const std::vector<long>& startTimes, long duration, float& timeUntilStart, float& endTime) const -- for HHMM start times and an HHMM duration, report whether a session is running now (timeUntilStart 0, endTime its end) or else the day fraction until the nearest next start (today or tomorrow) and that session's end
// A copy of the source model and logic (compiled separately from src/00658716.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}

class CGameTimeManager {
public:
    class CWorld* World;                               // +0x00
    void*         GameComponent;                       // +0x04
    float         Time;                                // +0x08 time of day, 0..1
};
class CWorld {
public:
    unsigned char     _pad00[0x1c];
    CGameTimeManager* PGameTimeManager;                // +0x1C scoped_ptr<CGameTimeManager>
};
class CTCBase {
public:
    const CWorld& PeekWorld() const;                   // 0x00686A80
};

static inline float HHMMToDayFraction(long time)
{
    return (time / 100 * 60 + time % 100) * (1.0f / 1440.0f);
}

class CTCVillage : public CTCBase {
public:
    bool GetSchoolTimes(float& start, float& end) const;
public:
    bool GetTimes(const std::vector<long>& startTimes, long duration, float& timeUntilStart, float& endTime) const;
};

bool CTCVillage::GetTimes(const std::vector<long>& startTimes, long duration, float& timeUntilStart, float& endTime) const
{
    const CGameTimeManager* timeManager = PeekWorld().PGameTimeManager;
    float now = timeManager->Time;
    float length = HHMMToDayFraction(duration);
    timeUntilStart = 0.0f;
    endTime = 0.0f;
    float best = -1.0f;
    for (std::vector<long>::const_iterator it = startTimes.begin(); it != startTimes.end(); ++it) {
        float start = HHMMToDayFraction(*it);
        float end = start + length;
        if (end < 1.0f) {
            if (now > start && now < end) {
                timeUntilStart = 0.0f;
                endTime = end;
                return true;
            }
        }
        if (end > 1.0f) {
            float wrappedEnd = end - (long)end;
            if ((now > start && now < 1.0f) || (now > 0.0f && now < wrappedEnd)) {
                timeUntilStart = 0.0f;
                endTime = end;
                return true;
            }
        }
        if (now < start) {
            if (best == -1.0f || start - now < best) {
                best = start - now;
                endTime = end;
            }
        }
        if (now - 1.0f < start) {
            if (best == -1.0f || start - (now - 1.0f) < best) {
                best = start - (now - 1.0f);
                endTime = end;
            }
        }
    }
    timeUntilStart = best;
    if (endTime > 1.0f)
        endTime -= 1.0f;
    return true;
}

// ---- test stand-ins ----
static CGameTimeManager g_time;
static CWorld g_world;
const CWorld& CTCBase::PeekWorld() const { return g_world; }

static int Fail(int line) { std::printf("00658716_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)
static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
static float Day(long minutes) { return (float)minutes / 1440.0f; }

int main()
{
    g_world.PGameTimeManager = &g_time;
    CTCVillage village;
    long times[2] = { 900, 1400 };                 // 09:00 and 14:00
    std::vector<long> starts;
    starts._M_start = times; starts._M_finish = times + 2; starts._M_end_of_storage = times + 2;
    float until = 5.0f, end = 5.0f;

    // 10:00, inside 09:00..11:30
    g_time.Time = Day(600);
    CHECK(village.GetTimes(starts, 230, until, end));
    CHECK(Near(until, 0.0f) && Near(end, Day(690)));

    // 12:00: next start 14:00 today, ending 16:30
    g_time.Time = Day(720);
    CHECK(village.GetTimes(starts, 230, until, end));
    CHECK(Near(until, Day(120)) && Near(end, Day(990)));

    // 20:00: both passed, next is 09:00 tomorrow
    g_time.Time = Day(1200);
    CHECK(village.GetTimes(starts, 230, until, end));
    CHECK(Near(until, Day(780)) && Near(end, Day(690)));

    // a session past midnight: 23:00 for 2:00
    long late[1] = { 2300 };
    std::vector<long> lateStarts;
    lateStarts._M_start = late; lateStarts._M_finish = late + 1; lateStarts._M_end_of_storage = late + 1;
    g_time.Time = Day(30);                          // 00:30, in the wrapped part
    CHECK(village.GetTimes(lateStarts, 200, until, end));
    CHECK(Near(until, 0.0f) && Near(end, Day(1500)));
    g_time.Time = Day(1410);                        // 23:30, before midnight
    CHECK(village.GetTimes(lateStarts, 200, until, end));
    CHECK(Near(until, 0.0f) && Near(end, Day(1500)));
    g_time.Time = Day(1320);                        // 22:00, an hour before; end wrapped to 01:00
    CHECK(village.GetTimes(lateStarts, 200, until, end));
    CHECK(Near(until, Day(60)) && Near(end, Day(60)));

    // no start times: -1 and 0
    std::vector<long> none;
    none._M_start = 0; none._M_finish = 0; none._M_end_of_storage = 0;
    CHECK(village.GetTimes(none, 230, until, end));
    CHECK(Near(until, -1.0f) && Near(end, 0.0f));
    std::printf("00658716_TEST PASS\n");
    return 0;
}