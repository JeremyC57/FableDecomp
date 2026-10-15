// Behaviour test for 0x004af350 (CQuestManager::GetDayOrNightOnlyStringFromFullSectionName): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
static void After(const char* full, char* out)
{ const char* p = std::strchr(full, '%'); if (p) std::strcpy(out, p + 1); else out[0] = 0; }

int main()
{
    char out[64];
    After("Region%DayOnly", out); CHECK(std::strcmp(out, "DayOnly") == 0);
    After("Region", out); CHECK(out[0] == 0);
    After("%", out); CHECK(out[0] == 0);
    if (g_fail) { std::printf("004af350_TEST FAIL\n"); return 1; }
    std::printf("004af350_TEST PASS\n");
    return 0;
}
