// CGameScriptInterface::SetAbilityAvailability(EHeroAbility, bool) const -- makes a hero special ability available or not
// Leaf: SetAbilityAvailability@CGameScriptInterface
// Retail: Fable.exe @ 0x0089D7C0, 99 bytes. Calls CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPPlayerCharacter 0x00487DC0,
// then CThing::GetTC<CTCSpecialAbilities> (Ego_r.pdb bool/out-pointer form; Components lower_bound 0x0040F020).
// Layout: CGameScriptInterface per engine/CGameScriptInterface.h (+0x14 PlayerManager); CThing retail (UsingComponents +0x20,
// Components +0x44, Dead +0x91 bit 0).
// CTCSpecialAbilities (interface 0xa7) retail: AbilityAvailability +0x1c (Ego_r.pdb +0x24 after a 12-byte list; retail list 4 bytes).
// Retail stores a byte per ability, so the vector is modelled over bool elements.
struct CTCBase;
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
    CThingPlayerCreature*       GetPPlayerCharacter() const;       // 0x00487DC0
    const CThingPlayerCreature* PeekPPlayerCharacter() const;      // 0x00487DD0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                                       // 0x00449970
};
enum EHeroAbility { HERO_ABILITY_FORCE_DWORD = 0x7fffffff };
template <class T> struct CRetailVector {                // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T& operator[](unsigned long n) { return _M_start[n]; }
};
class CTCSpecialAbilities {
public:
    enum { INTERFACE_TYPE = 0xa7 };
    unsigned char       _pad00[0x1c];
    CRetailVector<bool> AbilityAvailability;             // +0x1c
    void SetAbilityAvailable(EHeroAbility ability, bool available) { AbilityAvailability[ability] = available; }
};
class CGameScriptInterface {
public:
    virtual void SetAbilityAvailability(EHeroAbility ability, bool available) const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

void CGameScriptInterface::SetAbilityAvailability(EHeroAbility ability, bool available) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPPlayerCharacter();
    CTCSpecialAbilities* tc;
    if (hero && !hero->IsDead() && hero->GetTC(&tc))
        tc->SetAbilityAvailable(ability, available);
}