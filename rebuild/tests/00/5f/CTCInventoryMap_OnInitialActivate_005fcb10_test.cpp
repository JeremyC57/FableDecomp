// Behaviour test for CTCInventoryMap::OnInitialActivate @ 0x005FCB10 (model copy; stub thing manager, teleporters, count block).
#include <cstdio>
#include <cstdlib>
typedef long long int64_model;
static long g_blockDestroys = 0; static void* g_destroyedObject = 0;
static void destroy_stub(void* object) { ++g_blockDestroys; g_destroyedObject = object; }
static long g_deletes = 0; static void* g_lastDeleted = 0;
struct SCountBlock {
    long m_references; void (*m_pfnDestroy)(void*); void* m_pObject;
    static void operator delete(void* p) { ++g_deletes; g_lastDeleted = p; }   // spy: records, never frees
};
struct CUIDVector { int64_model* _M_start; int64_model* _M_finish; int64_model* _M_end_of_storage; };
struct CSerialisedUIDPointer {
    CUIDVector* Data; SCountBlock* Info;
    void Reset()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) { Info->m_pfnDestroy(Info->m_pObject); delete Info; }
        }
        Info = 0; Data = 0;
    }
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
class CThing { public:
    unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; unsigned char _pad4c[0x91 - 0x4c];
    unsigned char IsDead : 1;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
};
class CThingManager { public: CThing* things[8]; CThing* GetThingWithUID(int64_model uid) { return uid >= 0 && uid < 8 ? things[(long)uid] : 0; } };
class CTCTeleporter { public: long activations; bool lastActive; void SetAsActive(bool active) { ++activations; lastActive = active; } };
struct CTCInventoryBase_Map {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void OnInitialActivate() { ++baseActivates; }
    unsigned char _pad04[0x200 - 0x04];
    CThingManager* manager; long baseActivates;
    CThingManager* GetThingManager() { return manager; }
};
class CTCInventoryMap : public CTCInventoryBase_Map { public:
    CSerialisedUIDPointer SerialisedTeleporterUIDs;
    virtual void OnInitialActivate();
};
void CTCInventoryMap::OnInitialActivate()
{
    CTCInventoryBase_Map::OnInitialActivate();
    CUIDVector* uids = SerialisedTeleporterUIDs.Data;
    if (uids) {
        for (int64_model* it = uids->_M_start; it != uids->_M_finish; ++it) {
            int64_model uid = *it;
            CThing* thing = GetThingManager()->GetThingWithUID(uid);
            if (thing && !thing->IsDead) {
                CTCTeleporter* teleporter;
                if (thing->GetInterface(&teleporter, 0xDD))
                    teleporter->SetAsActive(true);
            }
        }
        SerialisedTeleporterUIDs.Reset();
    }
}
static void MakeThing(CThing& t, CInterfaceMap::SEntry* e, long n, bool teleporter, bool dead)
{
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    if (teleporter) t.m_presentBits[0xDD >> 5] |= 1UL << (0xDD & 31);
    t.m_interfaces.first = e; t.m_interfaces.last = e + n; t.IsDead = dead ? 1 : 0;
}
int main()
{
    CTCTeleporter tp1, tp2, tp3; tp1.activations = tp2.activations = tp3.activations = 0;
    CInterfaceMap::SEntry e1[2] = { { 0x10, 0 }, { 0xDD, &tp1 } };
    CInterfaceMap::SEntry e2[1] = { { 0xDD, &tp2 } };
    CInterfaceMap::SEntry e3[1] = { { 0xDD, &tp3 } };
    CInterfaceMap::SEntry e4[1] = { { 0x10, 0 } };
    CThing t1, t2, t3, t4;
    MakeThing(t1, e1, 2, true, false); MakeThing(t2, e2, 1, true, true); MakeThing(t3, e3, 1, true, false); MakeThing(t4, e4, 1, false, false);
    CThingManager manager; for (int i = 0; i < 8; ++i) manager.things[i] = 0;
    manager.things[1] = &t1; manager.things[2] = &t2; manager.things[3] = &t3; manager.things[4] = &t4;
    int64_model uids[6] = { 1, 2, 3, 4, 7, 0x100000000LL + 3 };
    CUIDVector vec; vec._M_start = uids; vec._M_finish = uids + 6; vec._M_end_of_storage = uids + 6;
    SCountBlock block; block.m_references = 1; block.m_pfnDestroy = destroy_stub; block.m_pObject = &vec;
    CTCInventoryMap map; map.manager = &manager; map.baseActivates = 0;
    map.SerialisedTeleporterUIDs.Data = &vec; map.SerialisedTeleporterUIDs.Info = &block;
    map.OnInitialActivate();
    bool ok = true;
    if (map.baseActivates != 1) ok = false;
    if (tp1.activations != 1 || !tp1.lastActive || tp3.activations != 1 || tp2.activations != 0) ok = false;   // dead thing skipped, no-teleporter thing skipped
    if (g_blockDestroys != 1 || g_destroyedObject != &vec || g_deletes != 1 || g_lastDeleted != &block) ok = false;
    if (map.SerialisedTeleporterUIDs.Data != 0 || map.SerialisedTeleporterUIDs.Info != 0) ok = false;
    map.OnInitialActivate();                                   // no list: only the base hook
    if (map.baseActivates != 2 || tp1.activations != 1 || g_deletes != 1) ok = false;
    // a shared block (two references) is only decremented
    SCountBlock shared; shared.m_references = 2; shared.m_pfnDestroy = destroy_stub; shared.m_pObject = &vec;
    map.SerialisedTeleporterUIDs.Data = &vec; map.SerialisedTeleporterUIDs.Info = &shared;
    map.OnInitialActivate();
    if (shared.m_references != 1 || g_blockDestroys != 1 || g_deletes != 1 || tp1.activations != 2 || map.SerialisedTeleporterUIDs.Info != 0) ok = false;
    if (!ok) { std::printf("005fcb10_TEST FAIL\n"); return 1; }
    std::printf("005fcb10_TEST PASS\n");
    return 0;
}