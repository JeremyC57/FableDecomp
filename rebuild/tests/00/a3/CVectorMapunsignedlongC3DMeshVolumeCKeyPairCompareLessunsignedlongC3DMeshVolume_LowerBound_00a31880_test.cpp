// Test for 0x00A31880 CVectorMap<unsigned long, C3DMeshVolume, CKeyPairCompareLess>::LowerBound: binary search over the sorted
// key/value pairs (count halved each step); checked against a linear scan for every probe key.
#include <stdio.h>
struct Entry { unsigned int first; int second; };
struct CKeyPairCompareLess {
    bool operator()(const Entry& a, unsigned int k) const { return a.first < k; }
    bool operator()(unsigned int k, const Entry& a) const { return k < a.first; }
};
struct CVectorMap {
    const Entry* b; const Entry* e; CKeyPairCompareLess Compare;
    const Entry* LowerBound(const unsigned int& key) const
    {
        const Entry* first = b; int count = (int)(e - first);
        while (count > 0) {
            int step = count / 2; const Entry* it = first + step;
            if (Compare(*it, key)) { first = it + 1; count -= step + 1; } else count = step;
        }
        return first;
    }
};
int main()
{
    Entry d[6] = { { 2, 0 }, { 4, 1 }, { 4, 2 }, { 4, 3 }, { 9, 4 }, { 12, 5 } };
    CVectorMap m; m.b = d; m.e = d + 6;
    bool ok = true; bool upper = false;
    for (unsigned int k = 0; k < 15; ++k) {
        const Entry* want = d; while (want != d + 6 && (upper ? !(k < want->first) : want->first < k)) ++want;
        if (m.LowerBound(k) != want) ok = false;
    }
    m.e = d; if (m.LowerBound(4) != d) ok = false;
    puts(ok ? "00a31880_TEST PASS" : "00a31880_TEST FAIL");
    return ok ? 0 : 1;
}