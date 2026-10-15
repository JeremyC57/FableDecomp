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
struct CBoast {
    long ID, Cost, Reward;
    CCharString ScriptName; CWideString Text; CWideString Title;
    bool Evil, Failed, Completed; long GroupIndex;
};
struct SBoastMapEntry { long first; CBoast second; };
void DestroyBoastMapEntry(SBoastMapEntry* entry) { delete entry; }
int main()
{
    int bad = 0;
    SBoastMapEntry* e = new SBoastMapEntry;
    e->second.ScriptName.m_tag = 1; e->second.Text.m_tag = 2; e->second.Title.m_tag = 3;
    DestroyBoastMapEntry(e);
    if (g_n != 3 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1) bad = 1;
    if (bad) { std::printf("0056cba0_TEST FAIL\n"); return 1; }
    std::printf("0056cba0_TEST PASS\n");
    return 0;
}
