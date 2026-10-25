// Behaviour test for 00c1c810: PKCCriticalSection wrapper -- passes the embedded section to InitializeCriticalSection
#include <cstdio>
static int g_calls = 0; static void* g_last = 0;
static void InitializeCriticalSection(void* section) { ++g_calls; g_last = section; }
struct PKCCriticalSection { unsigned char m_section[0x18]; void Run() { InitializeCriticalSection(this); } };
static bool Check() { PKCCriticalSection cs; cs.Run(); return g_calls == 1 && g_last == &cs && sizeof(cs) == 0x18; }

int main()
{
    if (!Check()) { std::printf("00c1c810_TEST FAIL\n"); return 1; }
    std::printf("00c1c810_TEST PASS\n");
    return 0;
}
