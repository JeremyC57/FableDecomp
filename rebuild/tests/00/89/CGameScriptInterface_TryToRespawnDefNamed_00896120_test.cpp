// Behaviour test for 00896120: body copied from src/00896120.cpp (CGameScriptInterface::TryToRespawnDefNamed), the thing / village callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CThing_Retail;
class CCharString { public: void* PStringData; };
struct CTCBase_Retail;
struct CInterfaceMap_Retail {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
namespace N_0099a2d0 { struct CBase { virtual ~CBase() {} }; } using N_0099a2d0::CBase;
struct CScriptThing : CBase {
    virtual CThing_Retail* GetThing() const { return m_thing; }
    virtual bool IsValid() const { return m_thing != 0; }
    CThing_Retail* m_thing;
    void* m_pItem;
    CScriptThing() : m_thing(0), m_pItem(0) { }
};
struct CThing_Retail {
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];
    CInterfaceMap_Retail  m_interfaces;
    unsigned char         _pad4c[0x45];
    unsigned char         m_dead;
    unsigned char         _pad92[0x40];
    int                   m_id;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    CScriptThing GetScriptThing() { CScriptThing s; s.m_thing = this; return s; }
};

static int g_respawn = 0; static const CCharString* g_name = 0; static long g_count = 0; static void* g_respawnThis = 0; static CThing_Retail* g_respawnResult = 0;
struct CTCVillage_Retail { CThing_Retail* TryToRespawnDefNamed(const CCharString& defName, long count) { ++g_respawn; g_name = &defName; g_count = count; g_respawnThis = this; return g_respawnResult; } };
struct CGameScriptInterface {
    virtual void v000() {}
    CScriptThing TryToRespawnDefNamed(const CScriptThing& village, const CCharString& defName, long count) const;
};
CScriptThing CGameScriptInterface::TryToRespawnDefNamed(const CScriptThing& village, const CCharString& defName, long count) const
{
    CThing_Retail* thing = village.GetThing();
    if (thing && !thing->m_dead) {
        if (thing->HasInterface(0x22)) {
            CTCVillage_Retail* component = thing->GetTC<CTCVillage_Retail>(0x22);
            CThing_Retail* creature = component->TryToRespawnDefNamed(defName, count);
            if (creature)
                return creature->GetScriptThing();
        }
    }
    return CScriptThing();
}
int main()
{
    CGameScriptInterface gsi; CCharString name; name.PStringData = 0;
    CThing_Retail village; memset(&village, 0, sizeof(village)); CThing_Retail creature; memset(&creature, 0, sizeof(creature)); creature.m_id = 42;
    CTCVillage_Retail comp; CTCBase_Retail* other = (CTCBase_Retail*)0x99;
    CInterfaceMap_Retail::SEntry entries[2] = { { 0x22, (CTCBase_Retail*)&comp }, { 0x30, other } };
    village.m_interfaces.first = entries; village.m_interfaces.last = entries + 2;
    village.m_presentBits[0x22 >> 5] = 1UL << (0x22 & 31);
    CScriptThing st; st.m_thing = &village;
    // 1. respawned: the creature's script thing comes back
    g_respawnResult = &creature;
    CScriptThing r = gsi.TryToRespawnDefNamed(st, name, 5);
    CHECK(r.m_thing == &creature && r.IsValid() && g_respawn == 1 && g_name == &name && g_count == 5 && g_respawnThis == &comp);
    // 2. nothing respawned: an empty script thing
    g_respawnResult = 0; r = gsi.TryToRespawnDefNamed(st, name, 1); CHECK(r.m_thing == 0 && !r.IsValid() && g_respawn == 2);
    // 3. no village thing / dead village / no village component: no call
    CScriptThing none; r = gsi.TryToRespawnDefNamed(none, name, 1); CHECK(r.m_thing == 0 && g_respawn == 2);
    village.m_dead = 1; r = gsi.TryToRespawnDefNamed(st, name, 1); CHECK(r.m_thing == 0 && g_respawn == 2); village.m_dead = 0;
    village.m_presentBits[0x22 >> 5] = 0; r = gsi.TryToRespawnDefNamed(st, name, 1); CHECK(r.m_thing == 0 && g_respawn == 2);
    if (fails == 0) printf("00896120_TEST PASS\n"); else printf("00896120_TEST FAIL\n");
    return fails;
}