#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
// Behaviour of CTCInventoryMap::RemapUIDsForLoading (0x005FAE90): every UID in the vector is replaced by the remap result.
struct CThingManager {
    long calls;
    u64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, u64 uid)
    { ++calls; return saving ? 0 : uid * 10 + (levelLess ? 1 : 0) + (u64)level * 100; }
};
static CThingManager g_manager;
struct CThing { short m_level; bool m_levelLess; bool IsLevelLess() { return m_levelLess; } };
struct CUIDVector { u64* m_first; u64* m_last; u64* m_end; };
struct CTCInventoryMap {
    CThing* m_pParentThing;
    CThingManager* GetThingManager() { return &g_manager; }
    void RemapUIDsForLoading(CUIDVector& uids)
    {
        for (u64* it = uids.m_first; it != uids.m_last; ++it)
            *it = GetThingManager()->RemapUIDForSerialisation(false, m_pParentThing->IsLevelLess(), m_pParentThing->m_level, *it);
    }
};
int main()
{
    u64 data[3] = { 1, 2, 3 };
    CUIDVector v = { data, data + 3, data + 3 };
    CThing thing; thing.m_level = 2; thing.m_levelLess = true;
    CTCInventoryMap map; map.m_pParentThing = &thing;
    g_manager.calls = 0;
    map.RemapUIDsForLoading(v);
    if (g_manager.calls != 3 || data[0] != 211 || data[1] != 221 || data[2] != 231) { std::printf("005fae90_TEST FAIL\n"); return 1; }
    CUIDVector empty = { data, data, data };
    map.RemapUIDsForLoading(empty);
    if (g_manager.calls != 3) { std::printf("005fae90_TEST FAIL\n"); return 1; }
    std::printf("005fae90_TEST PASS\n");
    return 0;
}