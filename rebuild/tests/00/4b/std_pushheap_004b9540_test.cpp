// behaviour test for 0x004b9540 _Push_heap<pair<CCharString,CPersistInfo>> (CKeyPairCompareLess)
#include <cstdio>
#include <cstring>
// CCharString stand-in: a pointer to static text (null = empty string); `<` follows the retail
// inline operator< (same payload -> false, null this -> true, null other -> false, else strcmp).
class CCharString { public:
    const char* PStringData;
    CCharString() : PStringData(0) { }
    explicit CCharString(const char* s) : PStringData(s) { }
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData) return false;
        if (!PStringData) return true;
        if (!other.PStringData) return false;
        return std::strcmp(PStringData, other.PStringData) < 0;
    }
    bool FastCompare(const CCharString& other) const { return *this < other; }
    bool operator==(const CCharString& other) const { return !(*this < other) && !(other < *this); }
};
struct CPersistInfo { CCharString QuestName; CCharString PersistScript; };
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, CPersistInfo> TEntry;
struct TCompare {
    char m_tag;
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};
static void _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static const char* NAMES[7] = { "lamp", "anvil", "zebra", "hook", "moss", "crate", 0 };
int main()
{
    TEntry heap[7];
    TCompare c; c.m_tag = 0;
    for (int i = 0; i < 7; ++i) {
        TEntry v;
        v.first = CCharString(NAMES[i]);
        v.second.QuestName = CCharString(NAMES[i]); v.second.PersistScript = CCharString(NAMES[6 - i]);
        _Push_heap(heap, i, 0, v, c);
        for (int k = 1; k <= i; ++k)
            if (c(heap[(k - 1) / 2], heap[k])) { std::printf("004b9540_TEST FAIL\n"); return 1; }
    }
    // the greatest key ("zebra", pushed as i = 2) is on top with its payload
    const int K = 2;
    const TEntry& h = heap[0];
    if (!(h.first == CCharString("zebra")) || !(h.second.QuestName == CCharString(NAMES[K]) && h.second.PersistScript == CCharString(NAMES[6 - K]))) { std::printf("004b9540_TEST FAIL\n"); return 1; }
    // the null (empty) key sorts lowest: it never climbs above a named parent
    bool nullFound = false;
    for (int k = 0; k < 7; ++k) if (!heap[k].first.PStringData) nullFound = k >= 3;
    if (!nullFound) { std::printf("004b9540_TEST FAIL\n"); return 1; }
    std::printf("004b9540_TEST PASS\n");
    return 0;
}