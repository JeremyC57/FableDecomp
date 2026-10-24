#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
// Test model: CTestQuest with CCharString members reduced to C strings; the comparator orders by QuestName
// (CTestQuestNameCompare 0x006238A0 compares the quest names).
struct CCharString { const char* Text; };
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
struct CTestQuestNameCompare {
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
static void __fastcall __linear_insert(CTestQuest* first, CTestQuest* last, CTestQuest val, CTestQuestNameCompare comp)
{
    if (comp(val, *first)) {
        for (CTestQuest* p = last; p != first; --p)
            *p = *(p - 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
static void __fastcall __insertion_sort(CTestQuest* first, CTestQuest* last, CTestQuestNameCompare comp)
{
    if (first == last)
        return;
    for (CTestQuest* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    CTestQuest q[10];
    const char* names[10] = { "delta", "alpha", "golf", "charlie", "echo", "bravo", "foxtrot", "hotel", "india", "juliet" };
    for (int k = 0; k < 10; ++k) q[k] = Q(names[k], k);
    __insertion_sort(q, q + 10, CTestQuestNameCompare());
    if (!Sorted(q, 10) || q[0].ColourIndex != 1 || q[9].ColourIndex != 9) { std::printf("00624c90_TEST FAIL\n"); return 1; }
    __insertion_sort(q, q, CTestQuestNameCompare());
    
    std::printf("00624c90_TEST PASS\n");
    return 0;
}