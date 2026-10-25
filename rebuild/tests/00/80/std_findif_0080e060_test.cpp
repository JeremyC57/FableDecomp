// Behaviour test for 0x0080E060 std::__find_if over pair<CIntelligentPointer<CThing>,long> with CIsFirstElementEqualTo.
#include <cstdio>
struct CObj { int id; };
struct CPtr {
    CObj* PData;
    CObj* GetPItem() const { return PData; }
};
struct CEntry { CPtr first; long second; };
struct CIsFirstElementEqualTo {
    CPtr Value;
    bool operator()(const CEntry& e) const { return e.first.GetPItem() == Value.GetPItem(); }
};
static CEntry* find_if_model(CEntry* first, CEntry* last, CIsFirstElementEqualTo pred)
{
    int trip_count = (int)(last - first) >> 2;
    for (; trip_count > 0; --trip_count) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
int main()
{
    CObj objs[8];
    CEntry e[7];
    for (int i = 0; i < 7; ++i) { e[i].first.PData = &objs[i]; e[i].second = i; }
    for (int n = 0; n <= 7; ++n) {
        for (int k = 0; k < 8; ++k) {
            CIsFirstElementEqualTo p; p.Value.PData = &objs[k];
            CEntry* r = find_if_model(e, e + n, p);
            CEntry* want = (k < n) ? e + k : e + n;
            if (r != want) { std::printf("0080e060_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("0080e060_TEST PASS\n");
    return 0;
}