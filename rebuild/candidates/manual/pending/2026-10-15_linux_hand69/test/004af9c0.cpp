// Behaviour test for CQuestManager::AreAllBoastsProcessed(CCharString&) const @ 0x004AF9C0
#include <cstdio>
#include <string.h>

struct CCharString {
    const char* m_text;
    bool operator!=(const CCharString& other) const { return strcmp(m_text, other.m_text) != 0; }
};

struct CBoast { long ID; CCharString ScriptName; bool Evil, Failed, Completed; };
struct CBoastNode { CBoastNode* next; CBoastNode* prev; CBoast value; };

struct CQuestManager {
    CBoastNode* AcceptedBoasts;
    bool AreAllBoastsProcessed(CCharString& scriptName) const
    {
        for (CBoastNode* node = AcceptedBoasts->next; node != AcceptedBoasts; node = node->next) {
            if (node->value.ScriptName != scriptName || node->value.Failed || node->value.Completed)
                continue;
            return false;
        }
        return true;
    }
};

int main()
{
    CBoastNode head, a, b, c;
    head.next = &a; a.next = &b; b.next = &c; c.next = &head;
    a.value.ScriptName.m_text = "Q_ONE"; a.value.Failed = true;  a.value.Completed = false;
    b.value.ScriptName.m_text = "Q_ONE"; b.value.Failed = false; b.value.Completed = true;
    c.value.ScriptName.m_text = "Q_TWO"; c.value.Failed = false; c.value.Completed = false;
    CQuestManager q; q.AcceptedBoasts = &head;
    CCharString one = { "Q_ONE" }, two = { "Q_TWO" }, three = { "Q_THREE" };
    if (!q.AreAllBoastsProcessed(one) || q.AreAllBoastsProcessed(two) || !q.AreAllBoastsProcessed(three)) { std::printf("004af9c0_TEST FAIL\n"); return 1; }
    c.value.Completed = true;
    if (!q.AreAllBoastsProcessed(two)) { std::printf("004af9c0_TEST FAIL\n"); return 1; }
    std::printf("004af9c0_TEST PASS\n");
    return 0;
}
