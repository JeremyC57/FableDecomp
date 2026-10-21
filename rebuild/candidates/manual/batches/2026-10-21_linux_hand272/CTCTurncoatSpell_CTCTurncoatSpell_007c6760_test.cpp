// Behaviour test for CTCTurncoatSpell::CTCTurncoatSpell (0x007c6760): helper timer for ability 5, ability-cast component marked, turncoat def loaded, ability level read, per-level tables turned into frames and fractions.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
class CTCBase;
class CThing;
static const char* g_defName; static int g_strings;
struct CCharString {
    const char* m_data;
    CCharString(const char* text, long length = -1) : m_data(text) { (void)length; ++g_strings; }
    ~CCharString() { --g_strings; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type) { SEntry* it = first; while (it != last && it->type < type) ++it; return it; }
    const SEntry* LowerBound(const long& type) const { const SEntry* it = first; while (it != last && it->type < type) ++it; return it; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTCC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CTCBaseModel {
    CThing* ParentThing;
    CTCBaseModel(CThing* owner) : ParentThing(owner) { }
};
struct CWillButtonHelperTimer {
    long Ability; CThing* Owner;
    CWillButtonHelperTimer(long ability, CThing* owner) : Ability(ability), Owner(owner) { }
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem; int sets;
    CBaseIntelligentPointer() : m_pItem((CBaseObject*)1), sets(0) { }
    void SetPItem(CBaseObject* item) { m_pItem = item; ++sets; }
};
struct CThingIntelligentPointer : CBaseIntelligentPointer {
    CThingIntelligentPointer() { }
};
struct CCreatureIntelligentPointer : CBaseIntelligentPointer {
    CCreatureIntelligentPointer() { }
    CCreatureIntelligentPointer(CBaseObject* item) { SetPItem(item); }
};
struct CFloatVector { float* First; float* Last; float* EndOfStorage; };
struct CTurncoatSpellDef {
    CFloatVector DurationSeconds;
    CFloatVector ChargeSeconds;
    CFloatVector CooldownSeconds;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
static CTurncoatSpellDef g_def;
struct CDefinitionManager {
    long GetDefIndex(const CCharString& name, bool required) { g_defName = name.m_data; (void)required; return 77; }
    bool GetDef(long index, CDefPointer<CTurncoatSpellDef>* def) { def->Object = index == 77 ? &g_def : 0; return true; }
};
static CDefinitionManager g_mgr;
CDefinitionManager* __fastcall GetDefinitionManager() { return &g_mgr; }
struct CTCAbilityCast { bool Busy; long Ability; };
struct CTCInventoryAbilities { long Level; long GetAbilityLevel(long ability) { return ability == 5 ? Level : -1; } };
static long g_FramesPerSecond = 30;
struct CTCTurncoatSpell : CTCBaseModel {
    CWillButtonHelperTimer Timer;
    CDefPointer<CTurncoatSpellDef> Def;
    CThingIntelligentPointer Target;
    long Level;
    long State;
    CCreatureIntelligentPointer Caster;
    float DurationFraction;
    long Counter;
    long DurationFrames;
    float ChargeFraction;
    long Counter2;
    long ChargeFrames;
    CCreatureIntelligentPointer Creature1;
    CCreatureIntelligentPointer Creature2;
    CCreatureIntelligentPointer Creature3;
    bool Active;
    long CooldownLeft;
    long Cooldown;
    CTCTurncoatSpell(CThing* owner);
};

CTCTurncoatSpell::CTCTurncoatSpell(CThing* owner)
    : CTCBaseModel(owner), Timer(5, ParentThing), Level(0), State(0), Caster(0), Counter(0), Counter2(0),
      Creature3(0), Active(false), CooldownLeft(0), Cooldown(0)
{
    if (ParentThing->HasTC(0xa7)) {
        CTCAbilityCast* cast = (CTCAbilityCast*)ParentThing->GetTC(0xa7);
        cast->Busy = true;
        cast->Ability = 5;
    }
    long index = GetDefinitionManager()->GetDefIndex("SPECIAL_ABILITIES_TURNCOAT_SPELL_DEF", true);
    GetDefinitionManager()->GetDef(index, &Def);
    if (ParentThing->HasTC(0x6f))
        Level = ((CTCInventoryAbilities*)ParentThing->GetTCC(0x6f))->GetAbilityLevel(5);
    CTurncoatSpellDef* def = Def.Object;
    float duration = def->DurationSeconds.First[Level] / (float)g_FramesPerSecond;
    long whole = (long)duration;
    DurationFrames = whole;
    DurationFraction = duration - (float)whole;
    float charge = def->ChargeSeconds.First[Level] / (float)g_FramesPerSecond;
    whole = (long)charge;
    ChargeFrames = whole;
    ChargeFraction = charge - (float)whole;
    long cooldown = (long)((float)g_FramesPerSecond * def->CooldownSeconds.First[Level]);
    Cooldown = cooldown;
    CooldownLeft = cooldown;
}

static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    int fails = 0;
    float dur[3] = { 15.0f, 75.0f, 45.0f };
    float chg[3] = { 3.0f, 9.0f, 33.0f };
    float cd[3] = { 1.0f, 2.5f, 4.0f };
    g_def.DurationSeconds.First = dur; g_def.ChargeSeconds.First = chg; g_def.CooldownSeconds.First = cd;
    static CThing owner; memset(&owner, 0, sizeof(owner));
    static CTCAbilityCast cast; cast.Busy = false; cast.Ability = 0;
    static CTCInventoryAbilities abilities; abilities.Level = 1;
    CInterfaceMap::SEntry entries[2] = { { 0x6f, (CTCBase*)&abilities }, { 0xa7, (CTCBase*)&cast } };
    owner.m_interfaces.first = entries; owner.m_interfaces.last = entries + 2;
    owner.m_presentBits[0xa7 >> 5] |= 1UL << (0xa7 & 31);
    owner.m_presentBits[0x6f >> 5] |= 1UL << (0x6f & 31);
    CTCTurncoatSpell spell(&owner);
    if (spell.ParentThing != &owner || spell.Timer.Ability != 5 || spell.Timer.Owner != &owner) ++fails;
    if (!cast.Busy || cast.Ability != 5) ++fails;
    if (strcmp(g_defName, "SPECIAL_ABILITIES_TURNCOAT_SPELL_DEF") != 0 || spell.Def.Object != &g_def || g_strings != 0) ++fails;
    if (spell.Level != 1) ++fails;
    if (spell.DurationFrames != 2 || !Near(spell.DurationFraction, 0.5f)) ++fails;
    if (spell.ChargeFrames != 0 || !Near(spell.ChargeFraction, 0.3f)) ++fails;
    if (spell.Cooldown != 75 || spell.CooldownLeft != 75) ++fails;
    if (spell.Caster.m_pItem != 0 || spell.Caster.sets != 1 || spell.Creature3.sets != 1 || spell.Creature1.sets != 0) ++fails;
    if (spell.State != 0 || spell.Counter != 0 || spell.Counter2 != 0 || spell.Active) ++fails;
    static CThing bare; memset(&bare, 0, sizeof(bare));             // no components: level 0
    CTCTurncoatSpell plain(&bare);
    if (plain.Level != 0 || plain.DurationFrames != 0 || !Near(plain.DurationFraction, 0.5f) || plain.Cooldown != 30) ++fails;
    if (fails) { printf("007c6760_TEST FAIL %d\n", fails); return 1; }
    printf("007c6760_TEST PASS\n");
    return 0;
}
