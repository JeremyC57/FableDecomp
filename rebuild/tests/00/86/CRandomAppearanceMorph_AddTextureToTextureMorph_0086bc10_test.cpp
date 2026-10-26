// Behaviour test for 0x0086BC10: AddTextureToTextureMorph inserts the morph under the key and re-sorts the map (Dirty cleared).
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

struct CTextureMorph { unsigned int NewTexture; unsigned int GroupID; };
static void AddTextureToTextureMorph(CVectorMap<unsigned int, CTextureMorph>& map, int key, const CTextureMorph& morph)
{
    unsigned int k = (unsigned int)key;
    map.Insert(k, morph);
    map.Update();
}
int main()
{
    int fails = 0;

    CVectorMap<unsigned int, CTextureMorph> map;
    CTextureMorph t1 = { 100, 1 }, t2 = { 200, 2 };
    AddTextureToTextureMorph(map, 9, t1);
    AddTextureToTextureMorph(map, 4, t2);
    if (map.Dirty || map.Vector.size() != 2u) ++fails;
    if (map.Vector[0].first != 4u || map.Vector[0].second.NewTexture != 200u || map.Vector[1].first != 9u || map.Vector[1].second.GroupID != 1u) ++fails;
    if (fails) { std::printf("0086bc10_TEST FAIL\n"); return 1; }
    std::printf("0086bc10_TEST PASS\n");
    return 0;
}