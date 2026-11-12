// CThingCreatureBase::GetShotAccuracyPercentage() -- a hero with its experience component asks the component; otherwise 100 while attacking a target that passes the 0x00864CB0 check, else the creature's own ShotAccuracyPercentage
// Leaf: ?GetShotAccuracyPercentage@CThingCreatureBase@@QBEJXZ
// Name-fix 2026-11-12: Ego_r.pdb 0x0053E950 + FableWin.pdb ?GetShotAccuracyPercentage@CThingCreatureBase@@QBEJXZ (const); the world
//   accessor 0x004C7AB0 is ?PeekWorld@CThing@@IBEABVCWorld@@XZ (Ego_r.pdb, protected const; was the stand-in CThing::GetWorld()).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663B10, 132 bytes.
// Callees: CInterfaceMap::LowerBound @ 0x004365B0, CTCHeroExperience::GetShotAccuracyPercentage @ 0x006D6EA0,
//   CThing::PeekWorld() const @ 0x004C7AB0 (label "CViewBase::GetDisplayEngine" is propagated),
//   CCombatManager::IsAttacker(const CThingCreatureBase&) const @ 0x006E92C0 (landed; was the descriptive IsCreatureAttacking, agentNF25b),
//   CCombatManager::GetCombatantInfo(const CThing&) @ 0x006E9360 (landed as CUIDMapOwner_006e9360::GetValueForThing;
//   label "CSurface::GetHeight" is propagated), the `__fastcall` thing test @ 0x00864CB0 (label
//   "IsThingRunningAwayFromCombat"; it tests the thing's TC 0x36).
// Layout: retail CThing: interface bitset +0x20, CVectorMap<ETCInterfaceType, CTCBase*> +0x44, flag byte +0x6c (bit 5
//   tested here); CTCHeroExperience is TC 0x68 (bitset word +0x2c bit 8); retail CWorld+0x4c = combat manager;
//   CCombatantInfo +4 = the attacked thing; ShotAccuracyPercentage +0xe4 (PDB +0x100).
// Shape: the agent160 GetInterface model (__forceinline: bit test, key in a frame slot, LowerBound, `it == last ||
//   key < it->type` -> last), the map pointer replacing `this` in esi on that arm. Each combat-manager read is a named
//   local (`mov eax,[eax+4ch] ; push esi ; mov ecx,eax`).
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
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
    bool IsAttacker(const CThingCreatureBase& creature) const;          // 0x006E92C0 ?IsAttacker@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z (Ego_r + FableWin)
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
    unsigned char _pad54[0x18];
    unsigned char m_flags6c;                    // +0x6c
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return (T*)it->second;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
protected:
    const CWorld& PeekWorld() const;            // 0x004C7AB0
public:
};

class CThingCreatureBase : public CThing { public:
    unsigned char _pad70[0xe4 - 0x70];
    long ShotAccuracyPercentage;                // +0xe4
    long GetShotAccuracyPercentage() const;
};

long CThingCreatureBase::GetShotAccuracyPercentage() const
{
    if ((m_flags6c & 0x20) && HasTC(0x68))
        return GetTC<CTCHeroExperience>(0x68)->GetShotAccuracyPercentage();
    CCombatManager* combat = PeekWorld().PCombatManager;
    if (combat->IsAttacker(*this)) {
        CCombatManager* combat2 = PeekWorld().PCombatManager;
        if (IsThingRunningAwayFromCombat(combat2->GetCombatantInfo(*this)->Attacked))
            return 100;
    }
    return ShotAccuracyPercentage;
}