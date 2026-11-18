// Behaviour test for CTCInventoryExperience vtable slot 89 (0x0057770A): the model and body of final.cpp with the callees
// stubbed. Checks: (1) with undo, every undoable stat purchase is dropped by its levels, re-upgraded level times and its
// four experience parts refunded into the total and the per-stat bucket; (2) every undoable ability purchase is set back
// to (current - levels), re-upgraded, refunded into the total and bucket 10; (3) both undo maps are cleared and the
// spending flag set; (4) with or without undo the previous screen is restored, the inventory closed, every component
// deleted and the vector cleared, the UI manager reset twice around the global list clear.
#include <stdio.h>

struct SRbNode {
    long     _M_color;
    SRbNode* _M_parent;
    SRbNode* _M_left;
    SRbNode* _M_right;
};
// a two-node "tree": the header's left is the first node, each node's right is the next, the header ends the walk
static SRbNode* g_headerForWalk = 0;
extern "C" SRbNode* _Rb_tree_increment(SRbNode* node) { return node->_M_right ? node->_M_right : g_headerForWalk; }
struct SLongList { void* _M_node; };
struct SExpSpendUndo {
    SLongList Generals, Strengths, Skills, Wills;
    long Levels;
    long General;
    long Strength;
    long Skill;
    long Will;
};
template <class K> struct SUndoNode : SRbNode {
    K             first;
    SExpSpendUndo second;
};
template <class K> struct CUndoIterator {
    SUndoNode<K>* _M_node;
    SUndoNode<K>* operator->() const { return _M_node; }
    bool operator!=(const CUndoIterator& o) const { return _M_node != o._M_node; }
    CUndoIterator& operator++() { _M_node = (SUndoNode<K>*)_Rb_tree_increment(_M_node); return *this; }
};
enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
static int g_statClears = 0, g_abilityClears = 0;
struct CStatUndoMap {
    SUndoNode<EHeroTrainableStatType>* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    void clear() { g_statClears++; _M_header->_M_left = _M_header; }
};
struct CAbilityUndoMap {
    SUndoNode<EHeroAbility>* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    void clear() { g_abilityClears++; _M_header->_M_left = _M_header; }
};

static long g_statLevel[8]; static int g_statUpgrades[8];
class CTCHeroExperience { public:
    void ModifyTrainableStatLevel(EHeroTrainableStatType stat, long delta) { g_statLevel[stat] += delta; }
    void UpgradeTrainableStat(EHeroTrainableStatType stat) { g_statUpgrades[stat]++; g_statLevel[stat]++; }
};
class CTCHeroStats { public:
    unsigned char _pad00[0x18];
    long* ExperienceSpentOn;
    unsigned char _pad1c[0x13c - 0x1c];
    long ExperienceSpent;
    void AddExperienceSpent(long amount) { ExperienceSpent += amount; }
};
static long g_abilityLevel[8]; static int g_abilityUpgrades[8];
struct CTCHeroAbilities {
    long GetAbilityLevel(EHeroAbility ability) { return g_abilityLevel[ability]; }
    void SetAbilityLevel(EHeroAbility ability, long level) { g_abilityLevel[ability] = level; }
    void UpgradeAbility(EHeroAbility ability) { g_abilityUpgrades[ability]++; g_abilityLevel[ability]++; }
};

static int g_componentDeletes = 0, g_resetA = 0, g_resetB = 0, g_resetC = 0, g_listClears = 0, g_order = 0;
static int g_resetBOrder = 0, g_listOrder = 0, g_resetCOrder = 0;
namespace NUISystem {
class CComponent { public: virtual ~CComponent() { g_componentDeletes++; } };
class CManager { public:
    virtual void m00() {} virtual void m01() {} virtual void m02() {} virtual void m03() {} virtual void m04() {} virtual void m05() {}
    virtual void ResetA() { g_resetA++; }
    virtual void m07() {} virtual void m08() {} virtual void m09() {} virtual void m10() {} virtual void m11() {} virtual void m12() {}
    virtual void m13() {} virtual void m14() {} virtual void m15() {} virtual void m16() {} virtual void m17() {} virtual void m18() {}
    virtual void m19() {} virtual void m20() {} virtual void m21() {} virtual void m22() {} virtual void m23() {} virtual void m24() {}
    virtual void m25() {} virtual void m26() {} virtual void m27() {} virtual void m28() {}
    virtual void ResetB() { g_resetB++; g_resetBOrder = ++g_order; }
    virtual void m30() {} virtual void m31() {} virtual void m32() {} virtual void m33() {} virtual void m34() {} virtual void m35() {}
    virtual void m36() {}
    virtual void ResetC() { g_resetC++; g_resetCOrder = ++g_order; }
    static CManager* __fastcall GetInstance();
};
static CManager g_manager;
CManager* CManager::GetInstance() { return &g_manager; }
}
struct CGlobalList { void clear() { g_listClears++; g_listOrder = ++g_order; } };
CGlobalList g_UIList;
static long g_screenType = -1; static int g_screenSets = 0;
struct CHeroInfoScreens { void SetScreenType(long type) { g_screenSets++; g_screenType = type; } };
static CHeroInfoScreens g_screens;
class CPlayerGui { public: CHeroInfoScreens* GetHeroInfoScreens() { return &g_screens; } };
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;

static int g_erases = 0;
struct CComponentVector {
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent*& operator[](unsigned long n) { return _M_start[n]; }
    NUISystem::CComponent** begin() { return _M_start; }
    NUISystem::CComponent** end() { return _M_finish; }
    NUISystem::CComponent** erase(NUISystem::CComponent** first, NUISystem::CComponent** last)
    {
        g_erases++;
        NUISystem::CComponent** d = first;
        for (NUISystem::CComponent** s = last; s != _M_finish; ++s, ++d) *d = *s;
        _M_finish = d;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

static int g_closed = 0;
struct CTCInventoryBase {
    void*            __vftable;
    void*            ParentThing;
    unsigned char    _pad08[0x0c - 0x08];
    CComponentVector Components;
    void OnInventoryClosed() { g_closed++; }
};
static CTCHeroExperience g_experience; static CTCHeroStats g_stats; static CTCHeroAbilities g_abilities;
class CTCInventoryExperience : public CTCInventoryBase { public:
    unsigned char   _pad18[0x140 - 0x18];
    CStatUndoMap    UndoableStatBuys;
    CAbilityUndoMap UndoableAbilityBuys;
    unsigned char   _pad158[0x1a0 - 0x158];
    bool            OpenedExperienceSpending;
    unsigned char   _pad1a1[0x220 - 0x1a1];
    long            PreviousScreenType;
    CTCHeroExperience* GetTCHeroExperience() { return &g_experience; }
    CTCHeroStats*      GetTCHeroStats() { return &g_stats; }
    CTCHeroAbilities*  GetTCHeroAbilities() { return &g_abilities; }
    void vfunc_89(bool undo);
};

void CTCInventoryExperience::vfunc_89(bool undo)
{
    if (undo) {
        SRbNode* node;
        for (node = UndoableStatBuys._M_header->_M_left; node != UndoableStatBuys._M_header; node = _Rb_tree_increment(node)) {
            SUndoNode<EHeroTrainableStatType>* it = (SUndoNode<EHeroTrainableStatType>*)node;
            GetTCHeroExperience()->ModifyTrainableStatLevel(it->first, -it->second.Levels);
            for (long i = 0; i != it->second.Levels; ++i)
                GetTCHeroExperience()->UpgradeTrainableStat(it->first);
            long spent = it->second.Will + it->second.Skill + it->second.Strength + it->second.General;
            GetTCHeroStats()->AddExperienceSpent(spent);
            GetTCHeroStats()->ExperienceSpentOn[it->first] += spent;
        }
        for (node = UndoableAbilityBuys._M_header->_M_left; node != UndoableAbilityBuys._M_header; node = _Rb_tree_increment(node)) {
            SUndoNode<EHeroAbility>* it = (SUndoNode<EHeroAbility>*)node;
            GetTCHeroAbilities()->SetAbilityLevel(it->first, GetTCHeroAbilities()->GetAbilityLevel(it->first) - it->second.Levels);
            for (long i = 0; i != it->second.Levels; ++i)
                GetTCHeroAbilities()->UpgradeAbility(it->first);
            long spent = it->second.Will + it->second.Skill + it->second.Strength + it->second.General;
            GetTCHeroStats()->AddExperienceSpent(spent);
            GetTCHeroStats()->ExperienceSpentOn[10] += spent;
        }
        UndoableStatBuys.clear();
        UndoableAbilityBuys.clear();
        OpenedExperienceSpending = true;
    }
    g_pPlayerGui->GetHeroInfoScreens()->SetScreenType(PreviousScreenType);
    OnInventoryClosed();
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    NUISystem::CManager::GetInstance()->ResetA();
    NUISystem::CManager::GetInstance()->ResetB();
    g_UIList.clear();
    NUISystem::CManager::GetInstance()->ResetC();
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

int main()
{
    long buckets[12] = { 0 };
    g_stats.ExperienceSpentOn = buckets; g_stats.ExperienceSpent = 100;
    // stat undo map: two nodes (stat 2: 3 levels, parts 1/2/3/4 -> 10; stat 5: 1 level, parts 5/0/0/5 -> 10)
    SUndoNode<EHeroTrainableStatType> statHeader, statA, statB;
    statA.first = (EHeroTrainableStatType)2; statA.second.Levels = 3; statA.second.General = 1; statA.second.Strength = 2; statA.second.Skill = 3; statA.second.Will = 4;
    statB.first = (EHeroTrainableStatType)5; statB.second.Levels = 1; statB.second.General = 5; statB.second.Strength = 0; statB.second.Skill = 0; statB.second.Will = 5;
    statHeader._M_left = &statA; statA._M_right = &statB; statB._M_right = 0;
    // ability undo map: one node (ability 3: 2 levels, parts 7/0/1/0 -> 8), current level 6
    SUndoNode<EHeroAbility> abilityHeader, abilityA;
    abilityA.first = (EHeroAbility)3; abilityA.second.Levels = 2; abilityA.second.General = 7; abilityA.second.Strength = 0; abilityA.second.Skill = 1; abilityA.second.Will = 0;
    abilityHeader._M_left = &abilityA; abilityA._M_right = 0;
    g_statLevel[2] = 10; g_statLevel[5] = 4; g_abilityLevel[3] = 6;

    CTCInventoryExperience inv;
    inv.UndoableStatBuys._M_header = &statHeader;
    inv.UndoableAbilityBuys._M_header = &abilityHeader;
    inv.OpenedExperienceSpending = false; inv.PreviousScreenType = 7;
    NUISystem::CComponent* comps[3] = { new NUISystem::CComponent, new NUISystem::CComponent, new NUISystem::CComponent };
    inv.Components._M_start = comps; inv.Components._M_finish = comps + 3; inv.Components._M_end_of_storage = comps + 3;

    // (1)-(3) undo pass over the stat map (statB's right is 0: the stub returns the stat header), then the ability map
    // (abilityA's right points at its own header)
    g_headerForWalk = &statHeader;
    abilityA._M_right = &abilityHeader;
    inv.vfunc_89(true);
    CHECK(g_statLevel[2] == 10 && g_statUpgrades[2] == 3);          // -3 then +3 upgrades
    CHECK(g_statLevel[5] == 4 && g_statUpgrades[5] == 1);
    CHECK(g_abilityLevel[3] == 6 && g_abilityUpgrades[3] == 2);     // set to 4 then upgraded twice
    CHECK(g_stats.ExperienceSpent == 100 + 10 + 10 + 8);
    CHECK(buckets[2] == 10 && buckets[5] == 10 && buckets[10] == 8 && buckets[3] == 0);
    CHECK(g_statClears == 1 && g_abilityClears == 1 && inv.OpenedExperienceSpending);
    // (4) screen restored, inventory closed, components deleted and cleared, manager resets around the list clear
    CHECK(g_screenSets == 1 && g_screenType == 7 && g_closed == 1);
    CHECK(g_componentDeletes == 3 && g_erases == 1 && inv.Components.size() == 0);
    CHECK(g_resetA == 1 && g_resetB == 1 && g_listClears == 1 && g_resetC == 1 && g_resetBOrder < g_listOrder && g_listOrder < g_resetCOrder);

    // without undo: no refunds, flag untouched, the rest still happens
    inv.OpenedExperienceSpending = false; inv.PreviousScreenType = 2;
    inv.vfunc_89(false);
    CHECK(!inv.OpenedExperienceSpending && g_statClears == 1 && g_stats.ExperienceSpent == 128);
    CHECK(g_screenSets == 2 && g_screenType == 2 && g_closed == 2 && g_componentDeletes == 3 && g_erases == 2);
    CHECK(g_resetA == 2 && g_resetB == 2 && g_listClears == 2 && g_resetC == 2);

    char line[64];
    sprintf(line, "0057770a_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}