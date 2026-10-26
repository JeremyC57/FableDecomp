// behaviour test for 0x0089A150 CGameScriptInterface::GetHeroWillEnergy
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
class CCharStringData;
class CCharString { public: CCharStringData* PStringData; };
struct CMarriageNode { CMarriageNode* _M_next; CMarriageNode* _M_prev; };
class CMarriageList {                                    // std::list<CHeroMarriageInfo> (STLport: one node pointer)
public:
    CMarriageNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
enum EMarriageState { MARRIAGE_STATE_1 = 1, MARRIAGE_STATE_2 = 2, MARRIAGE_STATE_HAS_CHILDREN = 4, MARRIAGE_STATE_DIVORCED = 5,
                      MARRIAGE_STATE_WIFE_MURDERED = 7, MARRIAGE_STATE_FORCE_DWORD = 0x7fffffff };
class CTCHeroStats {
public:
    enum { INTERFACE_TYPE = 0x4 };
    unsigned char _pad_0x000[0x30];
    float Age;                                  // +0x030
    float SunTan;                               // +0x034
    float Fatness;                              // +0x038
    unsigned char _pad_0x03c[0x1c];
    long Stamina;                               // +0x058
    long MaxStamina;                            // +0x05c
    unsigned char _pad_0x060[0x10];
    long RenownLevel;                           // +0x070
    long RenownMaxLevel;                        // +0x074
    unsigned char _pad_0x078[0x24];
    float Attractiveness;                       // +0x09c
    float Scariness;                            // +0x0a0
    unsigned char _pad_0x0a4[0xcc];
    CCharString DeathRecoveryMarkerName;        // +0x170
    unsigned char _pad_0x174[0x4];
    float BestTimePairs;                        // +0x178
    float BestTimeSorting;                      // +0x17c
    unsigned char _pad_0x180[0x10];
    float BestTimeGuessTheAddition;             // +0x190
    unsigned char _pad_0x194[0x44];
    CMarriageList Marriages;                    // +0x1d8
    float GetAge() const { return Age; }
    float GetFatness() const { return Fatness; }
    long  GetStamina() const { return Stamina; }
    long  GetMaxStamina() const { return MaxStamina; }
    float GetStaminaPercentage() const { return (float)Stamina / MaxStamina; }
    long  GetRenownLevel() const { return RenownLevel; }
    float GetAttractiveness() const { return Attractiveness; }
    float GetScariness() const { return Scariness; }
    float GetBestTimePairs() const { return BestTimePairs; }
    float GetBestTimeSorting() const { return BestTimeSorting; }
    float GetBestTimeGuessTheAddition() const { return BestTimeGuessTheAddition; }
    const CCharString& GetDeathRecoveryMarkerName() const { return DeathRecoveryMarkerName; }
    bool  HasMarriageInState(EMarriageState state) const;          // 0x0057AC29
    bool  HasCurrentMarriage() const;                              // 0x0057AC4B
    float GetPercentageThroughRenownLevel() const;                 // 0x0057AAD9
};
class CGameScriptInterface {
public:
    virtual long GetHeroWillEnergy() const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

long CGameScriptInterface::GetHeroWillEnergy() const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPPlayerCharacter();
    const CTCHeroStats* stats;
    if (hero && !hero->IsDead() && hero->PeekTC(&stats))
        return stats->GetStamina();
    return 0;
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
static int g_state_seen;
bool CTCHeroStats::HasMarriageInState(EMarriageState state) const { g_state_seen = state; return state == MARRIAGE_STATE_DIVORCED || state == MARRIAGE_STATE_HAS_CHILDREN || state == MARRIAGE_STATE_WIFE_MURDERED; }
bool CTCHeroStats::HasCurrentMarriage() const { return Age == 21.5f; }
float CTCHeroStats::GetPercentageThroughRenownLevel() const { return (float)RenownLevel / 10.0f; }
static CMarriageNode g_node, g_node2;

static unsigned char g_tcmem[0x1000];
static unsigned char g_thingmem[0x200];
static CInterfaceMap::SEntry g_entries[3];
int main()
{
    int ok = 1;
    CPlayerManager pm; CGameScriptInterface gsi; gsi.PlayerManager = &pm;
    const CGameScriptInterface* p = &gsi;
    CTCHeroStats* tc = (CTCHeroStats*)g_tcmem;
    CThingPlayerCreature* hero = (CThingPlayerCreature*)g_thingmem;
    g_entries[0].type = 1; g_entries[0].component = 0;
    g_entries[1].type = CTCHeroStats::INTERFACE_TYPE; g_entries[1].component = (CTCBase*)tc;
    g_entries[2].type = 0x100; g_entries[2].component = 0;
    hero->Components.first = g_entries; hero->Components.last = g_entries + 3;
    hero->UsingComponents[CTCHeroStats::INTERFACE_TYPE >> 5] |= 1UL << (CTCHeroStats::INTERFACE_TYPE & 31);
    g_hero = hero;
    tc->Age = 21.5f; tc->Stamina = 30; tc->MaxStamina = 120; tc->RenownLevel = 7; tc->Attractiveness = 0.25f; tc->Scariness = 0.75f;
    tc->BestTimePairs = 11.0f; tc->BestTimeSorting = 12.0f; tc->BestTimeGuessTheAddition = 13.0f;
    g_node._M_next = &g_node2; g_node2._M_next = &g_node; tc->Marriages._M_node = &g_node;
    ok &= (p->GetHeroWillEnergy() == 30);
    g_hero = 0;
    ok &= (p->GetHeroWillEnergy() == 0);
    g_hero = hero; hero->Dead = 1;
    ok &= (p->GetHeroWillEnergy() == 0);
    hero->Dead = 0; hero->UsingComponents[CTCHeroStats::INTERFACE_TYPE >> 5] = 0;
    ok &= (p->GetHeroWillEnergy() == 0);
    printf(ok ? "0089a150_TEST PASS\n" : "0089a150_TEST FAIL\n");
    return ok ? 0 : 1;
}