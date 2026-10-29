// CAIStateGroup_VersusEnemyBase::OnReplace() -- drop creature mode 0x19 and leave the combat manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00962B80, length 129 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface on the brain's creature, type 0x31 = bit 17
// of the presence word at +0x24), CTCCreatureModeManager::IsModeActive(long) @ 0x0073A8A0,
// CTCCreatureModeManager::RemoveMode(long) @ 0x0073ABA0, CThingCreatureBase::IsInMeleeAsAttacker @ 0x00662410
// (landed name), CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CCombatManager::DeregisterCombatant @ 0x006ED9C0.
//
// Shape: the mode manager comes from the out-parameter GetInterface whose result is not tested (a miss reads
// the unwritten frame slot into esi, `mov esi,[esp+8]`); mode 0x19 is removed when present. Then the landed
// 0x009540C0 (MazeBase::OnReplace) tail: a registered combatant is read into a named local before the world
// call and deregistered from the world's combat manager (+0x4c). The registration test runs on its own named
// creature local (`self`): with the inline chain in the condition the two brain loads swap eax/edx. `this` is the PDB header layout; PBrain lives
// in the flattened base bytes at +0x04.
#include "engine/CAIStateGroup_VersusEnemyBase.h"

class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CTCCreatureModeManager { public:                 // interface type 0x31 component
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
    void RemoveMode(unsigned long);   // 0x0073ABA0
};

class CThingCreatureBase : public CThing { public:
public:
    bool IsInMeleeAsAttacker() const;   // 0x00662410
};

class CCombatManager { public:
    void DeregisterCombatant(CThingCreatureBase* creature);   // 0x006ED9C0
};

class CWorld { public:
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
    if (modes->IsModeActive(0x19))
        modes->RemoveMode(0x19);
    CThingCreatureBase* self = GetPBrain()->PCreature;
    if (self->IsInMeleeAsAttacker()) {
        CThingCreatureBase* creature = GetPBrain()->PCreature;
        CWorld* world = GetPWorld();
        world->m_pCombatManager->DeregisterCombatant(creature);
    }
}