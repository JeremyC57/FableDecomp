// CThingCreatureBase::GetShotAccuracyPercentage() -- a hero with its experience component asks the component; otherwise 100 while attacking a target that passes the 0x00864CB0 check, else the creature's own ShotAccuracyPercentage
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663B10, 132 bytes.
// Callees: CInterfaceMap::LowerBound @ 0x004365B0, CTCHeroExperience::GetShotAccuracyPercentage @ 0x006D6EA0,
//   CThing::GetWorld @ 0x004C7AB0 (label "CViewBase::GetDisplayEngine" is propagated),
//   CCombatManager::IsCreatureAttacking(const CThingCreatureBase&) @ 0x006E92C0 (landed),
//   CCombatManager::GetCombatantInfo(const CThing&) @ 0x006E9360 (landed as CUIDMapOwner_006e9360::GetValueForThing;
//   label "CSurface::GetHeight" is propagated), the `__fastcall` thing test @ 0x00864CB0 (label
//   "IsThingRunningAwayFromCombat"; it tests the thing's TC 0x36).
// Layout: retail CThing: interface bitset +0x20, CVectorMap<ETCInterfaceType, CTCBase*> +0x44, flag byte +0x6c (bit 5
//   tested here); CTCHeroExperience is TC 0x68 (bitset word +0x2c bit 8); retail CWorld+0x4c = combat manager;
//   CCombatantInfo +4 = the attacked thing; ShotAccuracyPercentage +0xe4 (PDB +0x100).
// Shape: the agent160 GetInterface model (__forceinline: bit test, key in a frame slot, LowerBound, `it == last ||
//   key < it->type` -> last), the map pointer replacing `this` in esi on that arm. Each combat-manager read is a named
//   local (`mov eax,[eax+4ch] ; push esi ; mov ecx,eax`).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCHeroExperience { public:
    long GetShotAccuracyPercentage();           // 0x006D6EA0
};
class CThing;
class CThingCreatureBase;
struct CCombatantInfo {
    long Parent;
    CThing* Attacked;                           // +0x04
};
class CCombatManager { public:
    bool IsCreatureAttacking(const CThingCreatureBase& creature);       // 0x006E92C0
    CCombatantInfo* GetCombatantInfo(const CThing& thing);              // 0x006E9360
};
class CWorld { public:
    unsigned char _pad[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
bool __fastcall IsThingRunningAwayFromCombat(CThing* thing);            // 0x00864CB0

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char m_flags6c;                    // +0x6c
    CWorld* GetWorld();                         // 0x004C7AB0
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (T*)it->second;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

class CThingCreatureBase : public CThing { public:
    unsigned char _pad70[0xe4 - 0x70];
    long ShotAccuracyPercentage;                // +0xe4
    long GetShotAccuracyPercentage();
};

long CThingCreatureBase::GetShotAccuracyPercentage()
{
    if ((m_flags6c & 0x20) && HasTC(0x68))
        return GetTC<CTCHeroExperience>(0x68)->GetShotAccuracyPercentage();
    CCombatManager* combat = GetWorld()->PCombatManager;
    if (combat->IsCreatureAttacking(*this)) {
        CCombatManager* combat2 = GetWorld()->PCombatManager;
        if (IsThingRunningAwayFromCombat(combat2->GetCombatantInfo(*this)->Attacked))
            return 100;
    }
    return ShotAccuracyPercentage;
}