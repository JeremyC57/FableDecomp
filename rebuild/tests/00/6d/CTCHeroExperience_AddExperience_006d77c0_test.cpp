#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#define __fastcall
#define __stdcall
#endif
#include <stddef.h>
#include <cstdio>
#include <cstdlib>
extern long g_FramesPerSecond;                      // 0x01375550

template <class T> __forceinline void GFLimitHigh(T& value, const T& hi)
{
    if (value > hi)
        value = hi;
}

struct CTCBaseComponent;

struct CInterfaceMap {
    struct SEntry { long type; CTCBaseComponent* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    bool IsIncapacitated();                     // 0x00661E80
};

struct CQuestManager { bool IsBetweenQuestStartAndFinishScreens() const; };   // 0x004AF290
extern CQuestManager* g_pQuestManager;          // 0x013B89FC

struct CTCHero { bool GiveTutorialForCategory(long category, bool force); };   // 0x006C9FA0
struct CTCHeroStats { void AddHeroStatExperience(long category, long amount); };   // 0x0057A8DA
struct CTCHeroOnlineScoreboard { void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter); };   // 0x0055F3B0

struct CHeroStatsDef {
    unsigned char _pad00[0x60];
    long MaximumExpToSpend;                     // +0x60
    long ExpToSpendAtWhichToDisplayTutorial;    // +0x64
};

struct CDefinitionManager {
    unsigned char  _pad00[0xdc];
    CHeroStatsDef* PHeroStatsDef;               // +0xdc
};

struct CHeroExperienceDef {
    unsigned char _base[0x64];
    float ExperienceMultiplierParamPerUnitComboMultiplier;  // +0x64
};

struct CListNodeBase {
    CListNodeBase* _M_next;
    CListNodeBase* _M_prev;
};
template <class T> struct CListNode : CListNodeBase { T _M_data; };

#include <new>

template <class T> struct CRetailList {         // STLport list<T>: the header-node pointer only (4 bytes)
    CListNode<T>* _M_node;
    struct iterator {
        CListNodeBase* _M_node;
        iterator(CListNodeBase* n) : _M_node(n) { }
        iterator(const iterator& o) : _M_node(o._M_node) { }
    };
    iterator end() { return iterator(_M_node); }
    CListNode<T>* _M_create_node(const T& x)
    {
        CListNode<T>* p = (CListNode<T>*)malloc(sizeof(CListNode<T>));
        new (&p->_M_data) T(x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        CListNodeBase* tmp = _M_create_node(x);
        CListNodeBase* prev = position._M_node->_M_prev;
        tmp->_M_next = position._M_node;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        position._M_node->_M_prev = tmp;
        return tmp;
    }
    void push_back(const T& x) { insert(end(), x); }
};

struct SPickup {                                // pair<long,long>
    long first;
    long second;
    SPickup(long a, long b) : first(a), second(b) { }
    SPickup(const SPickup& o) : first(o.first), second(o.second) { }
};

class CTCHeroExperience {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long FishingLevel;                                      // +0x0c
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    long* StatExperiencePoints[3];                          // +0x18
    long* TrainableStatLevels[3];                           // +0x24
    float ExperienceMultiplierParam;                        // +0x30
    float LastExperienceMultiplierParam;                    // +0x34
    long ExperienceAttenuationTimer;                        // +0x38
    bool AbleToGainExperience;                              // +0x3c
    CRetailList<SPickup> Pickups;                           // +0x40
    long* ExperienceSpentOnStats[3];                        // +0x44
    void* AttackHistory;                                    // +0x50
    CHeroExperienceDef* PDef;                               // +0x54

    CDefinitionManager* GetDefinitionManager() const;       // 0x006869D0 (CTCBase)
    long GetExperiencePickupMultiplierForParam(float param) const
    {
        long multiplier = (long)(param / PDef->ExperienceMultiplierParamPerUnitComboMultiplier + 0.5);
        if (multiplier < 1)
            multiplier = 1;
        return multiplier;
    }
    long GetExperiencePickUpMultiplier() const { return GetExperiencePickupMultiplierForParam(ExperienceMultiplierParam); }
    void AddExperience(long amount);
};

void CTCHeroExperience::AddExperience(long amount)
{
    if (!AbleToGainExperience)
        return;
    long added = GetExperiencePickUpMultiplier() * amount;
    TotalExperiencePoints += added;
    ExperiencePointsAvailableToSpend += added;
    GFLimitHigh(ExperiencePointsAvailableToSpend, GetDefinitionManager()->PHeroStatsDef->MaximumExpToSpend);
    if (ExperiencePointsAvailableToSpend > GetDefinitionManager()->PHeroStatsDef->ExpToSpendAtWhichToDisplayTutorial
        && !g_pQuestManager->IsBetweenQuestStartAndFinishScreens()
        && !ParentThing->IsIncapacitated()) {
        CTCHero* hero;
        if (ParentThing->GetInterface(&hero, 0x29))
            hero->GiveTutorialForCategory(0x18, false);
    }
    Pickups.push_back(SPickup((long)(g_FramesPerSecond * 2.0f), added));
    CTCHeroStats* stats;
    ParentThing->GetInterface(&stats, 4);
    stats->AddHeroStatExperience(3, added);
    CTCHeroOnlineScoreboard* scoreboard;
    ParentThing->GetInterface(&scoreboard, 0x10d);
    scoreboard->UpdateScoreForActivity(3, (float)TotalExperiencePoints, false);
}

long g_FramesPerSecond = 30;
static CInterfaceMap::SEntry* g_lbFirst;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static bool g_incap = false, g_between = false;
bool CThing::IsIncapacitated() { return g_incap; }
bool CQuestManager::IsBetweenQuestStartAndFinishScreens() const { return g_between; }
CQuestManager g_questManager;
CQuestManager* g_pQuestManager = &g_questManager;
static long g_tutCat = -1, g_statCat = -1, g_statAmt = -1, g_scoreAct = -1; static float g_score = -1; static int g_tutForce = -1;
bool CTCHero::GiveTutorialForCategory(long category, bool force) { g_tutCat = category; g_tutForce = force ? 1 : 0; return true; }
void CTCHeroStats::AddHeroStatExperience(long category, long amount) { g_statCat = category; g_statAmt = amount; }
void CTCHeroOnlineScoreboard::UpdateScoreForActivity(long activity, float score, bool) { g_scoreAct = activity; g_score = score; }
static CDefinitionManager g_defs;
CDefinitionManager* CTCHeroExperience::GetDefinitionManager() const { return &g_defs; }
void CTCHeroExperience::v00() { }

int main()
{
    static CHeroStatsDef statsDef; statsDef.MaximumExpToSpend = 100; statsDef.ExpToSpendAtWhichToDisplayTutorial = 50;
    g_defs.PHeroStatsDef = &statsDef;
    static CHeroExperienceDef expDef; expDef.ExperienceMultiplierParamPerUnitComboMultiplier = 0.5f;
    static CTCHero hero; static CTCHeroStats stats; static CTCHeroOnlineScoreboard board;
    static CThing thing;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 4; entries[0].component = (CTCBaseComponent*)&stats;
    entries[1].type = 0x29; entries[1].component = (CTCBaseComponent*)&hero;
    entries[2].type = 0x10d; entries[2].component = (CTCBaseComponent*)&board;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    thing.m_presentBits[0] = 1UL << 4; thing.m_presentBits[1] = 1UL << 9; thing.m_presentBits[8] = 1UL << 13;
    CListNode<SPickup>& head = *(CListNode<SPickup>*)calloc(1, sizeof(CListNode<SPickup>)); head._M_next = &head; head._M_prev = &head;
    CTCHeroExperience* e = (CTCHeroExperience*)calloc(1, sizeof(CTCHeroExperience));
    e->ParentThing = &thing; e->PDef = &expDef; e->Pickups._M_node = &head;
    e->ExperienceMultiplierParam = 1.0f; e->TotalExperiencePoints = 5; e->ExperiencePointsAvailableToSpend = 90;
    int ok = 1;
    e->AbleToGainExperience = false;
    e->AddExperience(10);
    if (e->TotalExperiencePoints != 5 || head._M_next != &head) ok = 0;
    e->AbleToGainExperience = true;
    e->AddExperience(10);
    if (e->TotalExperiencePoints != 25 || e->ExperiencePointsAvailableToSpend != 100) ok = 0;
    if (g_tutCat != 0x18 || g_tutForce != 0) ok = 0;
    CListNode<SPickup>* n = (CListNode<SPickup>*)head._M_next;
    if (n == &head || n->_M_next != &head || head._M_prev != n || n->_M_data.first != 60 || n->_M_data.second != 20) ok = 0;
    if (g_statCat != 3 || g_statAmt != 20 || g_scoreAct != 3 || g_score != 25.0f) ok = 0;
    g_tutCat = -1; g_between = true; e->ExperienceMultiplierParam = 0.0f;   // multiplier clamps to 1
    e->AddExperience(3);
    if (g_tutCat != -1 || e->TotalExperiencePoints != 28 || e->ExperiencePointsAvailableToSpend != 100 || g_statAmt != 3) ok = 0;
    if (!ok) { std::printf("006d77c0_TEST FAIL\n"); return 1; }
    std::printf("006d77c0_TEST PASS\n");
    return 0;
}