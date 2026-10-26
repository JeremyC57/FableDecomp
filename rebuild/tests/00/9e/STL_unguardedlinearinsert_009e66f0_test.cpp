#include <cstdio>

// Host model of std::__unguarded_linear_insert over pair<CCharString, CSymbolMap::CSymbolInfo> ordered by
// CCompareString (a.first.FastCompare(b.first)); the string is modelled by an int key, FastCompare as key order.
class CCharString { public:
    int Key;
    bool FastCompare(const CCharString& other) const { return Key < other.Key; }
};
struct CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; };
struct TSymbolEntry { CCharString first; CSymbolInfo second; };

struct CCompareString {
    bool operator()(const TSymbolEntry& a, const TSymbolEntry& b) const { return a.first.FastCompare(b.first); }
};

static void unguarded_linear_insert(TSymbolEntry* last, TSymbolEntry val, CCompareString comp)
{
    TSymbolEntry* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

static TSymbolEntry make(int key, long val)
{
    TSymbolEntry e;
    e.first.Key = key;
    e.second.GroupName.Key = key * 2;
    e.second.EntryName.Key = key * 3;
    e.second.Val = val;
    return e;
}

int main()
{
    TSymbolEntry a[6];
    a[0] = make(1, 10); a[1] = make(3, 30); a[2] = make(5, 50); a[3] = make(7, 70); a[4] = make(9, 90);
    a[5] = make(4, 40);                                     // the element to sink (a[0] is the guard)
    unguarded_linear_insert(a + 5, a[5], CCompareString());
    const int want[6] = { 1, 3, 4, 5, 7, 9 };
    for (int i = 0; i < 6; ++i)
        if (a[i].first.Key != want[i] || a[i].second.Val != want[i] * 10 || a[i].second.EntryName.Key != want[i] * 3) {
            std::printf("009e66f0_TEST FAIL\n");
            return 1;
        }
    TSymbolEntry b[3];
    b[0] = make(2, 20); b[1] = make(6, 60); b[2] = make(8, 80);  // already in place: stays at the end
    unguarded_linear_insert(b + 2, b[2], CCompareString());
    if (b[2].first.Key != 8 || b[1].first.Key != 6) { std::printf("009e66f0_TEST FAIL\n"); return 1; }
    TSymbolEntry c[3];
    c[0] = make(2, 20); c[1] = make(6, 60); c[2] = make(6, 61);  // equal keys: no move (strict order)
    unguarded_linear_insert(c + 2, c[2], CCompareString());
    if (c[1].second.Val != 60 || c[2].second.Val != 61) { std::printf("009e66f0_TEST FAIL\n"); return 1; }
    std::printf("009e66f0_TEST PASS\n");
    return 0;
}