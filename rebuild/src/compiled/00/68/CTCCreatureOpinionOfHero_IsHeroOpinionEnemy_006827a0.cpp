// CTCCreatureOpinionOfHero::IsHeroOpinionEnemy() -- the hero is this creature's enemy: either flagged directly or, for a creature with interface 0xB4, when its village treats the hero as an enemy of the guards
// Leaf: ?IsHeroOpinionEnemy@CTCCreatureOpinionOfHero@@QAE_NXZ
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
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
    if (thing->HasTC(TCI_GUARD)) {
        const CTCVillageMember* member;
        if (thing->PeekTC(TCI_VILLAGE_MEMBER, &member)) {
            CTCVillage* village = member->GetPVillage();
            if (village && village->GetIsHeroEnemyOfGuards())
                return true;
        }
    }
    return false;
}