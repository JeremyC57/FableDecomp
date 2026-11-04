// Behaviour test for CSoundMap::operator= (0x0045CFAD): a model of the row -- both maps' entries and Dirty flags are copied
// and the assignment returns the target.
#include <stdio.h>
#include <vector>
struct CSoundPair { int FirstSound, LastSound; };
template <class V> struct CVectorMap {
    std::vector<std::pair<unsigned int, V> > Data; char Compare; bool Dirty;
    CVectorMap& operator=(const CVectorMap& o) { Data = o.Data; Compare = o.Compare; Dirty = o.Dirty; return *this; }
};
struct CSoundMap {
    CVectorMap<CSoundPair> SoundMap; CVectorMap<int> CriteriaMap;
    CSoundMap& operator=(const CSoundMap& o) { SoundMap = o.SoundMap; CriteriaMap = o.CriteriaMap; return *this; }
};
int main()
{
    CSoundMap a, b;
    CSoundPair p = { 3, 7 };
    a.SoundMap.Data.push_back(std::make_pair(5u, p)); a.SoundMap.Dirty = true;
    a.CriteriaMap.Data.push_back(std::make_pair(9u, 42)); a.CriteriaMap.Dirty = false;
    b.SoundMap.Dirty = false; b.CriteriaMap.Dirty = true;
    CSoundMap* r = &(b = a);
    int ok = r == &b && b.SoundMap.Data.size() == 1 && b.SoundMap.Data[0].second.LastSound == 7 && b.SoundMap.Dirty
          && b.CriteriaMap.Data.size() == 1 && b.CriteriaMap.Data[0].second == 42 && !b.CriteriaMap.Dirty;
    printf(ok ? "0045cfad_TEST PASS\n" : "0045cfad_TEST FAIL\n");
    return ok ? 0 : 1;
}