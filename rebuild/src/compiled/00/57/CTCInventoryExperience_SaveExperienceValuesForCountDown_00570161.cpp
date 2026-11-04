// CTCInventoryExperience::SaveExperienceValuesForCountDown() -- snapshots the hero's general experience and the strength/skill/will experience for the spend count-down
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00570161, length 121 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020 (the inline CThing::GetInterface for
// interface 0x68, the hero experience component), CTCHeroExperience::GetStatExperience(EHeroExperienceStatCategory) const @ 0x006D6510 (manifest
// "CTCInventoryAbilities::GetAbilityLevel", propagated label).
// Shape: no argument slot to borrow, so the GetInterface key is the one frame word [ebp-4]; the component stays in edi.
// Layout: local model. Retail CTCInventoryBase ParentThing +0x04; the four count-down values sit at +0x260..+0x26C
// (PointsGeneral..PointsWill, PDB +0x2C4..+0x2D0: the PDB's 16-byte debug containers are 12 bytes in retail).
#pragma optimize("y", off)
#pragma optimize("s", on)

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || wanted < it->first)
                it = map->end();
            *out = (T*)((void*)it->second);
            return true;
        }
        return false;
    }
};
enum EHeroExperienceStatCategory { HERO_STAT_STRENGTH = 0, HERO_STAT_SKILL = 1, HERO_STAT_WILL = 2 };
class CTCHeroExperience { public:                           // interface type 0x68
    unsigned char _pad_0x00[0x14];
    long GeneralExperience;                          // +0x14
    long GetStatExperience(EHeroExperienceStatCategory category) const;   // 0x006D6510 Ego_r/FableWin ?GetStatExperience@CTCHeroExperience@@QBEJW4EHeroExperienceStatCategory@@@Z
};

class CTCInventoryExperience { public:
    void*  __vftable;                                // +0x00
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x08[0x258];
    long   PointsGeneral;                            // +0x260
    long   PointsStrength;                           // +0x264
    long   PointsSkill;                              // +0x268
    long   PointsWill;                               // +0x26C
    void SaveExperienceValuesForCountDown();
};

void CTCInventoryExperience::SaveExperienceValuesForCountDown()
{
    CTCHeroExperience* experience;
    if (ParentThing->GetInterface(&experience, 0x68)) {
        PointsGeneral = experience->GeneralExperience;
        PointsStrength = experience->GetStatExperience(HERO_STAT_STRENGTH);
        PointsSkill = experience->GetStatExperience(HERO_STAT_SKILL);
        PointsWill = experience->GetStatExperience(HERO_STAT_WILL);
    }
}