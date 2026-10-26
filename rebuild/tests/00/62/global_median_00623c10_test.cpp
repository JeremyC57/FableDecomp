#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
// Test model: CTestQuest with CCharString members reduced to C strings; the comparator orders by QuestName
// (CTestQuestNameCompare 0x006238A0 compares the quest names).
class CCharString { public: const char* Text; };
class CTestQuest {
public:
    CCharString QuestName;
    CCharString HSPName;
    long        ColourIndex;
    CCharString DisplayName;
    CCharString IniFile;
    CCharString OutiFile;
    CCharString QuestCardDefName;
};
class CTestQuestNameCompare { public:
    bool operator()(const CTestQuest& a, const CTestQuest& b) const { return std::strcmp(a.QuestName.Text, b.QuestName.Text) < 0; }
};
static CTestQuest Q(const char* name, long colour)
{
    CTestQuest q;
    q.QuestName.Text = name; q.HSPName.Text = name; q.ColourIndex = colour; q.DisplayName.Text = name;
    q.IniFile.Text = name; q.OutiFile.Text = name; q.QuestCardDefName.Text = name;
    return q;
}
static bool Sorted(const CTestQuest* q, int n)
{
    for (int i = 1; i < n; ++i)
        if (std::strcmp(q[i - 1].QuestName.Text, q[i].QuestName.Text) > 0) return false;
    for (int j = 0; j < n; ++j)
        if (q[j].HSPName.Text != q[j].QuestName.Text || q[j].QuestCardDefName.Text != q[j].QuestName.Text) return false;
    return true;
}

static const CTestQuest& __fastcall __median(const CTestQuest& a, const CTestQuest& b, const CTestQuest& c, CTestQuestNameCompare comp)
{
    if (comp(a, b)) {
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    }
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

int main()
{
    CTestQuest a = Q("a", 1), b = Q("b", 2), c = Q("c", 3);
    CTestQuestNameCompare cmp;
    const CTestQuest* perms[6][3] = { {&a,&b,&c}, {&a,&c,&b}, {&b,&a,&c}, {&b,&c,&a}, {&c,&a,&b}, {&c,&b,&a} };
    for (int i = 0; i < 6; ++i)
        if (&__median(*perms[i][0], *perms[i][1], *perms[i][2], cmp) != &b) { std::printf("00623c10_TEST FAIL\n"); return 1; }
    if (&__median(a, a, c, cmp) != &a && __median(a, a, c, cmp).ColourIndex != 1) { std::printf("00623c10_TEST FAIL\n"); return 1; }
    std::printf("00623c10_TEST PASS\n");
    return 0;
}