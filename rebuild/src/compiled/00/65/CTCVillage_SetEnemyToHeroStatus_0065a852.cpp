#pragma optimize("s", on)
// CTCVillage::SetEnemyToHeroStatus() -- through the village thing's enemy component (interface 73): add the hero as an enemy while the village stance is hostile (1) and the hero is alive, else remove the hero from its enemies
// Leaf: SetEnemyToHeroStatus
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad24[0x20] removed.
// Naming wave 18: the interface-presence stand-in CBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe @ 0x0065A852, 156 bytes; size TU (unaligned entry): `optimize("s",on)`.
// Callees: std::bitset<>::test(size_t) const 0x00410DE0 (interface 73); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//          0x0040F020 (inlined GetInterface); CTCBase::GetPlayerManager() 0x00686D40 (x2); CPlayerManager::GetMainPlayer()
//          0x00449970 (x2); CPlayer::GetPControlledCreature() 0x00487DC0 (x2); IS_THING_ALIVE<CThing*>(CThing* const&) 0x00436380
//          (__fastcall); CTCEnemy::AddEnemyThing(CThing*) 0x0076D1B0 (manifest "CCreature_AddEnemyToList" is a gap-scan
//          name), CTCEnemy::RemoveEnemyThing(CThing*) 0x0076CC00.
// Layout: retail CTCVillage StanceToHero +0x98 (PDB +0x124, minus 0x8c); CTCBase::ParentThing +0x04.
// Shape: the out-pointer GetInterface with its result ignored: the component local is UNinitialised and shares the
//        [ebp-4] slot with the lookup key (the false arm loads it as is) and later with the hero pointer whose address
//        IS_THING_ALIVE takes.
class CTCBase;
class CThing;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274>       m_present;                    // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_present.test(interfaceType)) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
bool __fastcall IS_THING_ALIVE_00436380(CThing* const& thing);   // 0x00436380

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCEnemy { public:
    void AddEnemyThing(CThing* enemy);          // 0x0076D1B0
    void RemoveEnemyThing(CThing* enemy);       // 0x0076CC00
};

class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCVillage : public CTCBase { public:
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad_0x08[0x90];
    long          StanceToHero;                 // +0x98 EVillageStanceToHero
    void SetEnemyToHeroStatus();
};

void CTCVillage::SetEnemyToHeroStatus()
{
    CTCEnemy* enemy;
    ParentThing->GetInterface(&enemy, 73);
    if (StanceToHero == 1) {
        CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE_00436380(hero))
            enemy->AddEnemyThing(hero);
    } else {
        enemy->RemoveEnemyThing(GetPlayerManager().GetMainPlayer().GetPControlledCreature());
    }
}