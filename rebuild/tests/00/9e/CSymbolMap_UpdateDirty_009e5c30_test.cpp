// Behaviour test for CSymbolMap::UpdateDirty (0x009E5C30): a dirty map is sorted (by name) and marked clean; the unused capacity is
// trimmed whether or not the map was dirty.
#include <cstdio>
#include <cstring>
struct SEntry { const char* Name; long Val; };
static int g_trims = 0;
class CSymbolArray {
public:
    CSymbolArray() : Count(0) { }
    void DeallocateUnusedMemory() { ++g_trims; }
    SEntry Items[8];
    int Count;
};
class CSymbolVectorMap {
public:
    CSymbolVectorMap() : Dirty(false) { }
    void Sort()
    {
        if (Dirty) {
            for (int i = 1; i < Data.Count; ++i)
                for (int j = i; j > 0 && std::strcmp(Data.Items[j].Name, Data.Items[j - 1].Name) < 0; --j) {
                    SEntry t = Data.Items[j]; Data.Items[j] = Data.Items[j - 1]; Data.Items[j - 1] = t;
                }
            Dirty = false;
        }
    }
    CSymbolArray Data;
    bool Dirty;
};
class CSymbolMap {
public:
    void AddSymbolDirty(const char* name, long val) { SEntry e = { name, val }; SLONGMap.Data.Items[SLONGMap.Data.Count++] = e; SLONGMap.Dirty = true; }
    void UpdateDirty() const
    {
        SLONGMap.Sort();
        SLONGMap.Data.DeallocateUnusedMemory();
    }
    mutable CSymbolVectorMap SLONGMap;
};
int main()
{
    CSymbolMap m;
    m.UpdateDirty();
    int ok = g_trims == 1 && !m.SLONGMap.Dirty;
    m.AddSymbolDirty("ZOMBIE", 3); m.AddSymbolDirty("APPLE", 1); m.AddSymbolDirty("MIDDLE", 2);
    m.UpdateDirty();
    ok = ok && g_trims == 2 && !m.SLONGMap.Dirty && m.SLONGMap.Data.Items[0].Val == 1 && m.SLONGMap.Data.Items[1].Val == 2
        && m.SLONGMap.Data.Items[2].Val == 3;
    if (!ok) { std::printf("009e5c30_TEST FAIL\n"); return 1; }
    std::printf("009e5c30_TEST PASS\n");
    return 0;
}