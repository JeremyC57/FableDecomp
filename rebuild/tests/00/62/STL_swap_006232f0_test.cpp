// Behaviour test for 0x006232F0 std::swap<CTestQuest>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("006232f0_TEST FAIL\n"); return 1; } } while (0)

static int g_copies = 0, g_assigns = 0, g_dtors = 0;
class CCharString {                                     // test model: a short owned text
public:
    char Text[16];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strncpy(Text, s, 15); Text[15] = 0; }
    CCharString(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_copies; }
    ~CCharString() { ++g_dtors; }
    const CCharString& operator=(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_assigns; return *this; }
    bool Is(const char* s) const { return std::strcmp(Text, s) == 0; }
};

class CTestQuest {
public:
    CCharString QuestName;
    CCharString HSPName;
    long        ColourIndex;
    CCharString DisplayName;
    CCharString IniFile;
    CCharString OutiFile;
    CCharString QuestCardDefName;
    CTestQuest(const char* q, long c) : QuestName(q), HSPName("hsp"), ColourIndex(c), DisplayName(q), IniFile("ini"), OutiFile("outi"), QuestCardDefName(q) { }
};

template <class _Tp> inline void swap_model(_Tp& __a, _Tp& __b)    // STLport swap
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

int main()
{
    CTestQuest a("QuestA", 1);
    CTestQuest b("QuestB", 2);
    g_copies = g_assigns = g_dtors = 0;
    swap_model(a, b);
    CHECK(a.QuestName.Is("QuestB") && a.DisplayName.Is("QuestB") && a.QuestCardDefName.Is("QuestB") && a.ColourIndex == 2);
    CHECK(b.QuestName.Is("QuestA") && b.DisplayName.Is("QuestA") && b.QuestCardDefName.Is("QuestA") && b.ColourIndex == 1);
    CHECK(a.HSPName.Is("hsp") && b.IniFile.Is("ini") && b.OutiFile.Is("outi"));
    CHECK(g_copies == 6 && g_assigns == 12 && g_dtors == 6);
    std::printf("006232f0_TEST PASS\n");
    return 0;
}