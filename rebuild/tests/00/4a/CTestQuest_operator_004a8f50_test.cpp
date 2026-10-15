#include <cstdio>
#include <cstring>
static int g_copies = 0, g_assigns = 0;
struct CCharString {
    char text[16];
    CCharString(const char* t) { std::strcpy(text, t); }
    CCharString(const CCharString& o) { ++g_copies; std::strcpy(text, o.text); }
    CCharString& operator=(const CCharString& o) { ++g_assigns; std::strcpy(text, o.text); return *this; }
};
struct CTestQuest {
    CCharString QuestName, HSPName; long ColourIndex; CCharString DisplayName, IniFile, OutiFile, QuestCardDefName;
    CTestQuest(const char* n, long c) : QuestName(n), HSPName("hsp"), ColourIndex(c), DisplayName("disp"), IniFile("ini"), OutiFile("outi"), QuestCardDefName("card") { }
    CTestQuest(const CTestQuest& o) : QuestName(o.QuestName), HSPName(o.HSPName), ColourIndex(o.ColourIndex), DisplayName(o.DisplayName),
        IniFile(o.IniFile), OutiFile(o.OutiFile), QuestCardDefName(o.QuestCardDefName) { }
    CTestQuest& operator=(const CTestQuest& o)
    {
        QuestName = o.QuestName; HSPName = o.HSPName; ColourIndex = o.ColourIndex; DisplayName = o.DisplayName;
        IniFile = o.IniFile; OutiFile = o.OutiFile; QuestCardDefName = o.QuestCardDefName; return *this;
    }
};
int main()
{
    CTestQuest a("q1", 5);
    CTestQuest b("q2", 9); CTestQuest* r = &(b = a); bool ok = r == &b && g_assigns == 6 && g_copies == 0;
    ok = ok && !std::strcmp(b.QuestName.text, "q1") && b.ColourIndex == 5 && !std::strcmp(b.QuestCardDefName.text, "card") && !std::strcmp(b.IniFile.text, "ini");
    if (!ok) { std::printf("004a8f50_TEST FAIL\n"); return 1; }
    std::printf("004a8f50_TEST PASS\n");
    return 0;
}