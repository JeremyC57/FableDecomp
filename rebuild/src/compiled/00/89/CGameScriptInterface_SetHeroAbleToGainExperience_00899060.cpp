// CGameScriptInterface::SetHeroAbleToGainExperience(bool) const -- lets (or stops) the hero gaining experience
// Leaf: SetHeroAbleToGainExperience@CGameScriptInterface
// Retail: Fable.exe @ 0x00899060, 90 bytes. Calls CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
// then CThing::GetTC<CTCHeroExperience> (Ego_r.pdb bool/out-pointer form; Components lower_bound 0x0040F020).
// Layout: CGameScriptInterface per engine/CGameScriptInterface.h (+0x14 PlayerManager); CThing retail (UsingComponents +0x20,
// Components +0x44, Dead +0x91 bit 0).
// CTCHeroExperience retail = Ego_r.pdb minus 8 after its two STLport vectors: AbleToGainExperience +0x3c (PDB +0x44).
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
    unsigned char _pad00[0x3c];
    bool          AbleToGainExperience;                  // +0x3c
    void SetAbleToGainExperience(bool able) { AbleToGainExperience = able; }
};
class CGameScriptInterface {
public:
    virtual void SetHeroAbleToGainExperience(bool able) const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

void CGameScriptInterface::SetHeroAbleToGainExperience(bool able) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCHeroExperience* tc;
    if (hero && !hero->IsDead() && hero->GetTC(&tc))
        tc->SetAbleToGainExperience(able);
}