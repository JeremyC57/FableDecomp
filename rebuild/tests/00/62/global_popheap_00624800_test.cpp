// Behaviour test for the CTestQuest __pop_heap (0x00624800): popping a max-heap (by QuestName) moves the
// largest element to `result` and re-heaps the rest with the passed value; repeated pops sort the range.
#include <cstdio>
#include <cstring>

struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) {}
    CCharString(const CCharString& o) : m_pData(o.m_pData) {}
    ~CCharString() {}
    CCharString& operator=(const CCharString& o) { m_pData = o.m_pData; return *this; }
};

class CTestQuest {
public:
    CCharString QuestName, HSPName;
    long ColourIndex;
    CCharString DisplayName, IniFile, OutiFile, QuestCardDefName;
    CTestQuest(const char* n, long c) : QuestName(n), HSPName(""), ColourIndex(c), DisplayName(""), IniFile(""),
        OutiFile(""), QuestCardDefName("") {}
};

struct CTestQuestNameCompare {
    bool operator()(const CTestQuest& a, const CTestQuest& b) const { return std::strcmp(a.QuestName.m_pData, b.QuestName.m_pData) < 0; }
};

// SGI __adjust_heap (the callee @ 0x006243D0).
void __adjust_heap(CTestQuest* first, long holeIndex, long len, CTestQuest value, CTestQuestNameCompare comp)
{
    long topIndex = holeIndex;
    long secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1])) secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        first[holeIndex] = first[secondChild - 1];
        holeIndex = secondChild - 1;
    }
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], value)) {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = value;
}

void __pop_heap(CTestQuest* first, CTestQuest* last, CTestQuest* result, CTestQuest value, CTestQuestNameCompare comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

int main()
{
    // a valid max-heap by name: e > d, c; d > a, b
    CTestQuest q[5] = { CTestQuest("e", 5), CTestQuest("d", 4), CTestQuest("c", 3), CTestQuest("a", 1), CTestQuest("b", 2) };
    long n = 5;
    const char* expect = "edcba";
    bool ok = true;
    for (int i = 0; i < 5; ++i) {
        CTestQuest* last = q + n - 1;
        __pop_heap(q, last, last, CTestQuest(*last), CTestQuestNameCompare(), (long*)0);
        if (q[n - 1].QuestName.m_pData[0] != expect[i] || q[n - 1].ColourIndex != 5 - i) ok = false;
        --n;
    }
    if (!ok) { std::printf("00624800_TEST FAIL\n"); return 1; }
    std::printf("00624800_TEST PASS\n");
    return 0;
}