// CTCHeroExperience::AddExperience(long) -- scales the amount by the pickup multiplier, books it, clamps the spendable pool, may give the experience tutorial, queues a pickup display entry and reports the stat and scoreboard
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D77C0, 407 bytes (+ int3 padding), `ret 4`.
// Callees: __ftol2 0x00BFEA70, CTCBase::GetDefinitionManager 0x006869D0 (twice: +0xDC hero-stats def, MaximumExpToSpend +0x60,
// ExpToSpendAtWhichToDisplayTutorial +0x64 = PDB - 4), CQuestManager::IsBetweenQuestStartAndFinishScreens 0x004AF290 on the
// singleton 0x013B89FC, CThingCreatureBase::IsIncapacitated 0x00661E80, CVectorMap::LowerBound 0x0040F020 (three inline
// interface lookups: CTCHero 0x29, CTCHeroStats 4, CTCHeroOnlineScoreboard 0x10D), CTCHero::GiveTutorialForCategory
// 0x006C9FA0, malloc 0x00BFEA0E (STLport list node), CTCHeroStats::AddHeroStatExperience 0x0057A8DA,
// CTCHeroOnlineScoreboard::UpdateScoreForActivity 0x0055F3B0.
// Shape: the pickup multiplier is the landed inline GetExperiencePickUpMultiplier (`+ 0.5` then ftol, clamped to >= 1);
// the spendable clamp is an upper-only GFLimit taking the def maximum by reference (`add eax,60h ; mov eax,[eax]`); the
// tutorial lookup is the checked out-parameter GetInterface, the stats and scoreboard lookups the unchecked one (the absent
// arm reads the never-written out pointer from the dead `amount` slot the key was homed in); the entry pick is one ternary
// so the component lands straight in ecx. Pickups.push_back(pair(2 s of frames, added)) is STLport's insert(end(), x)
// with the old tail read once into a local before linking.
// Model: CTCHeroExperience in its retail layout (12-byte STLport vectors, 4-byte lists): engine/CTCHeroExperience.h carries
// the Ego_r.pdb debug-STL offsets (TrainableStatLevels +0x28, PDef +0x70), so the class is modelled locally as the landed
// 0x006D64B0 / 0x006D7170 do; only the members this body touches are typed.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E
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

inline void* operator new(size_t, void* p) { return p; }

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