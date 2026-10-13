#include <cstdio>

struct CCharString { void* p; };
CCharString g_EmptyQuestName;
struct Q { unsigned char pad[0x2c]; CCharString* First; CCharString* Last;
    CCharString* Get(long i) { if (i >= 0 && (unsigned long)i < (unsigned long)(Last - First)) return &First[i]; return &g_EmptyQuestName; } };
int main()
{
    CCharString names[3]; Q q; q.First = names; q.Last = names + 3;
    if (q.Get(2) != &names[2] || q.Get(3) != &g_EmptyQuestName || q.Get(-1) != &g_EmptyQuestName) { std::printf("004af3c0_TEST FAIL\n"); return 1; }
    std::printf("004af3c0_TEST PASS\n");
    return 0;
}
