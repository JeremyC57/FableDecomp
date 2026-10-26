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

static void __fastcall __push_heap(CTestQuest* first, int holeIndex, int topIndex, CTestQuest val, CTestQuestNameCompare comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
static void __fastcall __adjust_heap(CTestQuest* first, int holeIndex, int len, CTestQuest val, CTestQuestNameCompare comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
}
static bool IsHeap(const CTestQuest* q, int n)
{
    CTestQuestNameCompare c;
    for (int i = 1; i < n; ++i)
        if (c(q[(i - 1) / 2], q[i])) return false;
    return true;
}

int main()
{
    CTestQuest q[10];
    const char* names[10] = { "a","b","c","d","e","f","g","h","i","j" };
    CTestQuestNameCompare cmp;
    for (int k = 0; k < 10; ++k) {
        q[k] = Q(names[k], k);
        __push_heap(q, k, 0, q[k], cmp);
        if (!IsHeap(q, k + 1)) { std::printf("00623f80_TEST FAIL\n"); return 1; }
    }
    if (std::strcmp(q[0].QuestName.Text, "j") != 0 || q[0].ColourIndex != 9) { std::printf("00623f80_TEST FAIL\n"); return 1; }
    std::printf("00623f80_TEST PASS\n");
    return 0;
}