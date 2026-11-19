// Behaviour test for 006668a0: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CThingCreatureBase::GetPossibleCombatAnimations(const CCharString& name, std::vector<const CAnimationEntry*>& anim_list) const -- collect the combat-animation-set entries named `name` into the current mode's filter / weapon filter / no-group buckets, copy the first non-empty bucket into anim_list, else fall back to the "DEFAULT" animation and return false
#include <stdio.h>
#include <string.h>
#include <vector>
#include <utility>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// Fixtures: a CCharString holding its text; animation entries with a bank index and a group (-1: none); the animation set as a
// key-sorted vector of (name key, entry) records whose FindFirstAnimWithName returns the first record of the key (keys are the
// first character of the name); filters with a virtual operator() (a filter accepting a set of banks, and the no-group filter);
// the mode manager's current mode with the GetAnimFilter virtual and the inline GetDefaultAnimFilter (its weapon filter); PeekTC.
class CCharString { public: char Text[32]; CCharString(const char* s) { strncpy(Text, s, 31); Text[31] = 0; } };
class CAnimationEntry { public: long BankIndex; long GroupName; };
class CAnimationSet { public:
    typedef std::vector<std::pair<unsigned long, CAnimationEntry> > TAnimMap;
    TAnimMap Anims;
    TAnimMap::const_iterator FindFirstAnimWithName(const CCharString& name) const
    {
        for (TAnimMap::const_iterator it = Anims.begin(); it != Anims.end(); ++it) if (it->first == (unsigned long)name.Text[0]) return it;
        return Anims.end();
    }
};
class CAnimFilterBase { public: virtual bool operator()(const CAnimationEntry& entry) const = 0; };
class CBankFilter : public CAnimFilterBase { public:
    long Lo, Hi; mutable long Calls;
    CBankFilter(long lo, long hi) : Lo(lo), Hi(hi), Calls(0) { }
    virtual bool operator()(const CAnimationEntry& entry) const { ++Calls; return entry.BankIndex >= Lo && entry.BankIndex <= Hi; }
};
class CAnimFilterNoGroup : public CAnimFilterBase { public:
    virtual bool operator()(const CAnimationEntry& entry) const { return entry.GroupName == -1; }
};
class CTCCreatureModeManager { public:
    class CModeBase { public:
        CBankFilter Best; CBankFilter Weapon;
        CModeBase() : Best(0, -1), Weapon(0, -1) { }
        virtual const CAnimFilterBase& GetAnimFilter() const { return Best; }
        const CAnimFilterBase& GetDefaultAnimFilter() const { return Weapon; }
    };
    CModeBase Mode; mutable long ModeCalls;
    CTCCreatureModeManager() : ModeCalls(0) { }
    CModeBase& GetCurrentMode() const { ++ModeCalls; return const_cast<CModeBase&>(Mode); }
};
static CAnimationEntry g_default = { 999, -1 };
static char g_fallbackName[32]; static long g_fallbackIndex = -7; static int g_fallbackCalls = 0;
class CThingCreatureBase { public:
    CAnimationSet Set; CTCCreatureModeManager* Modes;
    const CAnimationSet& GetCombatAnimationSet() const { return Set; }
    template <class T> bool PeekTC(T** ptc) const { if (!Modes) return false; *ptc = Modes; return true; }
    const CAnimationEntry& GetAnimFromName(const CCharString& name, long index) const
    {
        ++g_fallbackCalls; strcpy(g_fallbackName, name.Text); g_fallbackIndex = index; return g_default;
    }
    bool GetPossibleCombatAnimations(const CCharString& name, std::vector<const CAnimationEntry*>& anim_list) const;
};
// ---- row body (verbatim) ----
bool CThingCreatureBase::GetPossibleCombatAnimations(const CCharString& name, std::vector<const CAnimationEntry*>& anim_list) const
{
    bool found_anims = true;
    const CAnimationSet& anim_set = GetCombatAnimationSet();
    const CAnimationSet::TAnimMap& anims = anim_set.Anims;
    const CAnimFilterBase* panim_filter_best = 0;
    const CAnimFilterBase* panim_filter_weapon = 0;
    CAnimFilterNoGroup no_group_filter;
    const CTCCreatureModeManager* ptc_cmm;
    if (PeekTC(&ptc_cmm)) {
        panim_filter_best = &ptc_cmm->GetCurrentMode().GetAnimFilter();
        panim_filter_weapon = &ptc_cmm->GetCurrentMode().GetDefaultAnimFilter();
    }
    std::vector<const CAnimationEntry*> best_list;
    std::vector<const CAnimationEntry*> weapon_based_list;
    std::vector<const CAnimationEntry*> no_group_list;
    CAnimationSet::TAnimMap::const_iterator start_it = anim_set.FindFirstAnimWithName(name);
    for (CAnimationSet::TAnimMap::const_iterator it = start_it; it != anims.end() && it->first == start_it->first; ++it) {
        const CAnimationEntry& anim_entry = it->second;
        if (!panim_filter_best || (*panim_filter_best)(anim_entry))
            best_list.push_back(&anim_entry);
        else if (!panim_filter_weapon || (*panim_filter_weapon)(anim_entry))
            weapon_based_list.push_back(&anim_entry);
        else if (no_group_filter(anim_entry))
            no_group_list.push_back(&anim_entry);
    }
    if (!best_list.empty())
        anim_list = best_list;
    else if (!weapon_based_list.empty())
        anim_list = weapon_based_list;
    else if (!no_group_list.empty())
        anim_list = no_group_list;
    if (anim_list.empty()) {
        found_anims = false;
        const CAnimationEntry& entry = GetAnimFromName("DEFAULT", 0);
        anim_list.push_back(&entry);
    }
    return found_anims;
}
// ---- harness ----
static void Add(CThingCreatureBase& c, char key, long bank, long group)
{
    CAnimationEntry e = { bank, group }; c.Set.Anims.push_back(std::make_pair((unsigned long)key, e));
}
static bool Banks(const std::vector<const CAnimationEntry*>& v, int n, long b0, long b1 = 0, long b2 = 0, long b3 = 0)
{
    long b[4] = { b0, b1, b2, b3 };
    if ((int)v.size() != n) return false;
    for (int i = 0; i < n; ++i) if (v[i]->BankIndex != b[i]) return false;
    return true;
}
int main()
{
    CThingCreatureBase c; c.Modes = 0;
    Add(c, 'a', 1, 0); Add(c, 'b', 10, -1); Add(c, 'b', 11, 3); Add(c, 'b', 12, -1); Add(c, 'b', 13, 4); Add(c, 'c', 20, 0);
    std::vector<const CAnimationEntry*> out;
    // (1) no mode manager: every entry of the name goes to the first bucket.
    g_fallbackCalls = 0;
    CHECK(c.GetPossibleCombatAnimations(CCharString("bow"), out) && Banks(out, 4, 10, 11, 12, 13));
    CHECK(out[0] == &c.Set.Anims[1].second && g_fallbackCalls == 0);
    // (2) a mode whose filter accepts banks 11..12: that bucket, replacing anim_list's old contents.
    CTCCreatureModeManager modes; c.Modes = &modes;
    modes.Mode.Best = CBankFilter(11, 12); modes.Mode.Weapon = CBankFilter(13, 13);
    CHECK(c.GetPossibleCombatAnimations(CCharString("bow"), out) && Banks(out, 2, 11, 12));
    CHECK(modes.ModeCalls == 2 && modes.Mode.Best.Calls == 4 && modes.Mode.Weapon.Calls == 2);
    // (3) the mode filter accepts nothing: the weapon filter's bucket.
    modes.Mode.Best = CBankFilter(0, -1); modes.Mode.Weapon = CBankFilter(10, 11);
    CHECK(c.GetPossibleCombatAnimations(CCharString("bow"), out) && Banks(out, 2, 10, 11));
    // (4) both reject everything: the no-group entries (group -1).
    modes.Mode.Best = CBankFilter(0, -1); modes.Mode.Weapon = CBankFilter(0, -1);
    CHECK(c.GetPossibleCombatAnimations(CCharString("bow"), out) && Banks(out, 2, 10, 12));
    CHECK(g_fallbackCalls == 0);
    // (5) nothing matches (name 'c' has a grouped entry only), anim_list empty: the "DEFAULT" animation, index 0, returns false.
    out.clear();
    CHECK(!c.GetPossibleCombatAnimations(CCharString("cast"), out) && out.size() == 1 && out[0] == &g_default);
    CHECK(g_fallbackCalls == 1 && strcmp(g_fallbackName, "DEFAULT") == 0 && g_fallbackIndex == 0);
    // (6) nothing matches but anim_list already holds entries: kept, no fallback, returns true.
    CHECK(c.GetPossibleCombatAnimations(CCharString("cast"), out) && out.size() == 1 && g_fallbackCalls == 1);
    // (7) a name with no records at all and no mode manager: the fallback appended to an empty list.
    c.Modes = 0; out.clear();
    CHECK(!c.GetPossibleCombatAnimations(CCharString("zap"), out) && out.size() == 1 && out[0] == &g_default && g_fallbackCalls == 2);
    // (8) the run stops at the first key change: name 'a' collects bank 1 only.
    CHECK(c.GetPossibleCombatAnimations(CCharString("axe"), out) && Banks(out, 1, 1));
    if (fails == 0) printf("006668a0_TEST PASS\n");
    return fails;
}