// Behaviour test for 0x0074CEE0 std::__find (random access, 4-way unrolled + switch tail) over
// CIntelligentPointer<const CThing>: for every range length up to 11 and every match position it returns the first
// entry whose item equals val's item (compared through GetPItem), and last when there is none
#include <cstdio>

namespace model {

struct CThing { int Id; };
int g_getpitem = 0;
struct CIntelligentPointer {
    const CThing* Item;
    const CThing* GetPItem() const { ++g_getpitem; return Item; }
    bool operator==(const CIntelligentPointer& o) const { return GetPItem() == o.GetPItem(); }
};
typedef CIntelligentPointer P;

P* find(P* first, P* last, const P& val)
{
    int trip_count = (int)(last - first) >> 2;
    for (; trip_count > 0; --trip_count) {
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == val) return first;
        ++first;
    case 2:
        if (*first == val) return first;
        ++first;
    case 1:
        if (*first == val) return first;
        ++first;
    case 0:
    default:
        return last;
    }
}

}

int main()
{
    using namespace model;
    CThing things[12];
    CThing other;
    other.Id = -1;
    int bad = 0;
    for (int n = 0; n <= 11; ++n) {
        P v[12];
        for (int i = 0; i < n; ++i) { things[i].Id = i; v[i].Item = &things[i]; }
        for (int m = 0; m < n; ++m) {
            P val;
            val.Item = &things[m];
            v[n - 1].Item = n - 1 > m ? &things[m] : v[n - 1].Item;   // a later duplicate must not win
            g_getpitem = 0;
            if (find(v, v + n, val) != v + m) bad = 1;
            if (g_getpitem != 2 * (m + 1)) bad = 2;
            v[n - 1].Item = &things[n - 1];
        }
        P none;
        none.Item = &other;
        if (find(v, v + n, none) != v + n) bad = 3;
    }
    if (bad) { std::printf("0074cee0_TEST FAIL\n"); return 1; }
    std::printf("0074cee0_TEST PASS\n");
    return 0;
}