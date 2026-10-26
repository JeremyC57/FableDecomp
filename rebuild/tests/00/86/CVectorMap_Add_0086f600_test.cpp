// Behaviour test for 0x0086F600: CVectorMap::Add inserts the value under its own first field as the key and sorts (Dirty cleared).
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

struct CLongMapValue8 { int Key; unsigned int Data; };
static void Add(CVectorMap<int, CLongMapValue8>& map, const CLongMapValue8& value) { map.Insert(value.Key, value); map.Sort(); }
int main()
{
    int fails = 0;

    CVectorMap<int, CLongMapValue8> map;
    CLongMapValue8 a = { 8, 80 }, b = { -3, 30 };
    Add(map, a);
    Add(map, b);
    if (map.Dirty || map.Vector.size() != 2u) ++fails;
    if (map.Vector[0].first != -3 || map.Vector[0].second.Key != -3 || map.Vector[0].second.Data != 30u || map.Vector[1].first != 8) ++fails;   // signed key
    if (fails) { std::printf("0086f600_TEST FAIL\n"); return 1; }
    std::printf("0086f600_TEST PASS\n");
    return 0;
}