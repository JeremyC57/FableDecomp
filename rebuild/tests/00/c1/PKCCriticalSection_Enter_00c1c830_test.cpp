// Behaviour test for 00c1c830: PKCCriticalSection wrapper -- passes the embedded section to EnterCriticalSection
#include <cstdio>
static int g_calls = 0; static void* g_last = 0;
static void EnterCriticalSection(void* section) { ++g_calls; g_last = section; }
struct PKCCriticalSection { unsigned char m_section[0x18]; void Run() { EnterCriticalSection(this); } };
static bool Check() { PKCCriticalSection cs; cs.Run(); return g_calls == 1 && g_last == &cs && sizeof(cs) == 0x18; }

int main()
{
    if (!Check()) { std::printf("00c1c830_TEST FAIL\n"); return 1; }
    std::printf("00c1c830_TEST PASS\n");
    return 0;
}
