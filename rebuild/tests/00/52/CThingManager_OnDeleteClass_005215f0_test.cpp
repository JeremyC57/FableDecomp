#include <cstdio>
// Behaviour test for CThingManager::OnDeleteClass (0x005215F0): the thing's UniqueID erased.
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CThing { public: u64 UniqueID; u64 GetUniqueID() const { return UniqueID; } };
struct CUIDToThingMap {
    u64 keys[8]; int count;
    unsigned long erase(const u64& key)
    {
        for (int i = 0; i < count; ++i) if (keys[i] == key) { keys[i] = keys[--count]; return 1; }
        return 0;
    }
};
class CThingManager { public:
    CUIDToThingMap UIDToThingMap;
    void OnDeleteClass(CThing& thing) { UIDToThingMap.erase(thing.GetUniqueID()); }
};
int main()
{
    CThingManager m; m.UIDToThingMap.count = 3;
    m.UIDToThingMap.keys[0] = 1; m.UIDToThingMap.keys[1] = ((u64)5 << 32); m.UIDToThingMap.keys[2] = 3;
    CThing t; t.UniqueID = ((u64)5 << 32);
    int fails = 0;
    m.OnDeleteClass(t);
    if (m.UIDToThingMap.count != 2 || m.UIDToThingMap.keys[1] != 3) ++fails;
    m.OnDeleteClass(t);
    if (m.UIDToThingMap.count != 2) ++fails;
    if (fails) { std::printf("005215f0_TEST FAIL\n"); return 1; }
    std::printf("005215f0_TEST PASS\n");
    return 0;
}