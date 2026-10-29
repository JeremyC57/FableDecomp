// Behaviour test for 0x004af450 NQuestManager::CRuntimeQuestPersistence::SaveToGameStateScript
#include <cstdio>
#include <cstring>

class CCharString { public:
    char text[256];
    CCharString() { text[0] = 0; }
    CCharString(const char* s) { std::strcpy(text, s); }
    const CCharString& operator+=(const CCharString& other) { std::strcat(text, other.text); return *this; }
    const CCharString& operator+=(const char* s) { std::strcat(text, s); return *this; }
};
CCharString operator+(const char* left, const CCharString& right) { CCharString r(left); r += right; return r; }
CCharString operator+(const CCharString& left, const char* right) { CCharString r(left); r += right; return r; }

struct CPersistInfo { CCharString QuestName; CCharString PersistScript; };
struct SPair { CCharString first; CPersistInfo second; };
struct CPersistMap { SPair* m_pFirst; SPair* m_pLast; SPair* m_pEnd; long m_sorted; };
struct CRuntimeQuestPersistence {
    CPersistMap PersistScripts;
    CPersistMap MasterDataPersistScripts;
    void SaveToGameStateScript(CCharString& script)
    {
        for (SPair* it = PersistScripts.m_pFirst; it != PersistScripts.m_pLast; ++it) {
            script += "\r\nSTART_NEW_QUEST " + it->second.QuestName + " \r\n";
            script += it->second.PersistScript;
            script += "END_NEW_QUEST\r\n";
        }
        for (SPair* it = MasterDataPersistScripts.m_pFirst; it != MasterDataPersistScripts.m_pLast; ++it) {
            script += "\r\nSTART_NEW_MASTER_QUEST_DATA " + it->second.QuestName + " \r\n";
            script += it->second.PersistScript;
            script += "END_NEW_MASTER_QUEST_DATA\r\n";
        }
    }
};

int main()
{
    SPair quests[2];
    quests[0].second.QuestName = CCharString("Q1"); quests[0].second.PersistScript = CCharString("a=1;");
    quests[1].second.QuestName = CCharString("Q2"); quests[1].second.PersistScript = CCharString("b=2;");
    SPair masters[1];
    masters[0].second.QuestName = CCharString("M"); masters[0].second.PersistScript = CCharString("m;");
    CRuntimeQuestPersistence p;
    p.PersistScripts.m_pFirst = quests; p.PersistScripts.m_pLast = quests + 2;
    p.MasterDataPersistScripts.m_pFirst = masters; p.MasterDataPersistScripts.m_pLast = masters + 1;
    CCharString script("X");
    p.SaveToGameStateScript(script);
    const char* expect = "X\r\nSTART_NEW_QUEST Q1 \r\na=1;END_NEW_QUEST\r\n\r\nSTART_NEW_QUEST Q2 \r\nb=2;END_NEW_QUEST\r\n"
                         "\r\nSTART_NEW_MASTER_QUEST_DATA M \r\nm;END_NEW_MASTER_QUEST_DATA\r\n";
    if (std::strcmp(script.text, expect) != 0) { std::printf("004af450_TEST FAIL\n"); return 1; }
    CRuntimeQuestPersistence e; e.PersistScripts.m_pFirst = e.PersistScripts.m_pLast = 0;
    e.MasterDataPersistScripts.m_pFirst = e.MasterDataPersistScripts.m_pLast = 0;
    CCharString empty("Y"); e.SaveToGameStateScript(empty);
    if (std::strcmp(empty.text, "Y") != 0) { std::printf("004af450_TEST FAIL\n"); return 1; }
    std::printf("004af450_TEST PASS\n");
    return 0;
}