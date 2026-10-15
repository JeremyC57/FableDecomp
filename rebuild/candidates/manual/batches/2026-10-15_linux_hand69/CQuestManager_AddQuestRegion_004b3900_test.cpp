// Behaviour test for CQuestManager::AddQuestRegion(const CCharString&, const CRegion*) @ 0x004B3900
#include <cstdio>
#include <string.h>

struct CRegion { int m_id; };
struct CCharStringData { const char* m_pData; unsigned long m_length; };
static long Compare_00411570(const char* a, const char* b) { return strcmp(a, b); }
struct CCharString { CCharStringData* PStringData; };
static inline bool SameText(const CCharStringData* a, const CCharStringData* b)
{
    if (a == b) return true;
    if (a && b && a->m_length == b->m_length) return Compare_00411570(a->m_pData, b->m_pData) == 0;
    return false;
}

struct CActiveQuest {
    CCharString QuestName;
    const CRegion* m_lastRegion; int m_regions;
    void AddPRegion(const CRegion* region) { m_lastRegion = region; ++m_regions; }
};
struct CActiveQuestNode { CActiveQuestNode* next; CActiveQuestNode* prev; CActiveQuest* quest; };

struct CQuestManager {
    CActiveQuestNode* ActiveQuests;
    void AddQuestRegion(const CCharString& questName, const CRegion* region)
    {
        CActiveQuestNode* head = ActiveQuests;
        CActiveQuestNode* node = head->next;
        if (node != head) {
            const CCharStringData* key = questName.PStringData;
            do {
                CActiveQuest* quest = node->quest;
                if (SameText(quest->QuestName.PStringData, key)) {
                    quest->AddPRegion(region);
                    return;
                }
                node = node->next;
            } while (node != head);
        }
    }
};

int main()
{
    CCharStringData s1 = { "Q_ONE", 5 }, s2 = { "Q_TWO", 5 }, key = { "Q_TWO", 5 }, miss = { "Q_SIX", 5 };
    CActiveQuest q1, q2; q1.QuestName.PStringData = &s1; q2.QuestName.PStringData = &s2;
    q1.m_regions = 0; q2.m_regions = 0; q1.m_lastRegion = 0; q2.m_lastRegion = 0;
    CActiveQuestNode head, n1, n2; head.next = &n1; n1.next = &n2; n2.next = &head; n1.quest = &q1; n2.quest = &q2;
    CQuestManager m; m.ActiveQuests = &head;
    CRegion r = { 7 };
    CCharString k = { &key }, x = { &miss };
    m.AddQuestRegion(k, &r);
    m.AddQuestRegion(x, &r);
    if (q2.m_regions != 1 || q2.m_lastRegion != &r || q1.m_regions != 0) { std::printf("004b3900_TEST FAIL\n"); return 1; }
    std::printf("004b3900_TEST PASS\n");
    return 0;
}
