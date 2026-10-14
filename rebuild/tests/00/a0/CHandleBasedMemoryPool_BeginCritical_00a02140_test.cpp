#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00A02140: exercises the same logic through a copy of the model (compiled separately from the source)
struct _RTL_CRITICAL_SECTION_Retail { long m_words[6]; };
static int g_try = 0, g_enter = 0, g_tryResult = 1;
static int TryEnterCriticalSection(_RTL_CRITICAL_SECTION_Retail*) { ++g_try; return g_tryResult; }
static void EnterCriticalSection(_RTL_CRITICAL_SECTION_Retail*) { ++g_enter; }
struct CHandleBasedMemoryPool {
    bool MultiThreaded; _RTL_CRITICAL_SECTION_Retail CriticalSection;
    bool BeginCritical(bool tryOnly)
    {
        if (!MultiThreaded) return true;
        if (tryOnly) { if (!TryEnterCriticalSection(&CriticalSection)) return false; }
        else EnterCriticalSection(&CriticalSection);
        return true;
    }
};
static bool Run()
{
    CHandleBasedMemoryPool pool; pool.MultiThreaded = false;
    if (!pool.BeginCritical(true) || g_try != 0) return false;
    pool.MultiThreaded = true;
    if (!pool.BeginCritical(false) || g_enter != 1) return false;
    if (!pool.BeginCritical(true) || g_try != 1) return false;
    g_tryResult = 0;
    return !pool.BeginCritical(true) && g_try == 2 && g_enter == 1;
}
int main()
{
    if (!Run()) { std::printf("00a02140_TEST FAIL\n"); return 1; }
    std::printf("00a02140_TEST PASS\n");
    return 0;
}