// CGameScriptInterface::GetHeroStatLevel(EHeroTrainableStatType) const -- the hero's level in a trainable stat; -1 when there is no live hero
// Leaf: GetHeroStatLevel@CGameScriptInterface
// Retail: Fable.exe @ 0x00899290, 122 bytes. Calls CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0,
// then CThing::PeekTC (Ego_r.pdb bool/out-pointer form; const Components lower_bound 0x004365B0); CTCHeroExperience::GetTrainableStatLevel 0x006D6520 (catalogued as CTCInventoryAbilities::GetAbilityLevel).
// Shape: PeekTC's result is not tested -- without the TC bit retail reads through the uninitialised out pointer.
// Layout: CGameScriptInterface per engine/CGameScriptInterface.h (+0x14 PlayerManager); CThing retail (UsingComponents +0x20,
// Components +0x44, Dead +0x91 bit 0).
// CTCHeroExperience (Ego_r.pdb, interface 0x68): ExperiencePointsAvailableToSpend +0x14 (no STL member precedes it).
class CTCBase;
class CInterfaceMap {                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const long* type);          // 0x0040F020
    const SEntry* LowerBound(const long* type) const;    // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (T*)it->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (const T*)it->component;
        return true;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;       // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;      // 0x00487DD0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                                       // 0x00449970
};
enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_STRENGTH = 0, HERO_EXPERIENCE_STAT_SKILL = 1, HERO_EXPERIENCE_STAT_WILL = 2,
                                   HERO_EXPERIENCE_STAT_FORCE_DWORD = 0x7fffffff };
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_FORCE_DWORD = 0x7fffffff };
class CTCHeroExperience {
public:
    enum { INTERFACE_TYPE = 0x68 };
    unsigned char _pad00[0x14];
    long          ExperiencePointsAvailableToSpend;      // +0x14
    long GetExperiencePointsAvailableToSpend() const { return ExperiencePointsAvailableToSpend; }
    long GetRoughExperienceLevel() const;                                         // 0x006D64B0
    long GetTotalStatLevel(EHeroExperienceStatCategory category) const;          // 0x006D6660
    long GetTrainableStatLevel(EHeroTrainableStatType stat) const;               // 0x006D6520
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat) const;            // 0x006D6770
};
class CGameScriptInterface {
public:
    virtual long GetHeroStatLevel(EHeroTrainableStatType stat) const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

long CGameScriptInterface::GetHeroStatLevel(EHeroTrainableStatType stat) const
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (hero && !hero->IsDead())
    {
        const CTCHeroExperience* tc;
        hero->PeekTC(&tc);
        return tc->GetTrainableStatLevel(stat);
    }
    return -1;
}