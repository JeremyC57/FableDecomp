// Behaviour test for 00890f40: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CGameScriptInterface::EntityResetCombatTypeToDefault(const CScriptThing& thing) const -- for a live creature, set its combat type back to the one its creature def names (through EntitySetCombatType)
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#include <string>
class CCharString { public: std::string Text; CCharString(const char* t) : Text(t) { } };
class CDefString { public: long TablePos; operator CCharString() const { static const char* names[3] = { "COMBAT_NONE", "COMBAT_SWORD", "COMBAT_STAFF" }; return CCharString(names[TablePos]); } };
class CGameDefinitionManager { public:
    CDefString GetDefNameFromGlobalIndex(long index) const { CDefString s = { index }; return s; }
};
static CGameDefinitionManager g_defs;
class CThing { public:
    bool Dead; unsigned long ThingTypeFlags;
    bool IsDead() const { return Dead; }
    unsigned long GetTypeFlags() const { return ThingTypeFlags; }
    const CGameDefinitionManager& PeekDefinitionManager() const { return g_defs; }
};
class CThingCreatureBase : public CThing { public: long CombatType; long GetCombatType() const { return CombatType; } };
class CScriptThing { public: CThing* Thing; virtual CThing* GetPThing() const { return Thing; } };
static std::string g_set; static const CScriptThing* g_setFor; static int g_sets;
class CGameScriptInterface { public:
    virtual void EntitySetCombatType(const CScriptThing& thing, const CCharString& combat_type) const { g_set = combat_type.Text; g_setFor = &thing; ++g_sets; }
    virtual void EntityResetCombatTypeToDefault(const CScriptThing& thing) const;
};
// ---- row body (verbatim) ----
void CGameScriptInterface::EntityResetCombatTypeToDefault(const CScriptThing& thing) const
{
    CThing* pthing = thing.GetPThing();
    if (pthing && !pthing->IsDead() && (pthing->GetTypeFlags() & 8)) {
        CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(pthing);
        EntitySetCombatType(thing, pthing->PeekDefinitionManager().GetDefNameFromGlobalIndex(creature->GetCombatType()));
    }
}
// ---- harness ----
int main()
{
    CThingCreatureBase creature; creature.Dead = false; creature.ThingTypeFlags = 15; creature.CombatType = 2;
    CScriptThing script; script.Thing = &creature;
    CGameScriptInterface gsi;
    gsi.EntityResetCombatTypeToDefault(script);
    CHECK(g_sets == 1 && g_set == "COMBAT_STAFF" && g_setFor == &script);
    creature.ThingTypeFlags = 7; gsi.EntityResetCombatTypeToDefault(script); CHECK(g_sets == 1);   // not a creature
    creature.ThingTypeFlags = 15; creature.Dead = true; gsi.EntityResetCombatTypeToDefault(script); CHECK(g_sets == 1);
    script.Thing = 0; gsi.EntityResetCombatTypeToDefault(script); CHECK(g_sets == 1);
    script.Thing = &creature; creature.Dead = false; creature.CombatType = 1; gsi.EntityResetCombatTypeToDefault(script);
    CHECK(g_sets == 2 && g_set == "COMBAT_SWORD");
    if (fails == 0) printf("FSE2_00890f40_TEST PASS\n");
    return fails;
}
