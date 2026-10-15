// Behaviour test for 0x004b1450 (CQuestManager::GetFullSectionNameForThing): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Thing { bool hasSupport; int dayNight; };
static void Suffix(const Thing& t, const char* sep, char* out) { out[0] = 0; if (t.dayNight == 1) { std::strcpy(out, sep); std::strcat(out, "DayOnly"); } else if (t.dayNight == 2) { std::strcpy(out, sep); std::strcat(out, "NightOnly"); } }
static void Full(const char* name, const Thing& t, char* out) { std::strcpy(out, name); if (t.hasSupport) { char s[32]; Suffix(t, "%", s); std::strcat(out, s); } }

int main()
{
    char out[64]; Thing a = { true, 2 }, b = { false, 1 }, c = { true, 0 };
    Full("Oakvale", a, out); CHECK(std::strcmp(out, "Oakvale%NightOnly") == 0);
    Full("Oakvale", b, out); CHECK(std::strcmp(out, "Oakvale") == 0);
    Full("Oakvale", c, out); CHECK(std::strcmp(out, "Oakvale") == 0);
    if (g_fail) { std::printf("004b1450_TEST FAIL\n"); return 1; }
    std::printf("004b1450_TEST PASS\n");
    return 0;
}