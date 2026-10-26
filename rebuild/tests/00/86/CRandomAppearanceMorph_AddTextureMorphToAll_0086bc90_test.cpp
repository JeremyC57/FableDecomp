// Behaviour test for 0x0086BC90: AddTextureMorphToAll adds {originalTexture -> CTextureMorph(newTexture, 0)} to the general texture morphs.
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

struct CTextureMorph { unsigned int NewTexture; unsigned int GroupID; CTextureMorph(unsigned int n, unsigned int g) : NewTexture(n), GroupID(g) { } };
class CRandomAppearanceMorph
{ public:
    CVectorMap<unsigned int, CTextureMorph> GeneralTextureMorphs;
    void AddTextureToTextureMorph(CVectorMap<unsigned int, CTextureMorph>& map, int key, const CTextureMorph& morph)
    {
        unsigned int k = (unsigned int)key;
        map.Insert(k, morph);
        map.Update();
    }
    void AddTextureMorphToAll(unsigned int originalTexture, unsigned int newTexture)
    {
        AddTextureToTextureMorph(GeneralTextureMorphs, (int)originalTexture, CTextureMorph(newTexture, 0));
    }
};
int main()
{
    int fails = 0;

    CRandomAppearanceMorph m;
    m.AddTextureMorphToAll(30, 31);
    m.AddTextureMorphToAll(10, 11);
    if (m.GeneralTextureMorphs.Dirty || m.GeneralTextureMorphs.Vector.size() != 2u) ++fails;
    if (m.GeneralTextureMorphs.Vector[0].first != 10u || m.GeneralTextureMorphs.Vector[0].second.NewTexture != 11u || m.GeneralTextureMorphs.Vector[0].second.GroupID != 0u) ++fails;
    if (m.GeneralTextureMorphs.Vector[1].first != 30u || m.GeneralTextureMorphs.Vector[1].second.NewTexture != 31u) ++fails;
    if (fails) { std::printf("0086bc90_TEST FAIL\n"); return 1; }
    std::printf("0086bc90_TEST PASS\n");
    return 0;
}