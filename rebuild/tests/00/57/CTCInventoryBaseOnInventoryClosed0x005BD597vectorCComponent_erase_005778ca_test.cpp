// Behaviour test for CTCInventoryExperience vtable slot 90 (0x005778CA), twin of slot 89 with the mouse-pointer rules: the
// model and body of final.cpp with the callees stubbed. Checks: (1) with undo, stat and ability purchases are given back
// and refunded as in slot 89, the maps cleared and the spending flag set; (2) the component loop deletes and erases every
// component, except the mouse pointer when NOT undoing (it is skipped and kept); (3) the screen restore, inventory close
// and the manager resets around the global list clear; (4) without undo the mouse pointer is handed to the manager and
// set to state 0x19, with undo it is not.
#include <stdio.h>

struct SRbNode {
    long     _M_color;
    SRbNode* _M_parent;
    SRbNode* _M_left;
    SRbNode* _M_right;
};
extern "C" SRbNode* _Rb_tree_increment(SRbNode* node) { return node->_M_right; }
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
struct CTCHeroExperience {
    void ModifyTrainableStatLevel(EHeroTrainableStatType stat, long delta) { g_statLevel[stat] += delta; }
    void UpgradeTrainableStat(EHeroTrainableStatType stat) { g_statUpgrades[stat]++; g_statLevel[stat]++; }
};
struct CTCHeroStats {
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
static int g_resetBOrder = 0, g_listOrder = 0, g_resetCOrder = 0, g_setPointerCalls = 0, g_setStateCalls = 0;
static void* g_setPointer = 0; static long g_state = -1;
namespace NUISystem {
struct CComponent { virtual ~CComponent() { g_componentDeletes++; } };
struct CBaseB {
    virtual void b00() {} virtual void b01() {} virtual void b02() {}
    virtual void SetState(long state) { g_setStateCalls++; g_state = state; }
};
struct CMousePointer : CComponent, CBaseB {};
struct CManager {
    virtual void m00() {} virtual void m01() {}
    virtual void SetMousePointer(CBaseB* pointer) { g_setPointerCalls++; g_setPointer = pointer; }
    virtual void m03() {} virtual void m04() {} virtual void m05() {}
    virtual void ResetA() { g_resetA++; }
    virtual void m07() {} virtual void m08() {} virtual void m09() {} virtual void m10() {} virtual void m11() {} virtual void m12() {}
    virtual void m13() {} virtual void m14() {} virtual void m15() {} virtual void m16() {} virtual void m17() {} virtual void m18() {}
    virtual void m19() {} virtual void m20() {} virtual void m21() {} virtual void m22() {} virtual void m23() {} virtual void m24() {}
    virtual void m25() {} virtual void m26() {} virtual void m27() {} virtual void m28() {}
    virtual void ResetB() { g_resetB++; g_resetBOrder = ++g_order; }
    virtual void m30() {} virtual void m31() {} virtual void m32() {} virtual void m33() {} virtual void m34() {} virtual void m35() {}
    virtual void m36() {}
    virtual void ResetC() { g_resetC++; g_resetCOrder = ++g_order; }
    static CManager* GetInstance();
};
static CManager g_manager;
CManager* CManager::GetInstance() { return &g_manager; }
}
struct CGlobalList { void clear() { g_listClears++; g_listOrder = ++g_order; } };
CGlobalList g_UIList;
static long g_screenType = -1; static int g_screenSets = 0;
struct CHeroInfoScreens { void SetScreenType(long type) { g_screenSets++; g_screenType = type; } };
static CHeroInfoScreens g_screens;
struct CPlayerGui { CHeroInfoScreens* GetHeroInfoScreens() { return &g_screens; } };
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;

static int g_erases = 0;
struct CComponentVector {
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    NUISystem::CComponent** begin() { return _M_start; }
    NUISystem::CComponent** end() { return _M_finish; }
    NUISystem::CComponent** erase(NUISystem::CComponent** position)
    {
        g_erases++;
        for (NUISystem::CComponent** s = position + 1; s != _M_finish; ++s) *(s - 1) = *s;
        --_M_finish;
        return position;
    }
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
struct CTCInventoryExperience : CTCInventoryBase {
    unsigned char   _pad18[0x140 - 0x18];
    CStatUndoMap    UndoableStatBuys;
    CAbilityUndoMap UndoableAbilityBuys;
    unsigned char   _pad158[0x1a0 - 0x158];
    bool            OpenedExperienceSpending;
    unsigned char   _pad1a1[0x220 - 0x1a1];
    long            PreviousScreenType;
    unsigned char   _pad224[0x258 - 0x224];
    NUISystem::CMousePointer* PMousePointer;
    CTCHeroExperience* GetTCHeroExperience() { return &g_experience; }
    CTCHeroStats*      GetTCHeroStats() { return &g_stats; }
    CTCHeroAbilities*  GetTCHeroAbilities() { return &g_abilities; }
    void vfunc_90(bool undo);
};

void CTCInventoryExperience::vfunc_90(bool undo)
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
    NUISystem::CComponent** it = Components.begin();
    while (it != Components.end()) {
        if (!undo && *it == PMousePointer) {
            ++it;
            continue;
        }
        delete *it;
        it = Components.erase(it);
    }
    NUISystem::CManager::GetInstance()->ResetA();
    NUISystem::CManager::GetInstance()->ResetB();
    g_UIList.clear();
    NUISystem::CManager::GetInstance()->ResetC();
    if (!undo) {
        NUISystem::CManager::GetInstance()->SetMousePointer(PMousePointer);
        PMousePointer->SetState(0x19);
    }
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

int main()
{
    long buckets[12] = { 0 };
    g_stats.ExperienceSpentOn = buckets; g_stats.ExperienceSpent = 50;
    SUndoNode<EHeroTrainableStatType> statHeader, statA;
    statA.first = (EHeroTrainableStatType)1; statA.second.Levels = 2; statA.second.General = 1; statA.second.Strength = 1; statA.second.Skill = 1; statA.second.Will = 1;
    statHeader._M_left = &statA; statA._M_right = &statHeader;
    SUndoNode<EHeroAbility> abilityHeader, abilityA, abilityB;
    abilityA.first = (EHeroAbility)4; abilityA.second.Levels = 1; abilityA.second.General = 2; abilityA.second.Strength = 2; abilityA.second.Skill = 2; abilityA.second.Will = 2;
    abilityB.first = (EHeroAbility)6; abilityB.second.Levels = 3; abilityB.second.General = 0; abilityB.second.Strength = 0; abilityB.second.Skill = 0; abilityB.second.Will = 9;
    abilityHeader._M_left = &abilityA; abilityA._M_right = &abilityB; abilityB._M_right = &abilityHeader;
    g_statLevel[1] = 5; g_abilityLevel[4] = 3; g_abilityLevel[6] = 7;

    CTCInventoryExperience inv;
    inv.UndoableStatBuys._M_header = &statHeader;
    inv.UndoableAbilityBuys._M_header = &abilityHeader;
    inv.OpenedExperienceSpending = false; inv.PreviousScreenType = 3;
    NUISystem::CMousePointer* pointer = new NUISystem::CMousePointer;
    inv.PMousePointer = pointer;
    NUISystem::CComponent* comps[3] = { new NUISystem::CComponent, pointer, new NUISystem::CComponent };
    inv.Components._M_start = comps; inv.Components._M_finish = comps + 3; inv.Components._M_end_of_storage = comps + 3;

    // (2) not undoing: the mouse pointer is skipped, the other two deleted; (4) pointer handed over and set to 0x19
    inv.vfunc_90(false);
    CHECK(g_componentDeletes == 2 && g_erases == 2 && inv.Components.end() - inv.Components.begin() == 1 && comps[0] == pointer);
    CHECK(g_setPointerCalls == 1 && g_setPointer == (NUISystem::CBaseB*)pointer && g_setStateCalls == 1 && g_state == 0x19);
    CHECK(!inv.OpenedExperienceSpending && g_statClears == 0 && g_stats.ExperienceSpent == 50);
    // (3)
    CHECK(g_screenSets == 1 && g_screenType == 3 && g_closed == 1);
    CHECK(g_resetA == 1 && g_resetB == 1 && g_listClears == 1 && g_resetC == 1 && g_resetBOrder < g_listOrder && g_listOrder < g_resetCOrder);

    // (1) undoing: refunds, maps cleared, flag set; the pointer is deleted like any component; no pointer hand-over
    inv.PreviousScreenType = 9;
    inv.vfunc_90(true);
    CHECK(g_statLevel[1] == 5 && g_statUpgrades[1] == 2 && buckets[1] == 4);
    CHECK(g_abilityLevel[4] == 3 && g_abilityUpgrades[4] == 1 && g_abilityLevel[6] == 7 && g_abilityUpgrades[6] == 3);
    CHECK(buckets[10] == 8 + 9 && g_stats.ExperienceSpent == 50 + 4 + 8 + 9);
    CHECK(g_statClears == 1 && g_abilityClears == 1 && inv.OpenedExperienceSpending);
    CHECK(g_componentDeletes == 3 && g_erases == 3 && inv.Components.end() == inv.Components.begin());
    CHECK(g_setPointerCalls == 1 && g_setStateCalls == 1 && g_screenSets == 2 && g_screenType == 9 && g_closed == 2);
    CHECK(g_resetA == 2 && g_resetB == 2 && g_listClears == 2 && g_resetC == 2);

    char line[64];
    sprintf(line, "005778ca_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}