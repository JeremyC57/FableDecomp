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
static CTestQuest* copy_backward(CTestQuest* first, CTestQuest* last, CTestQuest* result)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
static void __fastcall __linear_insert(CTestQuest* first, CTestQuest* last, CTestQuest val, CTestQuestNameCompare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

int main()
{
    CTestQuest q[4];
    q[0] = Q("c", 0); q[1] = Q("e", 1); q[2] = Q("g", 2); q[3] = Q("a", 3);
    __linear_insert(q, q + 3, q[3], CTestQuestNameCompare());
    if (!Sorted(q, 4) || q[0].ColourIndex != 3 || q[3].ColourIndex != 2) { std::printf("00624760_TEST FAIL\n"); return 1; }
    q[3] = Q("d", 7);
    __linear_insert(q, q + 3, q[3], CTestQuestNameCompare());
    if (!Sorted(q, 4) || q[2].ColourIndex != 7) { std::printf("00624760_TEST FAIL\n"); return 1; }
    std::printf("00624760_TEST PASS\n");
    return 0;
}