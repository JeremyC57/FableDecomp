// CTCCreatureOpinionOfHero::IsHeroOpinionEnemy() -- the hero is this creature's enemy: either flagged directly or, for a creature with interface 0xB4, when its village treats the hero as an enemy of the guards
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006827A0, length 99 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined GetInterface for type 0x23 = CTCVillageMember,
// `test byte ptr [eax+24h],8`), CTCVillageMember::GetPVillage @ 0x007EE540, CTCVillage::GetIsHeroEnemyOfGuards @ 0x00658604.
// The interface-0xB4 presence test (word 5 bit 20, `test dword ptr [eax+34h],100000h`) is an inline HasTC without lookup that
// returns the masked word (a `bool` HasTC compiles to `shr ecx,14h ; test cl,1`).
// Shape: nested ifs with two `return true` (one shared `mov al,1`) and a single `xor al,al` exit.
// Model: engine/CTCCreatureOpinionOfHero.h is the PDB layout; retail STLport lists are 4 bytes (PDB 12), so HeroOpinionEnemy
// sits at +0x342 (as in the landed SetHeroOpinionEnemy 0x006815D0 and KnowsOfDeed 0x00681AE0), hence a local model.
// Retail CThing: interface bitset +0x20, map +0x44, Dead bit +0x91.
struct CTCBaseInterface;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)((CTCBaseInterface*)it->second);
            return true;
        }
        return false;
    }
};
class CTCVillage { public:
    bool GetIsHeroEnemyOfGuards();              // 0x00658604
};
class CTCVillageMember { public:                       // interface 0x23
public:
    CTCVillage* GetPVillage() const;   // 0x007EE540
};
class CTCCreatureOpinionOfHero { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x342 - 0x08];
    bool HeroOpinionEnemy;                      // +0x342 (PDB +0x64e)
    bool IsHeroOpinionEnemy();
};

bool CTCCreatureOpinionOfHero::IsHeroOpinionEnemy()
{
    if (HeroOpinionEnemy)
        return true;
    CThing* thing = ParentThing;
    if (thing->HasTC(0xb4)) {
        CTCVillageMember* member;
        if (thing->GetInterface(&member, 0x23)) {
            CTCVillage* village = member->GetPVillage();
            if (village && village->GetIsHeroEnemyOfGuards())
                return true;
        }
    }
    return false;
}