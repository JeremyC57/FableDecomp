// CTCFireRangedWeaponOnLeavingStateGroup::Cleanup(CThingAICreature& creature) -- take the creature out of its combat manager's combatant list, then drop creature mode 0x19 if it has a mode manager
// Leaf: ?Cleanup@CTCFireRangedWeaponOnLeavingStateGroup@@SIXAAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E5B40, 78 bytes (+ int3 padding). FableWin.pdb
//   `?Cleanup@CTCFireRangedWeaponOnLeavingStateGroup@@SIXAAVCThingAICreature@@@Z` = public static void __fastcall
//   (CThingAICreature&); callers: the component's vfunc_2 @ 0x007E5CC0 (tail jmp) and
//   CAIStateGroup_CreatureRangedCombat2::Cleanup @ 0x0091E990.
// Callees: CThingCreatureBase::GetCombatManager @ 0x00661ED0, CCombatManager::DeregisterCombatant @ 0x006ED9C0,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (non-const) @ 0x0040F020,
//   CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0 (interface type 0x31 = presence word 1 bit 17).
// Shape: `GetCombatManager()->DeregisterCombatant(&creature)` pushes the argument before the getter call; the
//   out-parameter PeekTC template inlined with the map in a named local (`add esi,44h`, `last` re-read through it).
//   No CThingAICreature engine header exists: local model of the thing base (bitset +0x20, map +0x44).
class CTCBase;
class CCombatManager;
class CThingCreatureBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const ETCInterfaceType& kind);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool PeekTC(T** out)
    {
        if (HasTC(T::INTERFACE_TYPE)) {
            CTCMap* map = &m_tcs;
            STCEntry* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            if (it == map->m_pLast || T::INTERFACE_TYPE < it->first)
                it = map->m_pLast;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing {
public:
    CCombatManager& GetCombatManager();                         // 0x00661ED0
};

class CThingAICreature : public CThingCreatureBase {
};

class CCombatManager {
public:
    void DeregisterCombatant(CThingCreatureBase* creature);    // 0x006ED9C0
};

class CTCCreatureModeManager {
public:
    enum { INTERFACE_TYPE = 0x31 };
    void RemoveMode(unsigned long mode);                        // 0x0073ABA0
};

class CTCFireRangedWeaponOnLeavingStateGroup {
public:
    static void __fastcall Cleanup(CThingAICreature& creature);
};

void __fastcall CTCFireRangedWeaponOnLeavingStateGroup::Cleanup(CThingAICreature& creature)
{
    creature.GetCombatManager().DeregisterCombatant(&creature);
    CTCCreatureModeManager* modes;
    if (creature.PeekTC(&modes))
        modes->RemoveMode(0x19);
}