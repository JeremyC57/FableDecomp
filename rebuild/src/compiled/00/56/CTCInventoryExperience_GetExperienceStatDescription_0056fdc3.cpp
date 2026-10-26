// CTCInventoryExperience::GetExperienceStatDescription(long stat, long level) -- the description text of a trainable stat at `level`: the maxed-out description when `level` is one past the stat's maximum level, else the level's own description from the hero stats def's StatLevels track; the empty wide string when the text id is 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056FDC3, length 185 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: std::bitset::test @ 0x00410DE0 and CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (the inlined
// CThing::GetInterface for interface 0x68 = CTCHeroExperience; the result is NOT checked, so the absent arm reads the never-written
// out pointer), CTCBase::GetDefinitionManager() @ 0x006869D0 (+0xDC its CDefPointer<CHeroStatsDef>), CTCHeroExperience::
// GetTrainableStatMaxLevel(EHeroTrainableStatType) @ 0x006D6770, CTCInventoryBase::GetInventoryText(long) @ 0x005BD273 (inherited,
// CWideString by hidden pointer), CWideString::CWideString(const CWideString&) @ 0x0099B720 on the empty wide string 0x013BCA24.
// Twin of the landed GetExperienceAbilityDescription 0x0056FE7C (same TU, the abilities component and its def instead).
// Layout: retail CHeroStatsDef::StatLevels at +0xFC (PDB +0x114: 0x3C-byte def base and five 16-byte vectors before it);
// CAbilityTrackDef is 0x50 bytes in retail (PDB 0x58) with Abilities at +0x40 and MaxedOutDescription at +0x4C;
// CAbilityTrackLevelDef is 0x48 bytes (PDB 0x4C) with Description at +0x44.
// Shape: `this` is spilled to [ebp-4] (the lookup key takes [ebp-8]) because the component (ebx), the stat (esi) and the
// def (edi) occupy the callee-saved registers; the maximum level is fetched before `level` is loaded and compared as
// `level == max + 1`; the text id is a named local tested after the register pops.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CWideString { public: void* PStringData; CWideString(const CWideString&); ~CWideString(); };   // 0x0099B720 / 0x0099B510
extern CWideString g_EmptyWideString;                   // 0x013BCA24
enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };
class CAbilityTrackLevelDef { public:                          // retail 0x48 bytes
    unsigned char _pad00[0x44];
    long Description;                                   // +0x44
};
class CAbilityTrackDef { public:                               // retail 0x50 bytes
    unsigned char _pad00[0x40];
    CAbilityTrackLevelDef* Abilities_first;             // +0x40 std::vector<CAbilityTrackLevelDef>
    CAbilityTrackLevelDef* Abilities_last;
    CAbilityTrackLevelDef* Abilities_end;
    long MaxedOutDescription;                           // +0x4c
};
class CHeroStatsDef { public:
    unsigned char _pad00[0xfc];
    CAbilityTrackDef* StatLevels_first;                 // +0xfc std::vector<CAbilityTrackDef>
    CAbilityTrackDef* StatLevels_last;
    CAbilityTrackDef* StatLevels_end;
};
class CGameDefinitionManager { public:
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                       // +0xDC CDefPointer<const CHeroStatsDef>::Object
};
class CTCHeroExperience { public:                              // interface type 0x68
public:
    long GetTrainableStatMaxLevel(EHeroTrainableStatType) const;   // 0x006D6770
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType,CTCBase*>
    struct Entry { long type; void* component; };
    Entry* first; Entry* last;
    Entry* LowerBound(const long*);                     // 0x0040F020
};
struct CPresenceBits { bool test(long kind); };         // bitset<>::test 0x00410DE0
class CThing { public:
    unsigned char _pad00[0x20]; CPresenceBits present; unsigned char _pad21[0x23]; CInterfaceMap interfaces;
    template<class T> __forceinline bool GetInterface(T** out, long kind) {
        if (present.test(kind)) {
            long key = kind; CInterfaceMap* map = &interfaces; CInterfaceMap::Entry* it = map->LowerBound(&key);
            if (it == map->last || kind < it->type) it = map->last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
};
class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

struct CTCInventoryBase : public CTCBase {
    void* __vftable; CThing* ParentThing;
    CWideString GetInventoryText(long text_id);         // 0x005BD273
};
class CTCInventoryExperience : public CTCInventoryBase { public:
    CWideString GetExperienceStatDescription(long stat, long level);
};
CWideString CTCInventoryExperience::GetExperienceStatDescription(long stat, long level)
{
    CTCHeroExperience* experience;
    ParentThing->GetInterface(&experience, 0x68);
    const CHeroStatsDef* statsDef = GetDefinitionManager().PHeroStatsDef;
    long textId;
    if (level == experience->GetTrainableStatMaxLevel((EHeroTrainableStatType)stat) + 1)
        textId = statsDef->StatLevels_first[stat].MaxedOutDescription;
    else
        textId = statsDef->StatLevels_first[stat].Abilities_first[level].Description;
    if (textId)
        return GetInventoryText(textId);
    return g_EmptyWideString;
}