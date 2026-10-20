// Behaviour test for 0x0079A6E0 CTCSummonSpell::CTCSummonSpell (copy of the source logic over spy definitions).
#include <cstdio>
#include <cstring>
class CThing;
struct CTCBaseC { virtual ~CTCBaseC() {} };
struct CCharString {
    const char* text;
    CCharString(const char* t, long) : text(t) {}
    CCharString(const CCharString& o) : text(o.text) {}
};
static int g_getStringId = -1;
struct CDefStringTable { CCharString GetString(long id) { g_getStringId = (int)id; return CCharString("CREATURE_SUMMONED_HOBBE", -1); } };
static CDefStringTable g_DefStringTable;
class CSpecialAbilitiesSummonSpellDef {
public:
    long  BaseSummonCreatureName;
    float FadeInTime;
};
static CSpecialAbilitiesSummonSpellDef g_def;
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
};
static int g_nameLookups = 0; static long g_classIndex = 0; static const char* g_className = 0;
class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const { g_className = className.text; g_classIndex = classIndex; return 77; }
    long GetDefGlobalIndexFromName(const CCharString& name) { ++g_nameLookups; return std::strcmp(name.text, "CREATURE_SUMMONED_HOBBE") == 0 ? 1234 : 0; }
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const { if (index != 77) return false; out->Object = &g_def; return true; }
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    void SetPItem(CThing* item) { m_pItem = item; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() {}
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
};
class CThingAICreature;
static long g_abilityAsked = -1;
class CTCInventoryAbilities : public CTCBaseC { public: long GetAbilityLevel(long ability) { g_abilityAsked = ability; return 3; } };
class CThing {
public:
    bool hasAbilities; CTCInventoryAbilities* abilities;
    bool HasTC(long type) const { return type == 0x6f && hasAbilities; }
    CTCBaseC* GetTC(long) const { return abilities; }
};
static long g_framesPerSecond = 30;
class CTCBase {
public:
    CTCBase(CThing* parent) : ParentThing(parent) {}
    virtual ~CTCBase() {}
    CThing* ParentThing;
};
class CTCSummonSpell : public CTCBase {
public:
    CDefPointer<const CSpecialAbilitiesSummonSpellDef> PDef;
    bool JustReleased;
    CIntelligentPointer<CThingAICreature> Minion;
    long FadeInTimer, CurFadeInTimer, SpellLevel, CurTimer;
    CIntelligentPointer<CThing> PItemRemoved;
    bool CutSceneCancelled;
    static long SummonCreatureGlobalDefNum;
    CTCSummonSpell(CThing& parent);
};
long CTCSummonSpell::SummonCreatureGlobalDefNum = -1;
CTCSummonSpell::CTCSummonSpell(CThing& parent)
    : CTCBase(&parent), JustReleased(true), FadeInTimer(0), CurFadeInTimer(0), SpellLevel(0), CurTimer(0),
      PItemRemoved(0), CutSceneCancelled(false)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_SUMMON_SPELL_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    if (SummonCreatureGlobalDefNum == -1) {
        CCharString creatureName = g_DefStringTable.GetString(PDef->BaseSummonCreatureName);
        SummonCreatureGlobalDefNum = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(creatureName);
    }
    FadeInTimer = CurFadeInTimer = (long)(g_framesPerSecond * PDef->FadeInTime);
    if (ParentThing->HasTC(0x6f))
        SpellLevel = ((CTCInventoryAbilities*)ParentThing->GetTC(0x6f))->GetAbilityLevel(10);
}
static int fail() { std::printf("0079a6e0_TEST FAIL\n"); return 1; }
int main()
{
    g_def.BaseSummonCreatureName = 55; g_def.FadeInTime = 1.5f;
    CTCInventoryAbilities abilities;
    CThing hero; hero.hasAbilities = true; hero.abilities = &abilities;
    CTCSummonSpell a(hero);
    if (a.ParentThing != &hero || a.PDef.Object != &g_def || !a.JustReleased || a.CutSceneCancelled || a.CurTimer != 0) return fail();
    if (std::strcmp(g_className, "SPECIAL_ABILITIES_SUMMON_SPELL_DEF") != 0 || g_classIndex != 1) return fail();
    if (CTCSummonSpell::SummonCreatureGlobalDefNum != 1234 || g_getStringId != 55 || g_nameLookups != 1) return fail();
    if (a.FadeInTimer != 45 || a.CurFadeInTimer != 45 || a.SpellLevel != 3 || g_abilityAsked != 10) return fail();
    if (a.Minion.m_pItem != 0 || a.PItemRemoved.m_pItem != 0) return fail();
    hero.hasAbilities = false;
    CTCSummonSpell b(hero);                                   // the creature def is resolved only once
    if (g_nameLookups != 1 || b.SpellLevel != 0) return fail();
    std::printf("0079a6e0_TEST PASS\n");
    return 0;
}