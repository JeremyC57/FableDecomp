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

static void __fastcall __unguarded_linear_insert(CTestQuest* last, CTestQuest val, CTestQuestNameCompare comp)
{
    CTestQuest* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

int main()
{
    CTestQuest q[5];
    q[0] = Q("a", 0); q[1] = Q("c", 1); q[2] = Q("e", 2); q[3] = Q("g", 3); q[4] = Q("d", 4);
    __unguarded_linear_insert(q + 4, q[4], CTestQuestNameCompare());
    if (!Sorted(q, 5) || q[2].ColourIndex != 4 || q[4].ColourIndex != 3) { std::printf("00623e90_TEST FAIL\n"); return 1; }
    q[4] = Q("z", 9);
    __unguarded_linear_insert(q + 4, q[4], CTestQuestNameCompare());
    if (!Sorted(q, 5) || q[4].ColourIndex != 9) { std::printf("00623e90_TEST FAIL\n"); return 1; }
    std::printf("00623e90_TEST PASS\n");
    return 0;
}