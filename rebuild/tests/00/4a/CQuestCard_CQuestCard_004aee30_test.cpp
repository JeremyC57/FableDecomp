#include <cstdio>
// CCharString test model: default construction and assignment (copy of the data pointer).
static int g_defaults = 0, g_assigns = 0;
struct CCharString {
    const char* m_data;
    CCharString() : m_data(0) { ++g_defaults; }
    CCharString(const char* text) : m_data(text) {}
    CCharString& operator=(const CCharString& other) { m_data = other.m_data; ++g_assigns; return *this; }
};
struct CQuestCard {
    CCharString DefName;
    CCharString ScriptName;
    bool        Replayable;
    long        FeatIndex;
    CQuestCard(const CCharString& defName, const CCharString& scriptName, long featIndex);
};
CQuestCard::CQuestCard(const CCharString& defName, const CCharString& scriptName, long featIndex)
{
    DefName = defName;
    ScriptName = scriptName;
    Replayable = false;
    FeatIndex = featIndex;
}
int main()
{
    CCharString def("QUEST_CARD_A"), script("QuestScriptA");
    CQuestCard a(def, script, 7L);
    CQuestCard b(script, def, -3L);
    if (a.DefName.m_data != def.m_data || a.ScriptName.m_data != script.m_data || a.Replayable || a.FeatIndex != 7 ||
        b.DefName.m_data != script.m_data || b.ScriptName.m_data != def.m_data || b.Replayable || b.FeatIndex != -3 ||
        g_defaults != 4 || g_assigns != 4) {
        std::printf("004aee30_TEST FAIL\n");
        return 1;
    }
    std::printf("004aee30_TEST PASS\n");
    return 0;
}