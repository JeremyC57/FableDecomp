#include <cstdio>
int g_strings = 0;
struct CCharString { void* data; CCharString() : data(0) { ++g_strings; } };
struct CBase { int baseCtor; CBase() : baseCtor(1) { } };
struct CEventBase : CBase { bool triggered; long actionId, eventType, param; CEventBase(long a, long e, long p) : CBase(), triggered(false), actionId(a), eventType(e), param(p) { } };
struct CExplosion : CEventBase { CCharString name; long index; bool f1c, f1d; CExplosion(long a, long e, long p) : CEventBase(a, e, p), name() { index = -1; f1c = false; f1d = false; } };
int main()
{
    CExplosion x(7, 8, 9);
    bool ok = x.baseCtor == 1 && !x.triggered && x.actionId == 7 && x.eventType == 8 && x.param == 9 && x.name.data == 0 && g_strings == 1 && x.index == -1 && !x.f1c && !x.f1d;
    if (!ok) { std::printf("006955f0_TEST FAIL\n"); return 1; }
    std::printf("006955f0_TEST PASS\n"); return 0;
}