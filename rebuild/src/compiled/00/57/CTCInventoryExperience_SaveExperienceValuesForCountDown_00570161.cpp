// CTCInventoryExperience::SaveExperienceValuesForCountDown() -- snapshots the hero's general experience and the strength/skill/will experience for the spend count-down
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00570161, length 121 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020 (the inline CThing::GetInterface for
// interface 0x68, the hero experience component), CTCHeroExperience::GetStatExperience(long) @ 0x006D6510 (manifest
// "CTCInventoryAbilities::GetAbilityLevel", propagated label).
// Shape: no argument slot to borrow, so the GetInterface key is the one frame word [ebp-4]; the component stays in edi.
// Layout: local model. Retail CTCInventoryBase ParentThing +0x04; the four count-down values sit at +0x260..+0x26C
// (PointsGeneral..PointsWill, PDB +0x2C4..+0x2D0: the PDB's 16-byte debug containers are 12 bytes in retail).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || wanted < it->first)
                it = map->last;
            *out = (T*)((void*)it->second);
            return true;
        }
        return false;
    }
};
enum EHeroTrainableStatType { STAT_STRENGTH = 0, STAT_SKILL = 1, STAT_WILL = 2 };
class CTCHeroExperience { public:                           // interface type 0x68
    unsigned char _pad_0x00[0x14];
    long GeneralExperience;                          // +0x14
    long GetStatExperience(long stat);               // 0x006D6510
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
        PointsStrength = experience->GetStatExperience(STAT_STRENGTH);
        PointsSkill = experience->GetStatExperience(STAT_SKILL);
        PointsWill = experience->GetStatExperience(STAT_WILL);
    }
}