// Behaviour test for CQuestManager::SetQuestCardRenownReward(const CCharString&, long) @ 0x004B0EB0
#include <cstdio>

struct CTCBase { int m_dummy; };
struct CTCQuestCard : CTCBase { long GoldReward; long RenownReward; };
struct CCharString { const char* m_text; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last; SEntry* end;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};

struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_dead : 1;
    unsigned char m_91 : 7;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

static CThing* g_card = 0;

struct CQuestManager {
    CThing* GetActiveQuestCardFromScriptName(const CCharString&) { return g_card; }
    void SetQuestCardRenownReward(const CCharString& scriptName, long reward)
    {
        CThing* card = GetActiveQuestCardFromScriptName(scriptName);
        if (card && !card->m_dead) {
            CTCQuestCard* questCard = 0;      // (retail leaves it uninitialised; the test only runs the present path)
            card->GetInterface(&questCard, 0x6c);
            if (questCard)
                questCard->RenownReward = reward;
        }
    }
};

int main()
{
    CTCQuestCard tc; tc.GoldReward = 0; tc.RenownReward = 0;
    CInterfaceMap::SEntry entries[3] = { { 5, 0 }, { 0x6c, &tc }, { 0, 0 } };
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[3] = 1UL << 12;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2; thing.m_interfaces.end = entries + 2;
    thing.m_dead = 0; thing.m_91 = 0;
    CQuestManager m; CCharString s = { "Q_X" };
    m.SetQuestCardRenownReward(s, 500);                  // no card: nothing happens
    g_card = &thing;
    m.SetQuestCardRenownReward(s, 750);
    if (tc.RenownReward != 750) { std::printf("004b0eb0_TEST FAIL\n"); return 1; }
    thing.m_dead = 1;
    m.SetQuestCardRenownReward(s, 9);
    if (tc.RenownReward != 750) { std::printf("004b0eb0_TEST FAIL\n"); return 1; }
    if ((tc.GoldReward == 750) == (tc.RenownReward == 750)) { std::printf("004b0eb0_TEST FAIL\n"); return 1; }
    std::printf("004b0eb0_TEST PASS\n");
    return 0;
}