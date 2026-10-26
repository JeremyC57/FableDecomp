// Behaviour test for 0x0086BB60: AddSkeletalMorph inserts {groupID, CSkeletalMorph(groupID, name, allowVariation)} and leaves the map sorted and clean.
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

#include <cstring>
class CDefString { public: int TablePos; CDefString(const char* name) : TablePos((int)std::strlen(name)) { } };
struct CSkeletalMorph
{
    int GroupID; int NameTablePos; bool AllowVariation;
    CSkeletalMorph(int g, const char* name, bool a) : GroupID(g), NameTablePos(CDefString(name).TablePos), AllowVariation(a) { }
};
class CRandomAppearanceMorph
{ public:
    CVectorMap<int, CSkeletalMorph> SkeletalMorphs;
    void AddSkeletalMorph(unsigned int groupID, const char* name, bool allowVariation)
    {
        int key = (int)groupID;
        SkeletalMorphs.Insert(key, CSkeletalMorph((int)groupID, name, allowVariation));
        SkeletalMorphs.Update();
    }
};
int main()
{
    int fails = 0;

    CRandomAppearanceMorph m;
    m.AddSkeletalMorph(7, "Fat", true);
    m.AddSkeletalMorph(3, "Thin", false);
    m.AddSkeletalMorph(7, "Tall", false);
    if (m.SkeletalMorphs.Dirty || m.SkeletalMorphs.Vector.size() != 3u) ++fails;
    if (m.SkeletalMorphs.Vector[0].first != 3 || m.SkeletalMorphs.Vector[1].first != 7 || m.SkeletalMorphs.Vector[2].first != 7) ++fails;
    if (m.SkeletalMorphs.Vector[0].second.GroupID != 3 || m.SkeletalMorphs.Vector[0].second.NameTablePos != 4 || m.SkeletalMorphs.Vector[0].second.AllowVariation) ++fails;
    if (m.SkeletalMorphs.Vector[1].second.NameTablePos != 3 || !m.SkeletalMorphs.Vector[1].second.AllowVariation) ++fails;   // equal keys keep insertion order here
    if (fails) { std::printf("0086bb60_TEST FAIL\n"); return 1; }
    std::printf("0086bb60_TEST PASS\n");
    return 0;
}