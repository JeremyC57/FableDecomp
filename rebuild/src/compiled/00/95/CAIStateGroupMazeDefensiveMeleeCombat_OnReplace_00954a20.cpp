// CAIStateGroup_MazeDefensiveMeleeCombat::OnReplace() -- deregister the creature from the combat manager if it is a registered combatant, and drop creature mode 0x19 if it is active
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the world in a named local and the combat manager chained from it (`world->m_pCombatManager->DeregisterCombatant(creature)`): the manager loads straight into ecx after the GetPWorld call, then the creature push
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954A20, length 129 bytes (+ int3 padding); the first block is the
// CAIStateGroup_JackOfBladesBase::Cleanup (0x00972110) one.
// Callees: CThingCreatureBase::IsInMeleeAsAttacker @ 0x00662410 (landed name; manifest
// "C3DAnimationBlendState::AddCombinationBlend" is propagated), CAIStateGroupBase::GetPWorld @ 0x008FCFC0,
// CCombatManager::DeregisterCombatant(CThingCreatureBase*) @ 0x006ED9C0 (world +0x4c), CVectorMap::LowerBound
// @ 0x0040F020 (inlined interface lookup, type 0x31 = CTCCreatureModeManager), CTCCreatureModeManager::IsModeActive
// @ 0x0073A8A0 (descriptive), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0.
//
// Shape: the creature is a named local read before the world call (edi) and the combat manager a named
// local after it; the second block is the out-parameter GetInterface template with its result ignored (the
// arms join in esi; the miss arm reads the never-written manager local from the key's `push ecx` slot), then
// `if (modes->HasMode(0x19)) modes->RemoveMode(0x19);`. No engine header models the
// state group's vptr chain: local model.
// Tried: combat-manager local (this, best: world call first but the manager lands in eax + `mov ecx,eax`), the
// direct chain `GetPWorld()->m_pCombatManager->Deregister(creature)` (the lane pushes the creature before the
// world call, 139v141).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);      // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
public:
    bool IsInMeleeAsAttacker() const;   // 0x00662410
};

class CTCCreatureModeManager { public:                 // interface type 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};

class CCombatManager { public:
    void DeregisterCombatant(CThingCreatureBase* creature);   // 0x006ED9C0
};

class CWorld { public:
    unsigned char _pad[0x4c];
    CCombatManager* m_pCombatManager;           // +0x4c
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9 (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    CWorld* GetPWorld();                        // 0x008FCFC0
};

struct CAIStateGroup_MazeDefensiveMeleeCombat : CAIStateGroupBase {
    virtual void OnReplace();
};

void CAIStateGroup_MazeDefensiveMeleeCombat::OnReplace()
{
    if (PBrain->PCreature->IsInMeleeAsAttacker()) {
        CThingCreatureBase* creature = PBrain->PCreature;
        CWorld* world = GetPWorld();
        world->m_pCombatManager->DeregisterCombatant(creature);
    }
    CTCCreatureModeManager* modes;
    PBrain->PCreature->GetInterface(&modes, 0x31);
    if (modes->IsModeActive(0x19))
        modes->RemoveMode(0x19);
}