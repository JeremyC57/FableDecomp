// Behaviour test for 0x0086D740: CVectorMap::Insert appends (key, value) at the end and marks the map unsorted (no sort).
#include <cstdio>
#include <vector>
#include <algorithm>

struct SKeyLess { template <class P> bool operator()(const P& a, const P& b) const { return a.first < b.first; } };

template <class K, class V> struct SPair { K first; V second; SPair(const K& k, const V& v) : first(k), second(v) { } };

template <class K, class V> class CVectorMap
{ public:
    std::vector<SPair<K, V> > Vector;
    bool Dirty;
    CVectorMap() : Dirty(false) { }
    void Insert(const K& key, const V& value) { Vector.push_back(SPair<K, V>(key, value)); Dirty = true; }
    void Sort() { std::stable_sort(Vector.begin(), Vector.end(), SKeyLess()); Dirty = false; }
    void Update() { if (Dirty) Sort(); }
};

struct CSkeletalMorph { int GroupID; int NameTablePos; bool AllowVariation; };
int main()
{
    int fails = 0;

    CVectorMap<int, CSkeletalMorph> map;
    CSkeletalMorph a = { 5, 50, true }, b = { 2, 20, false };
    int k5 = 5, k2 = 2;
    map.Insert(k5, a);
    if (!map.Dirty || map.Vector.size() != 1u) ++fails;
    map.Dirty = false;
    map.Insert(k2, b);
    if (!map.Dirty || map.Vector.size() != 2u || map.Vector[0].first != 5 || map.Vector[1].first != 2) ++fails;   // unsorted
    if (map.Vector[1].second.NameTablePos != 20 || map.Vector[1].second.AllowVariation) ++fails;
    if (fails) { std::printf("0086d740_TEST FAIL\n"); return 1; }
    std::printf("0086d740_TEST PASS\n");
    return 0;
}