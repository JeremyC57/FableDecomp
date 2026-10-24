// Behaviour test for 0077ecb0: __insertion_sort over pair<CCharString, C3DVector> ordered by name -- with STLport's
// __linear_insert written out here, the range ends sorted by name with every vector still beside its name.
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString() : PStringData("") { ++g_live; }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct PairCharStringVector {
    CCharString first;
    C3DVector   second;
    PairCharStringVector() { }
    PairCharStringVector(const PairCharStringVector& other) : first(other.first), second(other.second) { }
};
struct CKeyPairCompareLess {
    CKeyPairCompareLess() { }
    bool operator()(const PairCharStringVector& a, const PairCharStringVector& b) const
    {
        return std::strcmp(a.first.PStringData, b.first.PStringData) < 0;
    }
};

// STLport __unguarded_linear_insert / __linear_insert (0x0077E960 in retail)
void __unguarded_linear_insert(PairCharStringVector* last, PairCharStringVector val, CKeyPairCompareLess comp)
{
    PairCharStringVector* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
void __linear_insert(PairCharStringVector* first, PairCharStringVector* last, PairCharStringVector val,
                     CKeyPairCompareLess comp)
{
    if (comp(val, *first)) {
        for (PairCharStringVector* p = last; p != first; --p)
            *p = *(p - 1);
        *first = val;
    } else
        __unguarded_linear_insert(last, val, comp);
}

void __insertion_sort(PairCharStringVector* first, PairCharStringVector* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (PairCharStringVector* i = first + 1; i != last; ++i)
        __linear_insert(first, i, PairCharStringVector(*i), comp);
}

int main()
{
    {
        static const char* const names[6] = { "orc", "balverine", "hobbe", "wasp", "bandit", "nymph" };
        static const char* const sorted[6] = { "balverine", "bandit", "hobbe", "nymph", "orc", "wasp" };
        PairCharStringVector v[6];
        for (int i = 0; i < 6; ++i) {
            v[i].first.PStringData = names[i];
            v[i].second.X = (float)std::strlen(names[i]);
            v[i].second.Z = (float)names[i][0];
        }
        CKeyPairCompareLess comp;
        __insertion_sort(v, v + 6, comp);
        for (int i = 0; i < 6; ++i) {
            if (std::strcmp(v[i].first.PStringData, sorted[i]) != 0 || v[i].second.X != (float)std::strlen(sorted[i]) ||
                v[i].second.Z != (float)sorted[i][0]) { std::printf("0077ecb0_TEST FAIL\n"); return 1; }
        }
        __insertion_sort(v, v, comp);                       // empty range is a no-op
    }
    if (g_live != 0) { std::printf("0077ecb0_TEST FAIL\n"); return 1; }
    std::printf("0077ecb0_TEST PASS\n");
    return 0;
}