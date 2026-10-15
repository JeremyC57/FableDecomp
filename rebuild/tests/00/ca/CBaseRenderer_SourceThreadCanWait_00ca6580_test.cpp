#include <cstdio>
static int g_set = 0, g_reset = 0;
struct CAMEvent { void* m_hEvent; void Set() { ++g_set; } void Reset() { ++g_reset; } };
struct CBaseRenderer {
    CAMEvent m_ThreadSignal;
    long SourceThreadCanWait(int bCanWait) { if (bCanWait == 1) m_ThreadSignal.Reset(); else m_ThreadSignal.Set(); return 0; }
};
int main()
{
    CBaseRenderer r;
    bool ok = r.SourceThreadCanWait(1) == 0 && g_reset == 1 && g_set == 0 && r.SourceThreadCanWait(0) == 0 && g_set == 1 && r.SourceThreadCanWait(2) == 0 && g_set == 2;
    if (!ok) { std::printf("00ca6580_TEST FAIL\n"); return 1; }
    std::printf("00ca6580_TEST PASS\n");
    return 0;
}