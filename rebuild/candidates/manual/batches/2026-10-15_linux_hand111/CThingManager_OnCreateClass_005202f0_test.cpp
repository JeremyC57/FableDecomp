#include <cstdio>
// Behaviour test for CThingManager::OnCreateClass (0x005202F0): {UniqueID, &thing} inserted.
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct CThing { u64 UniqueID; };
struct SUIDMapValue {
    u64 first; CThing* second;
    SUIDMapValue(const u64& k, CThing* const& v) : first(k), second(v) { }
};
struct CUIDToThingMap {
    u64 keys[8]; CThing* values[8]; int count;
    bool insert(const SUIDMapValue& v)
    {
        for (int i = 0; i < count; ++i) if (keys[i] == v.first) return false;
        keys[count] = v.first; values[count] = v.second; ++count; return true;
    }
};
struct CThingManager {
    CUIDToThingMap UIDToThingMap;
    void OnCreateClass(CThing& thing) { UIDToThingMap.insert(SUIDMapValue(thing.UniqueID, &thing)); }
};
int main()
{
    CThingManager m; m.UIDToThingMap.count = 0;
    CThing a, b; a.UniqueID = ((u64)1 << 32 | 2); b.UniqueID = 7;
    m.OnCreateClass(a); m.OnCreateClass(b); m.OnCreateClass(a);
    int fails = 0;
    if (m.UIDToThingMap.count != 2 || m.UIDToThingMap.keys[0] != ((u64)1 << 32 | 2) || m.UIDToThingMap.values[0] != &a || m.UIDToThingMap.values[1] != &b) ++fails;
    if (fails) { std::printf("005202f0_TEST FAIL\n"); return 1; }
    std::printf("005202f0_TEST PASS\n");
    return 0;
}
