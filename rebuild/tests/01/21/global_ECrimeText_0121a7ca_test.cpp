// Test for $E_CrimeText (0x0121A7CA): the anonymous-namespace CCharString CrimeText[10] is built in index order --
// "" for crime 0, then the nine TEXT_GUI_CRIME_* text keys -- and one atexit destructor tears the array down in reverse.
#include <cstdio>
#include <cstring>
static int g_order[10]; static int g_built = 0; static int g_destroyed[10]; static int g_dead = 0;
class CCharString { public:
    char text[40]; int index;
    void Init(const char* s, int i) { std::strcpy(text, s); index = i; g_order[g_built++] = i; }
    void Destroy() { g_destroyed[g_dead++] = index; }
};
static CCharString CrimeText[10];
static const char* const kKeys[10] = {
    "", "TEXT_GUI_CRIME_WEAPON_OUT", "TEXT_GUI_CRIME_TRESPASS", "TEXT_GUI_CRIME_VANDALISM", "TEXT_GUI_CRIME_LOCKPICKING",
    "TEXT_GUI_CRIME_PICK_POCKETS", "TEXT_GUI_CRIME_THEFT", "TEXT_GUI_CRIME_ASSAULT", "TEXT_GUI_CRIME_GBH", "TEXT_GUI_CRIME_MURDER" };
static void DynamicInitializer() { for (int i = 0; i < 10; ++i) CrimeText[i].Init(kKeys[i], i); }
static void DynamicAtexitDestructor() { for (int i = 10; i-- != 0; ) CrimeText[i].Destroy(); }
int main()
{
    DynamicInitializer();
    if (g_built != 10 || CrimeText[0].text[0] != 0) { std::printf("0121a7ca_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 10; ++i) if (g_order[i] != i || std::strcmp(CrimeText[i].text, kKeys[i]) != 0) { std::printf("0121a7ca_TEST FAIL\n"); return 1; }
    if (std::strcmp(CrimeText[9].text, "TEXT_GUI_CRIME_MURDER") != 0 || std::strcmp(CrimeText[1].text, "TEXT_GUI_CRIME_WEAPON_OUT") != 0) { std::printf("0121a7ca_TEST FAIL\n"); return 1; }
    DynamicAtexitDestructor();
    for (int i = 0; i < 10; ++i) if (g_destroyed[i] != 9 - i) { std::printf("0121a7ca_TEST FAIL\n"); return 1; }
    std::printf("0121a7ca_TEST PASS\n");
    return 0;
}