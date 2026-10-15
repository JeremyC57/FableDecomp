// Behaviour test for CQuestManager::OnRegionLoaded(const CCharString&) @ 0x004AFC00
#include <cstdio>

struct CCharString { const char* m_text; };

struct CScriptBase {
    const CCharString* m_last; int m_calls;
    void OnRegionLoaded(const CCharString& region) { m_last = &region; ++m_calls; }
};

struct CActiveQuest { long ID; void* PScriptInfo; CScriptBase* PScript; };
struct CActiveQuestNode { CActiveQuestNode* next; CActiveQuestNode* prev; CActiveQuest* quest; };

struct CQuestManager {
    CActiveQuestNode* ActiveQuests;
    void OnRegionLoaded(const CCharString& region)
    {
        for (CActiveQuestNode* node = ActiveQuests->next; node != ActiveQuests; node = node->next) {
            CScriptBase* script = node->quest->PScript;
            if (script)
                script->OnRegionLoaded(region);
        }
    }
};

int main()
{
    CScriptBase s1, s3; s1.m_calls = 0; s3.m_calls = 0; s1.m_last = 0; s3.m_last = 0;
    CActiveQuest q1 = { 1, 0, &s1 }, q2 = { 2, 0, 0 }, q3 = { 3, 0, &s3 };
    CActiveQuestNode head, n1, n2, n3;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &head;
    n1.quest = &q1; n2.quest = &q2; n3.quest = &q3;
    CQuestManager m; m.ActiveQuests = &head;
    CCharString region = { "Oakvale" };
    m.OnRegionLoaded(region);
    if (s1.m_calls != 1 || s3.m_calls != 1 || s1.m_last != &region || s3.m_last != &region) { std::printf("004afc00_TEST FAIL\n"); return 1; }
    std::printf("004afc00_TEST PASS\n");
    return 0;
}