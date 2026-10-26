// Behaviour test for 0093e240: body copied from src/0093e240.cpp (CAIStateGroup_MeleeCombatNearby2::Cleanup), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
static bool g_registered = false, g_usesModes = false; static int g_isReg = 0, g_uses = 0;
class CThingCreatureBase { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool IsRegisteredCombatant() { ++g_isReg; return g_registered; }
    bool UsesCreatureModes() { ++g_uses; return g_usesModes; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
static int g_removeMode = 0; static unsigned long g_removedMode = 0; static void* g_modesThis = 0; static int g_hasModeCalls = 0; static bool g_hasMode = false;
class CTCCreatureModeManager { public:
    bool HasMode(unsigned long mode) { ++g_hasModeCalls; return g_hasMode; }
    void RemoveMode(unsigned long mode) { ++g_removeMode; g_removedMode = mode; g_modesThis = this; }
};
static int g_dereg = 0; static CThingCreatureBase* g_deregCreature = 0; static void* g_deregThis = 0;
class CCombatManager { public: void DeregisterCombatant(CThingCreatureBase* creature) { ++g_dereg; g_deregCreature = creature; g_deregThis = this; } };
class CWorld { public: unsigned char _pad[0x4c]; CCombatManager* m_pCombatManager; };
class CAIBrain { public: unsigned char _pad[0x18]; CThingCreatureBase* PCreature; };
static CWorld* g_world = 0; static int g_getWorld = 0;
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void Cleanup() {}
    CAIBrain* PBrain;
    unsigned char _pad[0x0c];
    CWorld* GetPWorld() { ++g_getWorld; return g_world; }
};
struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroupBase {
    virtual void Cleanup();
};
void CAIStateGroup_MeleeCombatNearby2::Cleanup()
{
    if (PBrain->PCreature->IsRegisteredCombatant()) {
        CThingCreatureBase* creature = PBrain->PCreature;
        CWorld* world = GetPWorld();
        world->m_pCombatManager->DeregisterCombatant(creature);
    }
    if (PBrain->PCreature->UsesCreatureModes()) {
        CTCCreatureModeManager* modes;
        PBrain->PCreature->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x19);
    }
}
int main()
{
    CThingCreatureBase creature; memset(&creature, 0, sizeof(creature)); CTCCreatureModeManager modes;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x31, (CTCBase*)&modes } };
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    creature.m_presentBits[0x31 >> 5] = 1UL << (0x31 & 31);
    CCombatManager combat; CWorld world; world.m_pCombatManager = &combat; g_world = &world;
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_MeleeCombatNearby2 g; g.PBrain = &brain;
    // 1. not registered, no modes: nothing
    g.Cleanup(); CHECK(g_isReg == 1 && g_uses == 1 && g_dereg == 0 && g_removeMode == 0 && g_getWorld == 0);
    // 2. registered: deregistered through the world's combat manager
    g_registered = true; g.Cleanup(); CHECK(g_getWorld == 1 && g_dereg == 1 && g_deregCreature == &creature && g_deregThis == &combat && g_removeMode == 0);
    // 3. uses modes: mode 0x19 removed from the mode manager component
    g_registered = false; g_usesModes = true; g.Cleanup(); CHECK(g_dereg == 1 && g_removeMode == 1 && g_removedMode == 0x19 && g_modesThis == &modes);
    if (fails == 0) printf("0093e240_TEST PASS\n"); else printf("0093e240_TEST FAIL\n");
    return fails;
}