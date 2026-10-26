// 005eea9b behaviour test: CTCInventoryAbilities::OnSpecialAbilityUsed (control flow + data of the final.cpp body)
#include <stdio.h>
#include <string.h>
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
class CCharString { public: const char* Text; CCharString(const char* t, long = -1) : Text(t) {} ~CCharString() {} };
class C3DVector { public: float X, Y, Z; };
class CTCBase;
struct CPresenceBits { unsigned long Bits[9]; bool test(long bit) const { return (Bits[bit >> 5] >> (bit & 31)) & 1; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* lo = first;
        long count = last - first;
        while (count > 0) {
            long half = count >> 1;
            const SEntry* mid = lo + half;
            if (mid->type < *type) { lo = mid + 1; count -= half + 1; } else count = half;
        }
        return lo;
    }
    SEntry* LowerBound(const long* type) { return (SEntry*)((const CInterfaceMap*)this)->LowerBound(type); }
};
class CThing { public:
    CPresenceBits  m_present;
    CInterfaceMap  m_interfaces;
    C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
    template<class T> bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> bool GetInterfaceFound(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it != map->last && !(wanted < it->type)) ? it : map->last)->component;
            return true;
        }
        return false;
    }
};
static int g_destroys;
class CDefPointeeBase { public: void Destroy() const { ++g_destroys; } mutable long RefCount; };
class CHeroAbilityDef : public CDefPointeeBase { public: long TrainableStat; long* WillCosts; long TrainableStatExperience; };
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct SAbilityEntry { long first; CDefPointer<const CHeroAbilityDef> second; };
struct CAbilityEntryVector {
    SAbilityEntry* _M_start; SAbilityEntry* _M_finish; SAbilityEntry* _M_end_of_storage;
    SAbilityEntry& operator[](unsigned long n) { return _M_start[n]; }
};
static bool g_toggle, g_use = true, g_able = true;
static long g_lastUsed = -1;
class CTCSpecialAbilities { public:
    bool ToggleUsingAbility(EHeroAbility) { return g_toggle; }
    bool UseSpecialAbility(EHeroAbility a) { g_lastUsed = a; return g_use; }
};
class CTCHeroStats { public: long* AbilityUseCounts; long Stamina; void ModifyStaminaBy(long amount) { Stamina += amount; } };
struct CTCHeroState { bool CanUseAbilities; };
struct CTCSpellCost { long Cost; long GetCost() { return Cost; } };
class CTCHeroExperience { public: long Stat, Amount; void AddTrainableStatExperience(long stat, long amount) { Stat = stat; Amount += amount; } };
static int g_sounds;
struct CSoundInfo { unsigned long PlaySound(const CCharString& name, long, long, long, long) { if (!strcmp(name.Text, "HERO_ABILITY_CAST_FAILED")) ++g_sounds; return 0; } };
class CMessageEvent;
static int g_messages; static long g_msgParam; static unsigned long g_msgFlags;
class CMessageEventManager { public:
    CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector*, CThing*, CThing*, float, float, long priority)
    { if (type == 0x44) { ++g_messages; g_msgParam = priority; g_msgFlags = flags; } return 0; }
};
static CMessageEventManager g_mem;
class CWorld { public: CMessageEventManager* PMessageEventManager; };
static CWorld g_world = { &g_mem };
static long g_FrameRate = 30;
class CTCInventoryAbilities { public:
    CThing* ParentThing;
    bool    CurrentSpecialAbilityUsedFromQuickAccess;
    CAbilityEntryVector Abilities;
    CWorld* GetWorld() { return &g_world; }
    bool IsAbleToUseAbility(EHeroAbility, bool) const { return g_able; }
    void OnSpecialAbilityUsed(EHeroAbility ability, bool fromQuickAccess);
};
void CTCInventoryAbilities::OnSpecialAbilityUsed(EHeroAbility ability, bool fromQuickAccess)
{
    CTCSpecialAbilities* special;
    if (!ParentThing->GetInterface(&special, 0xa7))
        return;
    if (special->ToggleUsingAbility(ability))
        return;
    if (IsAbleToUseAbility(ability, true)) {
        CTCHeroStats* stats = 0;
        ParentThing->GetInterface(&stats, 4);
        CTCHeroState* state;
        if (!static_cast<const CThing*>(ParentThing)->GetInterface(&state, 0x29))
            return;
        if (!state->CanUseAbilities)
            return;
        CAbilityEntryVector* abilitiesP = &Abilities;
        CAbilityEntryVector& abilities = *abilitiesP;
        CDefPointer<const CHeroAbilityDef> def = abilities[ability].second;
        long cost = -def->WillCosts[abilities[ability].first];
        if (ability == 10) {
            const CThing* parent = ParentThing;
            CTCSpellCost* spellCost;
            if (parent->GetInterface(&spellCost, 0x91))
                cost = -spellCost->GetCost();
        }
        if (special->UseSpecialAbility(ability)) {
            CurrentSpecialAbilityUsedFromQuickAccess = fromQuickAccess;
            CThing* parent = ParentThing;
            long frames = g_FrameRate;
            CMessageEventManager* messages = GetWorld()->PMessageEventManager;
            messages->AddMessage(0x44, frames, &parent->GetPosition(), 0, 0, 0.0f, 0.0f, ability);
            stats->ModifyStaminaBy(cost);
            ++stats->AbilityUseCounts[ability];
            if (ability != 0xb && ability != 5 && ability != 0x12 && ability != 0x13) {
                CTCHeroExperience* experience = 0;
                ParentThing->GetInterface(&experience, 0x68);
                experience->AddTrainableStatExperience(def->TrainableStat, def->TrainableStatExperience);
            }
        }
    } else {
        CSoundInfo* sound;
        if (ParentThing->GetInterfaceFound(&sound, 0x10))
            sound->PlaySound("HERO_ABILITY_CAST_FAILED", 1, 0, 0, -1);
    }
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }
int main()
{
    CTCSpecialAbilities special; CTCHeroStats stats; long counts[32] = { 0 }; stats.AbilityUseCounts = counts; stats.Stamina = 100;
    CTCHeroState state = { true }; CTCSpellCost spell = { 7 }; CTCHeroExperience exp = { 0, 0 }; CSoundInfo sound;
    CInterfaceMap::SEntry e[6] = { { 4, (CTCBase*)&stats }, { 0x10, (CTCBase*)&sound }, { 0x29, (CTCBase*)&state },
        { 0x68, (CTCBase*)&exp }, { 0x91, (CTCBase*)&spell }, { 0xa7, (CTCBase*)&special } };
    CThing hero; memset(&hero.m_present, 0, sizeof(hero.m_present));
    for (int i = 0; i < 6; ++i) hero.m_present.Bits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
    hero.m_interfaces.first = e; hero.m_interfaces.last = e + 6;
    long costs[3] = { 5, 10, 15 };
    CHeroAbilityDef defs[20];
    SAbilityEntry entries[20];
    for (int i = 0; i < 20; ++i) {
        defs[i].RefCount = 1; defs[i].TrainableStat = i; defs[i].WillCosts = costs; defs[i].TrainableStatExperience = 3;
        entries[i].first = i % 3;
        entries[i].second.Object = &defs[i]; ++defs[i].RefCount;   // held by the vector
    }
    CTCInventoryAbilities inv; inv.ParentThing = &hero; inv.CurrentSpecialAbilityUsedFromQuickAccess = false;
    inv.Abilities._M_start = entries; inv.Abilities._M_finish = entries + 20; inv.Abilities._M_end_of_storage = entries + 20;

    inv.OnSpecialAbilityUsed((EHeroAbility)4, true);   // cost index 1 -> -10, trains stat 4
    Check("use", g_lastUsed == 4 && stats.Stamina == 90 && counts[4] == 1 && inv.CurrentSpecialAbilityUsedFromQuickAccess);
    Check("message", g_messages == 1 && g_msgParam == 4 && g_msgFlags == 30);
    Check("train", exp.Stat == 4 && exp.Amount == 3 && defs[4].RefCount == 2);
    inv.OnSpecialAbilityUsed((EHeroAbility)10, false);  // spell cost component overrides: -7
    Check("spell cost", stats.Stamina == 83 && counts[10] == 1 && exp.Stat == 10 && !inv.CurrentSpecialAbilityUsedFromQuickAccess);
    inv.OnSpecialAbilityUsed((EHeroAbility)11, false);  // no training for 11 (cost index 2 -> -15)
    Check("no train", stats.Stamina == 68 && exp.Stat == 10 && exp.Amount == 6);
    g_use = false;
    inv.OnSpecialAbilityUsed((EHeroAbility)2, false);
    Check("use fails", stats.Stamina == 68 && counts[2] == 0 && g_messages == 3);
    g_use = true; state.CanUseAbilities = false;
    inv.OnSpecialAbilityUsed((EHeroAbility)2, false);
    Check("cannot use", g_lastUsed == 2 && counts[2] == 0 && g_messages == 3);
    state.CanUseAbilities = true; g_toggle = true;
    inv.OnSpecialAbilityUsed((EHeroAbility)3, false);
    Check("toggled", counts[3] == 0);
    g_toggle = false; g_able = false;
    inv.OnSpecialAbilityUsed((EHeroAbility)3, false);
    Check("not able", g_sounds == 1 && counts[3] == 0);
    Check("refs", g_destroys == 0 && defs[2].RefCount == 2 && defs[10].RefCount == 2);
    if (g_fail) return 1;
    printf("005eea9b_TEST PASS\n");
    return 0;
}