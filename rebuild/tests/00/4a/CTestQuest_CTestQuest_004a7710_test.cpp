// Behaviour test for CTestQuest::CTestQuest (0x004A7710): six strings constructed in declaration order, the colour index untouched.
#include <cstdio>
#include <new>
static int g_log[8]; static int g_n = 0;
class CCharString { public: int order; CCharString() { order = g_n; g_log[g_n++] = 1; } };
class CTestQuest { public: CCharString QuestName; CCharString HSPName; long ColourIndex; CCharString DisplayName; CCharString IniFile; CCharString OutiFile; CCharString QuestCardDefName; };
int main()
{
    static unsigned char buf[sizeof(CTestQuest) + 16];
    CTestQuest* q = reinterpret_cast<CTestQuest*>(buf);
    q->ColourIndex = 77;
    new (buf) CTestQuest;
    if (g_n != 6 || q->QuestName.order != 0 || q->HSPName.order != 1 || q->DisplayName.order != 2 || q->IniFile.order != 3 ||
        q->OutiFile.order != 4 || q->QuestCardDefName.order != 5 || q->ColourIndex != 77) { std::printf("004a7710_TEST FAIL\n"); return 1; }
    std::printf("004a7710_TEST PASS\n");
    return 0;
}