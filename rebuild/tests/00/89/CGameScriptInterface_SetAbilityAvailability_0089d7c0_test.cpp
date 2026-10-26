// behaviour test for 0x0089D7C0 CGameScriptInterface::SetAbilityAvailability
#include <stdio.h>
#include <string.h>
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

static CThingPlayerCreature* g_hero;
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return g_hero; }
const CThingPlayerCreature* CPlayer::PeekPPlayerCharacter() const { return g_hero; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}

static unsigned char g_tcmem[0x1000];
static unsigned char g_thingmem[0x200];
static CInterfaceMap::SEntry g_entries[3];
int main()
{
    int ok = 1;
    CPlayerManager pm; CGameScriptInterface gsi; gsi.PlayerManager = &pm;
    const CGameScriptInterface* p = &gsi;
    CTCSpecialAbilities* tc = (CTCSpecialAbilities*)g_tcmem;
    CThingPlayerCreature* hero = (CThingPlayerCreature*)g_thingmem;
    g_entries[0].type = 1; g_entries[0].component = 0;
    g_entries[1].type = CTCSpecialAbilities::INTERFACE_TYPE; g_entries[1].component = (CTCBase*)tc;
    g_entries[2].type = 0x100; g_entries[2].component = 0;
    hero->Components.first = g_entries; hero->Components.last = g_entries + 3;
    hero->UsingComponents[CTCSpecialAbilities::INTERFACE_TYPE >> 5] |= 1UL << (CTCSpecialAbilities::INTERFACE_TYPE & 31);
    g_hero = hero;
    static bool flags[8]; tc->AbilityAvailability._M_start = flags; tc->AbilityAvailability._M_finish = flags + 8;
    ok &= ((p->SetAbilityAvailability((EHeroAbility)3, true), tc->AbilityAvailability._M_start[3] == true && tc->AbilityAvailability._M_start[2] == false) && (p->SetAbilityAvailability((EHeroAbility)3, false), tc->AbilityAvailability._M_start[3] == false));
    g_hero = 0;
    ok &= ((p->SetAbilityAvailability((EHeroAbility)5, true), tc->AbilityAvailability._M_start[5] == false));
    g_hero = hero; hero->Dead = 1;
    ok &= ((p->SetAbilityAvailability((EHeroAbility)5, true), tc->AbilityAvailability._M_start[5] == false));
    hero->Dead = 0; hero->UsingComponents[CTCSpecialAbilities::INTERFACE_TYPE >> 5] = 0;
    ok &= ((p->SetAbilityAvailability((EHeroAbility)5, true), tc->AbilityAvailability._M_start[5] == false));
    printf(ok ? "0089d7c0_TEST PASS\n" : "0089d7c0_TEST FAIL\n");
    return ok ? 0 : 1;
}