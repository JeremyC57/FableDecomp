// Behaviour test for 0x00e3e6b0: CV_BordelloScript::GetHeroStatusTextTag: whore while tricking in a dress, lady in a dress, else man
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct B { bool tricking; bool dress; const char* Tag() { if (tricking && dress) return "_HEROWHORE"; if (dress) return "_HEROLADY"; return "_HEROMAN"; } };
static void run() { B b; b.tricking = true; b.dress = true; CHECK(!std::strcmp(b.Tag(), "_HEROWHORE")); b.tricking = false; CHECK(!std::strcmp(b.Tag(), "_HEROLADY")); b.dress = false; CHECK(!std::strcmp(b.Tag(), "_HEROMAN")); b.tricking = true; CHECK(!std::strcmp(b.Tag(), "_HEROMAN")); }

int main()
{
    run();
    if (g_fail) { std::printf("00e3e6b0_TEST FAIL\n"); return 1; }
    std::printf("00e3e6b0_TEST PASS\n");
    return 0;
}
