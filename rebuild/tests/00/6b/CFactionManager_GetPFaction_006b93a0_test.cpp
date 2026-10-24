// Behaviour test for 0x006B93A0 CFactionManager::GetPFaction(unsigned long hash) const.
#include <cstdio>

struct CFaction { int id; };

template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    T* Get() const { return Data; }
};
template <class A, class B> struct pair { A first; B second; };

struct CFactionMap {
    typedef pair<unsigned int, CCountedPointer<CFaction> > value_type;
    value_type* First;
    value_type* Last;
    value_type* EndOfStorage;
    const value_type* LowerBound(const unsigned int& key) const
    {
        const value_type* first = First;
        long count = (long)(Last - First);
        while (count > 0) {
            long half = count >> 1;
            if (first[half].first < key) { first += half + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
    const value_type* Find(const unsigned int& key) const
    {
        const value_type* it = LowerBound(key);
        if (it == Last || key < it->first)
            return Last;
        return it;
    }
};

struct CFactionManager {
    CFactionMap Factions;
    CFaction* GetPFaction(unsigned int hash) const
    {
        const CFactionMap::value_type* it = Factions.Find(hash);
        if (it != Factions.Last)
            return it->second.Get();
        return 0;
    }
};

int main()
{
    CFaction f[3];
    CFactionMap::value_type entries[3];
    unsigned int keys[3] = { 10, 0x80000000u, 0xfffffff0u };
    for (int i = 0; i < 3; ++i) { entries[i].first = keys[i]; entries[i].second.Data = &f[i]; entries[i].second.Info = 0; }
    CFactionManager m;
    m.Factions.First = entries; m.Factions.Last = entries + 3; m.Factions.EndOfStorage = entries + 3;
    int ok = m.GetPFaction(10) == &f[0] && m.GetPFaction(0x80000000u) == &f[1] && m.GetPFaction(0xfffffff0u) == &f[2] &&
             m.GetPFaction(5) == 0 && m.GetPFaction(11) == 0 && m.GetPFaction(0xffffffffu) == 0;
    m.Factions.Last = entries;                          // empty map
    if (m.GetPFaction(10) != 0) ok = 0;
    if (!ok) { std::printf("006b93a0_TEST FAIL\n"); return 1; }
    std::printf("006b93a0_TEST PASS\n");
    return 0;
}