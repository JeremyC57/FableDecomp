// Behaviour test for CWoundMorphs::GetPWoundWithIndex (0x00757930): re-sorts a dirty map, then returns the entry at the index or 0 when out of range.
#include <cstdio>
struct CEntry { int Location; int WoundTextureIndex; };
struct Pair { int first; CEntry second; };
class CWoundMorphs { public:
    Pair* First; Pair* Last; Pair* Eos; bool Compare; bool Dirty; int Sorts;
    unsigned int size() const { return (unsigned int)(Last - First); }
    void sort() { ++Sorts; for (Pair* i = First + 1; i < Last; ++i) { Pair v = *i; Pair* j = i; while (j != First && v.first < j[-1].first) { *j = j[-1]; --j; } *j = v; } }
    const CEntry* GetPWoundWithIndex(int index)
    {
        if (Dirty) { sort(); Dirty = false; }
        if (index >= 0 && (unsigned int)index < size()) return &First[index].second;
        return 0;
    }
};
int main()
{
    Pair p[3]; int keys[3] = { 3, 1, 2 };
    for (int i = 0; i < 3; ++i) { p[i].first = keys[i]; p[i].second.Location = keys[i]; p[i].second.WoundTextureIndex = keys[i] * 10; }
    CWoundMorphs w; w.First = p; w.Last = p + 3; w.Dirty = true; w.Sorts = 0;
    const CEntry* e = w.GetPWoundWithIndex(0);
    if (!e || e->Location != 1 || w.Dirty || w.Sorts != 1) { std::printf("00757930_TEST FAIL\n"); return 1; }
    if (w.GetPWoundWithIndex(2)->WoundTextureIndex != 30 || w.Sorts != 1) { std::printf("00757930_TEST FAIL\n"); return 1; }
    if (w.GetPWoundWithIndex(3) != 0 || w.GetPWoundWithIndex(-1) != 0) { std::printf("00757930_TEST FAIL\n"); return 1; }
    std::printf("00757930_TEST PASS\n");
    return 0;
}