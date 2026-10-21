// CAIStateGroup_VersusEnemyBase::OnReplace() -- drop creature mode 0x19 and leave the combat manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00962B80, length 129 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface on the brain's creature, type 0x31 = bit 17
// of the presence word at +0x24), CTCCreatureModeManager::HasMode(long) @ 0x0073A8A0,
// CTCCreatureModeManager::RemoveMode(long) @ 0x0073ABA0, CThingCreatureBase::IsRegisteredCombatant @ 0x00662410
// (landed name), CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CCombatManager::DeregisterCombatant @ 0x006ED9C0.
//
// Shape: the mode manager comes from the out-parameter GetInterface whose result is not tested (a miss reads
// the unwritten frame slot into esi, `mov esi,[esp+8]`); mode 0x19 is removed when present. Then the landed
// 0x009540C0 (MazeBase::OnReplace) tail: a registered combatant is read into a named local before the world
// call and deregistered from the world's combat manager (+0x4c). The registration test runs on its own named
// creature local (`self`): with the inline chain in the condition the two brain loads swap eax/edx. `this` is the PDB header layout; PBrain lives
// in the flattened base bytes at +0x04.
#include "engine/CAIStateGroup_VersusEnemyBase.h"

struct CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {                // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                          // +0x00
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CTCCreatureModeManager {                 // interface type 0x31 component
    bool HasMode(long mode);                    // 0x0073A8A0
    void RemoveMode(long mode);                 // 0x0073ABA0
};

struct CThingCreatureBase : CThing {
    bool IsRegisteredCombatant();               // 0x00662410
};

struct CCombatManager {
    void DeregisterCombatant(CThingCreatureBase* creature);   // 0x006ED9C0
};

struct CWorld {
    unsigned char _pad[0x4c];
    CCombatManager* m_pCombatManager;           // +0x4c
};

struct CAIBrainCreature {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

struct CAIStateGroup_VersusEnemyBase_Methods : CAIStateGroup_VersusEnemyBase {
    CAIBrainCreature* GetPBrain() const { return *(CAIBrainCreature* const*)_pad_0x04; }   // PBrain (+0x04)
    CWorld* GetPWorld();                        // 0x008FCFC0
    void OnReplace();
};

void CAIStateGroup_VersusEnemyBase_Methods::OnReplace()
{
    CTCCreatureModeManager* modes;
    GetPBrain()->PCreature->GetInterface(&modes, 0x31);
    if (modes->HasMode(0x19))
        modes->RemoveMode(0x19);
    CThingCreatureBase* self = GetPBrain()->PCreature;
    if (self->IsRegisteredCombatant()) {
        CThingCreatureBase* creature = GetPBrain()->PCreature;
        CWorld* world = GetPWorld();
        world->m_pCombatManager->DeregisterCombatant(creature);
    }
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static bool g_hasMode = true; static int g_removes = 0, g_hasQueries = 0; static long g_lastRemoved = 0, g_lastQueried = 0;
static const void* g_modesThis = 0;
bool CTCCreatureModeManager::HasMode(long mode) { ++g_hasQueries; g_lastQueried = mode; g_modesThis = this; return g_hasMode; }
void CTCCreatureModeManager::RemoveMode(long mode) { ++g_removes; g_lastRemoved = mode; }
static bool g_registered = true; static const void* g_regThis = 0;
bool CThingCreatureBase::IsRegisteredCombatant() { g_regThis = this; return g_registered; }
static int g_deregs = 0; static const void* g_deregArg = 0; static const void* g_deregThis = 0;
void CCombatManager::DeregisterCombatant(CThingCreatureBase* creature) { ++g_deregs; g_deregArg = creature; g_deregThis = this; }
static CWorld g_world; static CCombatManager g_combat;
static const void* g_worldThis = 0;
CWorld* CAIStateGroup_VersusEnemyBase_Methods::GetPWorld() { g_worldThis = this; return &g_world; }
static CThingCreatureBase g_creature;
static CAIBrainCreature g_brain;
static CTCCreatureModeManager g_modes;
static CInterfaceMap::SEntry g_entries[2];
int main()
{
    g_world.m_pCombatManager = &g_combat;
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0x31; g_entries[1].component = (CTCBase*)&g_modes;
    g_creature.m_interfaces.first = g_entries; g_creature.m_interfaces.last = g_entries + 2;
    g_creature.m_presentBits[0x31 >> 5] = 1UL << (0x31 & 31);
    g_brain.PCreature = &g_creature;
    static CAIStateGroup_VersusEnemyBase_Methods group;
    CAIBrainCreature* brain = &g_brain;
    memcpy(group._pad_0x04, &brain, 4);
    group.OnReplace();
    CHECK(g_hasQueries == 1 && g_lastQueried == 0x19 && g_modesThis == &g_modes && g_removes == 1 && g_lastRemoved == 0x19);
    CHECK(g_regThis == &g_creature && g_deregs == 1 && g_deregArg == &g_creature && g_deregThis == &g_combat && g_worldThis == &group);
    g_hasMode = false; g_registered = false;        // mode absent, not a combatant
    group.OnReplace();
    CHECK(g_hasQueries == 2 && g_removes == 1 && g_deregs == 1);
    g_registered = true;
    group.OnReplace();
    CHECK(g_removes == 1 && g_deregs == 2);
    if (fails == 0) printf("00962b80_TEST PASS\n");
    return fails;
}