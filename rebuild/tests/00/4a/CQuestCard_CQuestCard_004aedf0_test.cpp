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
    CQuestCard(const CCharString& defName, const CCharString& scriptName, bool replayable);
};
CQuestCard::CQuestCard(const CCharString& defName, const CCharString& scriptName, bool replayable)
{
    DefName = defName;
    ScriptName = scriptName;
    Replayable = replayable;
    FeatIndex = -1;
}
int main()
{
    CCharString def("QUEST_CARD_A"), script("QuestScriptA");
    CQuestCard a(def, script, true);
    CQuestCard b(script, def, false);
    if (a.DefName.m_data != def.m_data || a.ScriptName.m_data != script.m_data || !a.Replayable || a.FeatIndex != -1 ||
        b.DefName.m_data != script.m_data || b.ScriptName.m_data != def.m_data || b.Replayable || b.FeatIndex != -1 ||
        g_defaults != 4 || g_assigns != 4) {
        std::printf("004aedf0_TEST FAIL\n");
        return 1;
    }
    std::printf("004aedf0_TEST PASS\n");
    return 0;
}