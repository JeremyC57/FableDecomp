#include <cstdio>
// Behaviour test: CTCAISummon::GetAllSummonedCreatures -- purges entries whose creature has gone (null pointers) from the global list and returns it.
#define CHECK(c) if (!(c)) { std::printf("007fdeb0_TEST FAIL\n"); return 1; }
struct CThing { int id; };
static CThing* g_all[8]; static int g_count = 0; static int g_removes = 0;
static void RemoveNull() { ++g_removes; int j = 0; for (int i = 0; i < g_count; ++i) if (g_all[i] != 0) g_all[j++] = g_all[i]; g_count = j; }
static int* GetAllSummonedCreatures() { RemoveNull(); return &g_count; }
int main()
{
    CThing a, b; g_all[0] = &a; g_all[1] = 0; g_all[2] = &b; g_all[3] = 0; g_count = 4;
    int* list = GetAllSummonedCreatures();
    CHECK(list == &g_count && g_count == 2 && g_all[0] == &a && g_all[1] == &b && g_removes == 1)
    GetAllSummonedCreatures(); CHECK(g_count == 2 && g_removes == 2)
    std::printf("007fdeb0_TEST PASS\n");
    return 0;
}