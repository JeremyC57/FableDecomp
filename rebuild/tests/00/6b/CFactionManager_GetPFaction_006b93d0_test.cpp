// Behaviour test for 0x006B93D0 CFactionManager::GetPFaction(const CCharString&): CRC the name, binary-search the hash-keyed map, null when absent.
#include <cstdio>
#include <cstring>
struct CCharStringData { const char* Text; unsigned long Length; };
struct CCharString { CCharStringData* PStringData; };
// Stand-in CRC: a simple additive hash with the retail signature (seed, data, length); the real one is CRC::Calc 0x004014A0.
unsigned long __fastcall CRCCalc(unsigned long seed, const void* data, unsigned long length)
{
    unsigned long h = seed;
    const unsigned char* p = (const unsigned char*)data;
    for (unsigned long i = 0; i < length; ++i) h = h * 31 + p[i];
    return h;
}
class CFaction { public: int Id; };
template <class T> struct CCountedPointer { T* Data; T* Get() const { return Data; } };
template <class A, class B> struct pair { A first; B second; };
struct CFactionMap {
    typedef pair<unsigned long, CCountedPointer<CFaction> > value_type;
    value_type* First; value_type* Last; value_type* EndOfStorage;
    __declspec(noinline) value_type* LowerBound(const unsigned long& key);
    value_type* Find(const unsigned long& key)
    {
        value_type* it = LowerBound(key);
        if (it == Last || key < it->first) return Last;
        return it;
    }
};
class CFactionManager {
public:
    CFactionMap Factions;
    CFaction* GetPFaction(unsigned long hash)
    {
        CFactionMap::value_type* it = Factions.Find(hash);
        if (it != Factions.Last) return it->second.Get();
        return 0;
    }
    CFaction* GetPFaction(const CCharString& name);
};
CFaction* CFactionManager::GetPFaction(const CCharString& name)
{
    return GetPFaction(CRCCalc(0, name.PStringData == 0 ? "" : name.PStringData->Text, name.PStringData == 0 ? 0 : name.PStringData->Length));
}
CFactionMap::value_type* CFactionMap::LowerBound(const unsigned long& key)
{
    value_type* first = First;
    long count = Last - First;
    while (count > 0) {
        long half = count >> 1;
        if (first[half].first < key) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
static CCharString Make(CCharStringData& d, const char* s) { d.Text = s; d.Length = (unsigned long)std::strlen(s); CCharString c; c.PStringData = &d; return c; }
int main()
{
    const char* names[3] = { "Bandit", "Guard", "Hero" };
    CFaction f[3]; CFactionMap::value_type entries[3];
    int i = 0;
    for (; i < 3; ++i) { f[i].Id = i + 1; entries[i].first = CRCCalc(0, names[i], (unsigned long)std::strlen(names[i])); entries[i].second.Data = &f[i]; }
    // keep the map sorted by hash (insertion sort on 3 entries)
    for (i = 1; i < 3; ++i) { CFactionMap::value_type v = entries[i]; int j = i - 1; while (j >= 0 && entries[j].first > v.first) { entries[j + 1] = entries[j]; --j; } entries[j + 1] = v; }
    CFactionManager m; m.Factions.First = entries; m.Factions.Last = entries + 3; m.Factions.EndOfStorage = entries + 3;
    CCharStringData d0, d1, d2, d3;
    bool ok = true;
    ok = ok && m.GetPFaction(Make(d0, "Guard")) == &f[1];
    ok = ok && m.GetPFaction(Make(d1, "Hero")) == &f[2];
    ok = ok && m.GetPFaction(Make(d2, "Bandit")) == &f[0];
    ok = ok && m.GetPFaction(Make(d3, "Villager")) == 0;
    CCharString empty; empty.PStringData = 0;                // null string data -> CRC of "" (length 0) -> absent
    ok = ok && m.GetPFaction(empty) == 0;
    CFactionManager e; e.Factions.First = e.Factions.Last = e.Factions.EndOfStorage = entries;   // empty map
    ok = ok && e.GetPFaction(Make(d0, "Guard")) == 0;
    if (!ok) { std::printf("006b93d0_TEST FAIL\n"); return 1; }
    std::printf("006b93d0_TEST PASS\n");
    return 0;
}