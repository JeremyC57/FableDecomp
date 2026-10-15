#include <cstdio>
#include <cstring>
struct CCharStringData { const char* m_pData; unsigned long m_length; };
static int g_compares = 0;
static long Compare(const char* a, const char* b) { ++g_compares; return std::strcmp(a, b); }
struct CCharString {
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = PStringData; CCharStringData* b = other.PStringData;
        if (a == b) return true;
        if (a && b && a->m_length == b->m_length) return Compare(a->m_pData, b->m_pData) == 0;
        return false;
    }
};
static CCharStringData Block(const char* s) { CCharStringData d; d.m_pData = s; d.m_length = (unsigned long)std::strlen(s); return d; }
// Behaviour model of 0x004AF5B0: CQuestManager::GetActiveQuestFromScriptName over the active-quest list.
struct CActiveQuest { long ID; CCharString QuestName; };
struct SNode { SNode* next; SNode* prev; CActiveQuest* m_pData; void* m_pInfo; };
struct CQuestManager {
    SNode* ActiveQuests;
    CActiveQuest* GetActiveQuestFromScriptName(const CCharString& name) const
    {
        SNode* head = ActiveQuests; SNode* node = head->next;
        if (node != head) {
            CCharStringData* key = name.PStringData;
            do {
                CActiveQuest* quest = node->m_pData;
                CCharString k; k.PStringData = key;
                if (quest->QuestName == k) return quest;
                node = node->next;
            } while (node != head);
        }
        return 0;
    }
};
int main()
{
    CCharStringData a = Block("Q_Orchard"), b = Block("Q_Wasps"), c = Block("Q_Wasps"), z = Block("Q_Absent");
    CActiveQuest q1, q2; q1.ID = 1; q1.QuestName.PStringData = &a; q2.ID = 2; q2.QuestName.PStringData = &b;
    SNode head, n1, n2; head.next = &n1; n1.next = &n2; n2.next = &head; n1.m_pData = &q1; n2.m_pData = &q2;
    CQuestManager m; m.ActiveQuests = &head;
    CCharString name; name.PStringData = &c;
    if (m.GetActiveQuestFromScriptName(name) != &q2) { std::printf("004af5b0_TEST FAIL\n"); return 1; }
    name.PStringData = &a;                     // the same block: no Compare call needed
    int before = g_compares;
    if (m.GetActiveQuestFromScriptName(name) != &q1 || g_compares != before) { std::printf("004af5b0_TEST FAIL\n"); return 1; }
    name.PStringData = &z;
    if (m.GetActiveQuestFromScriptName(name) != 0) { std::printf("004af5b0_TEST FAIL\n"); return 1; }
    SNode empty; empty.next = &empty; m.ActiveQuests = &empty;
    if (m.GetActiveQuestFromScriptName(name) != 0) { std::printf("004af5b0_TEST FAIL\n"); return 1; }
    std::printf("004af5b0_TEST PASS\n");
    return 0;
}