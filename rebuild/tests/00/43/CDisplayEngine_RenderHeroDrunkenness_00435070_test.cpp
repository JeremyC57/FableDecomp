#include <cstdio>
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
        *out = (T*)((it == map->last || T::INTERFACE_TYPE < it->type) ? map->last : it)->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        *out = (const T*)((it == map->last || T::INTERFACE_TYPE < it->type) ? map->last : it)->component;
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
class CIEngine;
class CInterpolationInfo;
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
    void RenderHeroDrunkenness(CIEngine* engine);                    // 0x0057B43F
};
class CMainGameComponent {
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                      // +0x1c
    CPlayerManager* GetPlayerManager() const { return PPlayerManager; }
};
extern CMainGameComponent* g_pMainGameComponent;         // 0x013B86A0
class CDisplayEngine {
public:
    unsigned char _pad00[0x2c];
    CIEngine*     Engine3D;                              // +0x2c
    void RenderHeroDrunkenness(const CInterpolationInfo& info) const;
};

void CDisplayEngine::RenderHeroDrunkenness(const CInterpolationInfo& info) const
{
    CThingPlayerCreature* hero = g_pMainGameComponent->GetPlayerManager()->GetMainPlayer().GetPPlayerCharacter();
    CTCHeroStats* stats;
    if (hero && hero->GetTC(&stats))
    {
        if (stats)
            stats->RenderHeroDrunkenness(Engine3D);
    }
}

static CInterfaceMap::SEntry g_entries[3];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
static CThingPlayerCreature g_hero; static CThingPlayerCreature* g_heroPtr;
CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return g_heroPtr; }
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
static unsigned char g_mgcRaw[sizeof(CMainGameComponent)];
CMainGameComponent* g_pMainGameComponent = (CMainGameComponent*)g_mgcRaw;
static CTCHeroStats* g_calledOn; static CIEngine* g_engine; static int g_calls;
void CTCHeroStats::RenderHeroDrunkenness(CIEngine* e) { g_calledOn = this; g_engine = e; ++g_calls; }
int main()
{
    static unsigned char statsRaw[sizeof(CTCHeroStats)]; CTCHeroStats* stats = (CTCHeroStats*)statsRaw;
    CPlayerManager pm; g_pMainGameComponent->PPlayerManager = &pm;
    for (int i = 0; i < 9; ++i) g_hero.UsingComponents[i] = 0;
    g_hero.UsingComponents[0] = 1u << 4;
    g_entries[0].type = 2; g_entries[0].component = 0;
    g_entries[1].type = 4; g_entries[1].component = (CTCBase*)stats;
    g_hero.Components.first = g_entries; g_hero.Components.last = g_entries + 2;
    static unsigned char deRaw[sizeof(CDisplayEngine)]; CDisplayEngine* de = (CDisplayEngine*)deRaw; de->Engine3D = (CIEngine*)0x5150;
    const CInterpolationInfo& info = *(const CInterpolationInfo*)deRaw;
    g_heroPtr = &g_hero; de->RenderHeroDrunkenness(info);
    bool ok = g_calls == 1 && g_calledOn == stats && g_engine == (CIEngine*)0x5150;
    g_entries[1].component = 0; de->RenderHeroDrunkenness(info); ok = ok && g_calls == 1; g_entries[1].component = (CTCBase*)stats;
    g_hero.UsingComponents[0] = 0; de->RenderHeroDrunkenness(info); ok = ok && g_calls == 1; g_hero.UsingComponents[0] = 1u << 4;
    g_heroPtr = 0; de->RenderHeroDrunkenness(info); ok = ok && g_calls == 1;
    g_heroPtr = &g_hero; de->RenderHeroDrunkenness(info); ok = ok && g_calls == 2;
    std::printf(ok ? "00435070_TEST PASS\n" : "00435070_TEST FAIL\n");
    return ok ? 0 : 1;
}