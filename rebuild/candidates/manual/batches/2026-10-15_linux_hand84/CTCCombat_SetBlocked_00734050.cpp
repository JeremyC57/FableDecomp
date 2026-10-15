// CTCCombat::SetBlocked(CThing* blocker) -- when the blocking thing is a creature-type thing with interface 48, that component's +0x50 frame counter is set to two seconds of frames; then the parent creature is deregistered from the combat manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00734050, length 96 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, __ftol2 @ 0x00BFEA70,
// CCombatManager::DeregisterCombatant(CThingCreatureBase*) @ 0x006EA430 (landed). Global: the frame rate
// long @ 0x01375550 (15).
// Layout = engine/CTCCombat.h: CombatManager +0x20, ParentCreature +0x24. CThing: the thing-type byte
// +0x10, the interface presence bitset +0x20 (type 48 -> word 1 bit 16: `test dword ptr [eax+24h],10000h`),
// the sorted interface map +0x44.
// Shape: `type == 2 && GetInterface(48, &component)` with the landed out-parameter GetInterface
// template (0x007504E0): the key temporary is homed in the consumed argument slot (`lea eax,[esp+0Ch]`),
// the map pointer is a named local (esi); `(long)(g_FramesPerSecond * 2.0f)` is `fild ; fadd st0,st0 ;
// __ftol2`; the deregistration is unconditional.
struct CTCBase;

struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct CTCBase {
    unsigned char _pad[0x50];
    long          m_frameCounter;           // +0x50
};

struct CThing {
    unsigned char _pad0[0x10];
    unsigned char m_thingType;              // +0x10 (2 = creature)
    unsigned char _pad11[0xf];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    __forceinline bool GetInterface(long type, CTCBase** out)   // the landed GetInterface_0x.. family shape (forced: inside the `&&` VC calls it out of line)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = it->component;
            return true;
        }
        return false;
    }
};

extern long g_FramesPerSecond;              // 0x01375550

#include "engine/CTCCombat.h"

struct CThingCreatureBase;
struct CCombatManager {
    void DeregisterCombatant(CThingCreatureBase* creature);   // 0x006EA430
};

struct CTCCombat_Methods : CTCCombat {
    void SetBlocked(CThing* blocker);
};

void CTCCombat_Methods::SetBlocked(CThing* blocker)
{
    CTCBase* component;
    if (blocker->m_thingType == 2 && blocker->GetInterface(48, &component))
        component->m_frameCounter = (long)(g_FramesPerSecond * 2.0f);
    CombatManager->DeregisterCombatant(ParentCreature);
}
