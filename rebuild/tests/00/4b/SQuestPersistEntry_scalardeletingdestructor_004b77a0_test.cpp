#include <cstdio>
static int g_order[8]; static int g_n = 0;
struct CCharString {
    int m_tag;
    ~CCharString() { if (g_n < 8) g_order[g_n++] = m_tag; }
};
struct CWideString {
    int m_tag;
    ~CWideString() { if (g_n < 8) g_order[g_n++] = m_tag; }
};
struct CQuestPersistInfo { CCharString m_questName; CCharString m_persistScript; };
struct SQuestPersistEntry { CCharString first; CQuestPersistInfo second; };
void DestroyQuestPersistEntry(SQuestPersistEntry* entry) { delete entry; }
int main()
{
    int bad = 0;
    SQuestPersistEntry* e = new SQuestPersistEntry;
    e->first.m_tag = 1; e->second.m_questName.m_tag = 2; e->second.m_persistScript.m_tag = 3;
    DestroyQuestPersistEntry(e);
    if (g_n != 3 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1) bad = 1;
    if (bad) { std::printf("004b77a0_TEST FAIL\n"); return 1; }
    std::printf("004b77a0_TEST PASS\n");
    return 0;
}