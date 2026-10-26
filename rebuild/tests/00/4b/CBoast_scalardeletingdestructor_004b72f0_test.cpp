#include <cstdio>
static int g_order[8]; static int g_n = 0;
class CCharString { public:
    int m_tag;
    ~CCharString() { if (g_n < 8) g_order[g_n++] = m_tag; }
};
class CWideString { public:
    int m_tag;
    ~CWideString() { if (g_n < 8) g_order[g_n++] = m_tag; }
};
struct CBoast {
    long ID, Cost, Reward;
    CCharString ScriptName; CWideString Text; CWideString Title;
    bool Evil, Failed, Completed; long GroupIndex;
};
void DestroyBoast(CBoast* boast) { delete boast; }
int main()
{
    int bad = 0;
    CBoast* b = new CBoast;
    b->ScriptName.m_tag = 1; b->Text.m_tag = 2; b->Title.m_tag = 3;
    DestroyBoast(b);
    if (g_n != 3 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1) bad = 1;
    if (bad) { std::printf("004b72f0_TEST FAIL\n"); return 1; }
    std::printf("004b72f0_TEST PASS\n");
    return 0;
}