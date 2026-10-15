// Behaviour test for CQuestManager::OnQuestFinishScreenDisplayed(const CCharString&) @ 0x004B0160
#include <cstdio>
#include <string.h>

struct CCharString { const char* m_text; };
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

static CCharString* FindString_004b8e40(CCharString* first, CCharString* last, const CCharString& value, const SRandomAccessIteratorTag&)
{
    for (; first != last; ++first)
        if (strcmp(first->m_text, value.m_text) == 0) break;
    return first;
}

struct CCharStringVector {
    CCharString* m_pFirst; CCharString* m_pLast; CCharString* m_pEnd;
    CCharString* erase(CCharString* where)
    {
        for (CCharString* p = where; p + 1 != m_pLast; ++p) *p = *(p + 1);
        --m_pLast;
        return where;
    }
};

struct CQuestManager {
    CCharStringVector BetweenStartAndEndScreenQuests;
    long              NumberOfQuestsBetweenStartAndFinishScreens;
    bool OnQuestFinishScreenDisplayed(const CCharString& quest)
    {
        CCharString* last = BetweenStartAndEndScreenQuests.m_pLast;
        CCharStringVector* quests = &BetweenStartAndEndScreenQuests;
        CCharString* it = FindString_004b8e40(quests->m_pFirst, last, quest, SRandomAccessIteratorTag());
        if (it != last) {
            if (NumberOfQuestsBetweenStartAndFinishScreens > 0)
                --NumberOfQuestsBetweenStartAndFinishScreens;
            quests->erase(it);
            return true;
        }
        return false;
    }
};

int main()
{
    CCharString storage[3] = { { "Q_A" }, { "Q_B" }, { "Q_C" } };
    CQuestManager m;
    m.BetweenStartAndEndScreenQuests.m_pFirst = storage;
    m.BetweenStartAndEndScreenQuests.m_pLast = storage + 3;
    m.BetweenStartAndEndScreenQuests.m_pEnd = storage + 3;
    m.NumberOfQuestsBetweenStartAndFinishScreens = 1;
    CCharString b = { "Q_B" }, x = { "Q_X" }, c = { "Q_C" };
    if (!m.OnQuestFinishScreenDisplayed(b) || m.NumberOfQuestsBetweenStartAndFinishScreens != 0 || m.BetweenStartAndEndScreenQuests.m_pLast != storage + 2 || strcmp(storage[1].m_text, "Q_C") != 0) { std::printf("004b0160_TEST FAIL\n"); return 1; }
    if (m.OnQuestFinishScreenDisplayed(x)) { std::printf("004b0160_TEST FAIL\n"); return 1; }
    if (!m.OnQuestFinishScreenDisplayed(c) || m.NumberOfQuestsBetweenStartAndFinishScreens != 0 || m.BetweenStartAndEndScreenQuests.m_pLast != storage + 1) { std::printf("004b0160_TEST FAIL\n"); return 1; }
    std::printf("004b0160_TEST PASS\n");
    return 0;
}