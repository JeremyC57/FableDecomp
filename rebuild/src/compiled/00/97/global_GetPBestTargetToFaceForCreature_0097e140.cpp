// GetPBestTargetToFaceForCreature(const CThingCreatureBase& creature) -- the thing an AI creature should face: in mode 25 (combat), the creature it is attacking in melee when it is in melee as the attacker and able to strafe, else its targeting component's best enemy; null otherwise
// Leaf: ?GetPBestTargetToFaceForCreature@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097E140, 190 bytes, __fastcall (ecx = creature), plain `ret`.
// Identity: Ego_r.pdb ?GetPBestTargetToFaceForCreature@?A0xbf588e90@@YIPAVCThing@@ABVCThingCreatureBase@@@Z, an
//   anonymous-namespace helper of the CAIStateGroupBase TU (callers AIAction_MoveToPos 0x0097EC00, MoveToPosProximity
//   0x0097E890, FleeToThingProximity 0x009804A0; the landed callers declare it __fastcall at this address).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (the creature is const), CTCCreatureModeManager::
//   GetCurrentMode const @ 0x0073A6E0 (interface 0x31) + the mode's virtual slot 1 GetModeType, CThingCreatureBase::IsInMelee
//   const @ 0x00661E80, IsInMeleeAsAttacker const @ 0x00662410, IsAbleToStrafe const @ 0x006621A0, GetCombatManager const @
//   0x00661EC0, CCombatManager::GetCombatantInfo const @ 0x006E9360 (CCombatantInfo::AttackerInfo.Attacked +4), the
//   targeting component's (interface 8, CTCTargetingAI) virtual slot 29 GetPBestEnemyCreatureForAttack.
//   Callee identities from the Ego_r.pdb order around the retail bodies (IsAbortingCurrentAction 0x00661E70 / IsInMelee
//   0x00661E80 / GetCombatManager const 0x00661EC0 + non-const 0x00661ED0; IsAbleToStrafe 0x006621A0 before
//   IsFreeToFinishAction; GetMeleeWeaponDef / GetRangedWeaponDef / IsInMeleeAsAttacker 0x00662410 / IsInMeleeAsAttacked).
// Layout: creature interface bitset +0x20 (0x31 = bit 17 of +0x24, 8 = bit 8 of +0x20), interface map +0x44.
// Shape: two const GetInterface lookups written as HasTC + the one-ternary entry pick (component straight into ecx); VC
//   keeps the map address in esi across the combat tests and reuses it for the second lookup; both keys share the
//   `push ecx` frame word.
class CThing { };
class CThingCreatureBase;
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CModeBase {                               // CTCCreatureModeManager::CModeBase (local model)
public:
    virtual void v00();
    virtual long GetModeType() const;           // slot 1 (+4)
};
class CTCCreatureModeManager {                  // interface 0x31
public:
    CModeBase& GetCurrentMode() const;          // 0x0073A6E0
};
class CTCTargetingAI {                          // interface 8
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThingCreatureBase* GetPBestEnemyCreatureForAttack() const;   // slot 29 (+0x74)
};
class CAttackingCombatantInfo { public:
    CThingCreatureBase* Attacked;               // +0x00
};
struct CCombatantInfo {
    CThingCreatureBase*     Parent;             // +0x00
    CAttackingCombatantInfo AttackerInfo;       // +0x04
};
class CCombatManager {
public:
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const;   // 0x006E9360
};
class CThingCreatureBase : public CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline unsigned long HasTC(long type) const
    {
        return m_presentBits[type >> 5] & (1UL << (type & 31));
    }
    template<class T> __forceinline const T* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (const T*)((it == map->last || type < it->first) ? map->last : it)->second;
    }
    bool IsInMelee() const;                     // 0x00661E80
    bool IsInMeleeAsAttacker() const;           // 0x00662410
    bool IsAbleToStrafe() const;                // 0x006621A0
    const CCombatManager& GetCombatManager() const;   // 0x00661EC0
};

namespace {
CThing* __fastcall GetPBestTargetToFaceForCreature(const CThingCreatureBase& creature)
{
    if (creature.HasTC(0x31) && creature.GetTC<CTCCreatureModeManager>(0x31)->GetCurrentMode().GetModeType() == 25) {
        if (creature.IsInMelee() && creature.IsInMeleeAsAttacker() && creature.IsAbleToStrafe())
            return creature.GetCombatManager().GetCombatantInfo(creature).AttackerInfo.Attacked;
        if (creature.HasTC(8))
            return creature.GetTC<CTCTargetingAI>(8)->GetPBestEnemyCreatureForAttack();
    }
    return 0;
}
}

CThing* (__fastcall *g_keepGetPBestTargetToFaceForCreature)(const CThingCreatureBase&) =
    &GetPBestTargetToFaceForCreature;   // keeps the internal-linkage body emitted; not part of the retail function