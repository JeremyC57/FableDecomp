// Behaviour test for 0x004DFC42 _STL::__median of pair<CCharString, CCharString> by the first string (CKeyPairCompareLess; same body as 0x00A70020 in a frame-pointer, size-optimised TU).
#include <cstdio>
#include <cstring>

template <class C> class CBasicString {
public:
    const C* s;
    bool operator<(const CBasicString& other) const { return std::strcmp(s, other.s) < 0; }   // stand-in for 0x00429950
};
class CCharString {
public:
    CBasicString<char>* PData;
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData) return false;
        if (PData == 0) return true;
        if (other.PData == 0) return false;
        return *PData < *other.PData;
    }
};
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef pair<CCharString, CCharString> P;
typedef CKeyPairCompareLess<CCharString, CCharString> L;

static const P& __median(const P& a, const P& b, const P& c, L comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

int main()
{
    CBasicString<char> s[3] = { { "ant" }, { "bee" }, { "cat" } };
    P e[3];
    for (int k = 0; k < 3; ++k) { e[k].first.PData = &s[k]; e[k].second.PData = &s[2 - k]; }
    L comp;
    // every permutation of (ant, bee, cat) picks the element holding "bee"
    int perm[6][3] = { { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 }, { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 } };
    for (int j = 0; j < 6; ++j) {
        const P& m = __median(e[perm[j][0]], e[perm[j][1]], e[perm[j][2]], comp);
        if (&m != &e[1]) { std::printf("004dfc42_TEST FAIL\n"); return 1; }
    }
    // a null first string is the smallest: median of (null, cat, ant) is ant
    P n; n.first.PData = 0; n.second.PData = 0;
    if (&__median(n, e[2], e[0], comp) != &e[0]) { std::printf("004dfc42_TEST FAIL\n"); return 1; }
    // all equal (shared data): the SGI tree returns b
    if (&__median(e[1], e[1], e[1], comp) != &e[1]) { std::printf("004dfc42_TEST FAIL\n"); return 1; }
    P b2 = e[1];
    if (&__median(e[1], b2, e[1], comp) != &b2) { std::printf("004dfc42_TEST FAIL\n"); return 1; }
    std::printf("004dfc42_TEST PASS\n");
    return 0;
}