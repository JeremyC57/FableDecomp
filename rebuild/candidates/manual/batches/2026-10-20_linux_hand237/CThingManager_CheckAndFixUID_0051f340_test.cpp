// Behaviour test for CThingManager::CheckAndFixUID (0x0051F340): counters pushed past a loaded UID per tag, and a
// UID already owned by another thing re-allocated.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

struct CMap { u64 NextThingUID; };
struct CWorldMap {
    CMap Maps[4];
    bool Loaded[4];
    bool IsMapLoaded(long n) const { return Loaded[n]; }
    CMap& GetMap(long n) { return Maps[n]; }
    long GetMapNumberFromMapUID(unsigned long uid) const { return uid < 4 ? (long)uid : 0; }
};
struct CThingManager {
    CWorldMap* WorldMap;
    u64 Owned[4]; int OwnedCount;
    u64 CurrentUniqueID, CurrentLocalThingUniqueID;
    long LastMap; bool LastGlobal;
    bool IsOwned(u64 key) { for (int i = 0; i < OwnedCount; ++i) if (Owned[i] == key) return true; return false; }
    u64 AllocateNewThingUID(long mapNumber, bool global, bool) { LastMap = mapNumber; LastGlobal = global; return 999; }
    void CheckAndFixUID(u64& uid)
    {
        u64 counter = uid & (((u64)0xFF << 32) | 0xFFFFFFFF);
        u64 tag = uid >> 40;
        if (uid == 0)
            return;
        if (tag == 0xFFFFFD) {
            if (counter >= CurrentLocalThingUniqueID)
                CurrentLocalThingUniqueID = counter + 1;
        } else if (tag == 0xFFFFFF) {
            if (counter >= CurrentUniqueID)
                CurrentUniqueID = counter + 1;
        } else {
            long mapNumber = WorldMap->GetMapNumberFromMapUID((unsigned long)tag);
            if (WorldMap->IsMapLoaded(mapNumber)) {
                CMap& map = WorldMap->GetMap(mapNumber);
                if (counter >= map.NextThingUID)
                    map.NextThingUID = counter + 1;
            }
        }
        if (uid != 0) {
            u64 key = uid;
            if (IsOwned(key)) {
                long mapNumber = WorldMap->GetMapNumberFromMapUID((unsigned long)tag);
                bool global = tag == 0xFFFFFD;
                uid = AllocateNewThingUID(mapNumber, global, false);
            }
        }
    }
};

int main()
{
    CWorldMap wm;
    for (int i = 0; i < 4; ++i) { wm.Maps[i].NextThingUID = 5; wm.Loaded[i] = i != 3; }
    CThingManager m; m.WorldMap = &wm; m.OwnedCount = 0; m.CurrentUniqueID = 10; m.CurrentLocalThingUniqueID = 10;
    bool ok = true;
    u64 a = ((u64)0xFFFFFD << 40) | 50; m.CheckAndFixUID(a);
    ok = ok && m.CurrentLocalThingUniqueID == 51 && a == (((u64)0xFFFFFD << 40) | 50);
    u64 b = ((u64)0xFFFFFF << 40) | 3; m.CheckAndFixUID(b);
    ok = ok && m.CurrentUniqueID == 10;
    u64 c = ((u64)2 << 40) | 77; m.CheckAndFixUID(c);
    ok = ok && wm.Maps[2].NextThingUID == 78;
    u64 d = ((u64)3 << 40) | 77; m.CheckAndFixUID(d);
    ok = ok && wm.Maps[3].NextThingUID == 5;
    u64 z = 0; m.CheckAndFixUID(z); ok = ok && z == 0;
    m.Owned[0] = a; m.OwnedCount = 1; m.CheckAndFixUID(a);
    ok = ok && a == 999 && m.LastGlobal && m.LastMap == 0;
    m.Owned[0] = c; m.CheckAndFixUID(c);
    ok = ok && c == 999 && !m.LastGlobal && m.LastMap == 2;
    if (!ok) { std::printf("0051f340_TEST FAIL\n"); return 1; }
    std::printf("0051f340_TEST PASS\n");
    return 0;
}
