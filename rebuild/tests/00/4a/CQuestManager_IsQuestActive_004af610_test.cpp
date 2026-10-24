#include <cstdio>

// Behaviour model of CQuestManager::IsQuestActive (0x004AF610): walk the active-quest list; CCharString equality is
// same data block -> equal, otherwise both non-null with equal length and an equal signed compare.
struct CCharStringData { const char* m_pData; unsigned long m_length; };
static long Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0) return 0;
        char cb = *b;
        if (ca < cb) return -1;
        if (ca > cb) return 1;
        ++a; ++b;
    }
}
struct CCharString {
    CCharStringData* PStringData;
    bool operator==(const CCharString& o) const
    {
        if (PStringData == o.PStringData) return true;
        if (PStringData && o.PStringData && PStringData->m_length == o.PStringData->m_length)
            return Compare(PStringData->m_pData, o.PStringData->m_pData) ? false : true;
        return false;
    }
};
struct CActiveQuest { long ID; CCharString QuestName; };
struct Node { Node* next; Node* prev; CActiveQuest* data; };
struct CQuestManager {
    Node* ActiveQuests;
    bool IsQuestActive(const CCharString& name) const
    {
        Node* end = ActiveQuests;
        for (Node* it = ActiveQuests->next; it != end; it = it->next)
            if (it->data->QuestName == name) return true;
        return false;
    }
};

int main()
{
    CCharStringData arena = { "Q_Arena", 7 }, arena2 = { "Q_Arena", 7 }, arenb = { "Q_Arenb", 7 }, longer = { "Q_ArenaX", 8 }, guild = { "Q_Guild", 7 };
    CActiveQuest q1 = { 1, { &guild } }, q2 = { 2, { &arena } }, q3 = { 3, { 0 } };
    Node head, n1, n2, n3;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &head;
    n1.data = &q1; n2.data = &q2; n3.data = &q3;
    CQuestManager qm = { &head };
    CCharString sameBlock = { &arena }, sameText = { &arena2 }, other = { &arenb }, len8 = { &longer }, empty = { 0 };
    if (!qm.IsQuestActive(sameBlock) || !qm.IsQuestActive(sameText)) { std::printf("004af610_TEST FAIL\n"); return 1; }
    if (qm.IsQuestActive(other) || qm.IsQuestActive(len8)) { std::printf("004af610_TEST FAIL\n"); return 1; }
    if (!qm.IsQuestActive(empty)) { std::printf("004af610_TEST FAIL\n"); return 1; }          // q3's null name equals a null name
    head.next = &head; CQuestManager none = { &head };
    if (none.IsQuestActive(sameBlock)) { std::printf("004af610_TEST FAIL\n"); return 1; }
    std::printf("004af610_TEST PASS\n");
    return 0;
}