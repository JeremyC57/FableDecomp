// Behaviour test for CQuestManager::GetAvailableQuestCardDefNameFromScriptName(const CCharString&) const @ 0x004AF670
#include <cstdio>
#include <string.h>

struct CCharStringData { const char* m_pData; unsigned long m_length; };
static long Compare_00411570(const char* a, const char* b) { return strcmp(a, b); }
struct CCharString { CCharStringData* PStringData; };
static inline bool SameText(const CCharStringData* a, const CCharStringData* b)
{
    if (a == b)
        return true;
    if (a && b && a->m_length == b->m_length)
        return Compare_00411570(a->m_pData, b->m_pData) == 0;
    return false;
}

static CCharString g_EmptyCharString_013bd804 = { 0 };

struct CQuestCard { CCharString DefName; CCharString ScriptName; bool Replayable; long FeatIndex; };
struct CQuestCardNode { CQuestCardNode* next; CQuestCardNode* prev; CQuestCard value; };

struct CQuestManager {
    CQuestCardNode* AvailableQuestCardTypes;
    const CCharString& GetAvailableQuestCardDefNameFromScriptName(const CCharString& scriptName) const
    {
        CQuestCardNode* head = AvailableQuestCardTypes;
        CQuestCardNode* node = head->next;
        if (node != head) {
            const CCharStringData* key = scriptName.PStringData;
            do {
                if (SameText(node->value.ScriptName.PStringData, key))
                    return node->value.DefName;
                node = node->next;
            } while (node != head);
        }
        return g_EmptyCharString_013bd804;
    }
};

int main()
{
    CCharStringData dA = { "CARD_A", 6 }, sA = { "Q_A", 3 }, dB = { "CARD_B", 6 }, sB = { "Q_B", 3 }, key = { "Q_B", 3 }, other = { "Q_C", 3 };
    CQuestCardNode head, n1, n2;
    head.next = &n1; n1.next = &n2; n2.next = &head;
    n1.value.DefName.PStringData = &dA; n1.value.ScriptName.PStringData = &sA;
    n2.value.DefName.PStringData = &dB; n2.value.ScriptName.PStringData = &sB;
    CQuestManager q; q.AvailableQuestCardTypes = &head;
    CCharString k = { &key }, o = { &other }, same = { &sA };
    if (&q.GetAvailableQuestCardDefNameFromScriptName(k) != &n2.value.DefName) { std::printf("004af670_TEST FAIL\n"); return 1; }
    if (&q.GetAvailableQuestCardDefNameFromScriptName(same) != &n1.value.DefName) { std::printf("004af670_TEST FAIL\n"); return 1; }
    if (&q.GetAvailableQuestCardDefNameFromScriptName(o) != &g_EmptyCharString_013bd804) { std::printf("004af670_TEST FAIL\n"); return 1; }
    CQuestCardNode empty; empty.next = &empty; q.AvailableQuestCardTypes = &empty;
    if (&q.GetAvailableQuestCardDefNameFromScriptName(k) != &g_EmptyCharString_013bd804) { std::printf("004af670_TEST FAIL\n"); return 1; }
    std::printf("004af670_TEST PASS\n");
    return 0;
}