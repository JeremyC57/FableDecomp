#include <cstdio>
struct _RTL_CRITICAL_SECTION { long count; long entered; long tried; };
static int TryEnterCriticalSection(_RTL_CRITICAL_SECTION* cs) { ++cs->tried; return cs->count == 0 ? (cs->count = 1) : 0; }
static void EnterCriticalSection(_RTL_CRITICAL_SECTION* cs) { ++cs->entered; cs->count = 1; }
struct CCriticalSectionLock {
    _RTL_CRITICAL_SECTION* CriticalSection; bool Locked;
    CCriticalSectionLock(_RTL_CRITICAL_SECTION* cs, bool tryOnly) {
        CriticalSection = cs;
        if (tryOnly) { Locked = TryEnterCriticalSection(cs) != 0; } else { EnterCriticalSection(cs); Locked = true; }
    }
};
int main()
{
    _RTL_CRITICAL_SECTION cs = { 0, 0, 0 };
    CCriticalSectionLock a(&cs, true);  if (!a.Locked || cs.tried != 1 || cs.entered != 0 || a.CriticalSection != &cs) { std::printf("00a62920_TEST FAIL\n"); return 1; }
    CCriticalSectionLock b(&cs, true);  if (b.Locked || cs.tried != 2) { std::printf("00a62920_TEST FAIL\n"); return 1; }
    CCriticalSectionLock c(&cs, false); if (!c.Locked || cs.entered != 1 || cs.tried != 2) { std::printf("00a62920_TEST FAIL\n"); return 1; }
    std::printf("00a62920_TEST PASS\n"); return 0;
}
