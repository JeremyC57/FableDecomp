// behaviour test for 0x00899680 CGameScriptInterface::SetHeroAsApprentice
#include <stdio.h>
#include <string.h>
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
enum EMorality { MORALITY_NEUTRAL = 3, MORALITY_FORCE_DWORD = 0x7fffffff };
enum EHeroTrainingStatus { HERO_TRAINING_STATUS_NONE = 0, HERO_TRAINING_STATUS_APPRENTICE = 1, HERO_TRAINING_STATUS_FORCE_DWORD = 0x7fffffff };
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
    unsigned char _pad_0x060[0x8];
    bool ScriptOverridePreventStaminaRefill;    // +0x068
    unsigned char _pad_0x069[0x7];
    long RenownLevel;                           // +0x070
    long RenownMaxLevel;                        // +0x074
    unsigned char _pad_0x078[0x24];
    float Attractiveness;                       // +0x09c
    float Scariness;                            // +0x0a0
    unsigned char _pad_0x0a4[0x70];
    EHeroTrainingStatus TrainingStatus;         // +0x114
    unsigned char _pad_0x118[0x58];
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
    void  UpgradeRenownLevel();                                    // 0x0057CAE9
    void  AddRenown(long renown);                                  // 0x0057D8BE
    void  SetAge(float age);                                       // 0x0057B05F
    void  ForceStaminaTo(float stamina);                           // 0x0057B26F
    void  ResetTrainingStatus();                                   // 0x0057ADB8
    float GetMoralityPercentage() const;                           // 0x0057A690
    EMorality GetMoralityCategory() const;                         // 0x0057A6B8
};
class CGameScriptInterface {
public:
    virtual void SetHeroAsApprentice(bool apprentice) const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

void CGameScriptInterface::SetHeroAsApprentice(bool apprentice) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPPlayerCharacter();
    if (hero && !hero->IsDead())
    {
        CTCHeroStats* tc;
        hero->GetTC(&tc);
        if (apprentice)
            tc->TrainingStatus = HERO_TRAINING_STATUS_APPRENTICE;
        else
            tc->ResetTrainingStatus();
    }
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
static int g_calls; static long g_larg; static float g_farg;
void  CTCHeroStats::UpgradeRenownLevel() { ++g_calls; }
void  CTCHeroStats::AddRenown(long renown) { ++g_calls; g_larg = renown; }
void  CTCHeroStats::SetAge(float age) { ++g_calls; g_farg = age; }
void  CTCHeroStats::ForceStaminaTo(float stamina) { ++g_calls; g_farg = stamina; }
void  CTCHeroStats::ResetTrainingStatus() { ++g_calls; TrainingStatus = HERO_TRAINING_STATUS_NONE; }
float CTCHeroStats::GetMoralityPercentage() const { return 0.75f; }
EMorality CTCHeroStats::GetMoralityCategory() const { return (EMorality)5; }

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

    ok &= ((p->SetHeroAsApprentice(true), tc->TrainingStatus == HERO_TRAINING_STATUS_APPRENTICE && g_calls == 0) && (p->SetHeroAsApprentice(false), tc->TrainingStatus == HERO_TRAINING_STATUS_NONE && g_calls == 1));
    g_hero = 0;
    ok &= ((tc->TrainingStatus = HERO_TRAINING_STATUS_NONE, p->SetHeroAsApprentice(true), tc->TrainingStatus == HERO_TRAINING_STATUS_NONE));
    g_hero = hero; hero->Dead = 1;
    ok &= ((tc->TrainingStatus = HERO_TRAINING_STATUS_NONE, p->SetHeroAsApprentice(true), tc->TrainingStatus == HERO_TRAINING_STATUS_NONE));

    printf(ok ? "00899680_TEST PASS\n" : "00899680_TEST FAIL\n");
    return ok ? 0 : 1;
}