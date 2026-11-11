// Behaviour test for 008d08a0: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CGameScriptThing::IsSummonedCreature() const -- a live thing that is a summoned creature: its summonable-creature component says it was summoned, or it is the GHOSTLY_SWORD def
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#include <string.h>
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) { } };
class CDefinitionManager { public:
    long GhostlySwordIndex;
    long GetDefGlobalIndexFromName(const CCharString& name) const { return strcmp(name.Text, "GHOSTLY_SWORD") == 0 ? GhostlySwordIndex : -1; }
};
class CGameDefinitionManager : public CDefinitionManager { };
static CGameDefinitionManager g_defs;
class CTCSummonableCreature { public: bool HasBeenSummoned; bool GetHasBeenSummoned() const { return HasBeenSummoned; } };
class CThing { public:
    bool Dead; unsigned short DefGlobalIndex; const CTCSummonableCreature* Summonable;
    bool IsDead() const { return Dead; }
    unsigned long GetDefGlobalIndex() const { return DefGlobalIndex; }
    template <class T> bool PeekTC(const T** out) const { if (!Summonable) return false; *out = Summonable; return true; }
    const CGameDefinitionManager& PeekDefinitionManager() const { return g_defs; }
};
template <class T> class CIntelligentPointer { public: T* Item; mutable int Gets; T* Get() const { ++Gets; return Item; } };
class CGameScriptThing { public:
    CIntelligentPointer<CThing> PThing;
    virtual bool IsSummonedCreature() const;
};
// ---- row body (verbatim) ----
bool CGameScriptThing::IsSummonedCreature() const
{
    if (PThing.Get() && !PThing.Get()->IsDead()) {
        const CTCSummonableCreature* summonable;
        if (PThing.Get()->PeekTC(&summonable) && summonable->GetHasBeenSummoned())
            return true;
        long ghostly_sword = PThing.Get()->PeekDefinitionManager().GetDefGlobalIndexFromName("GHOSTLY_SWORD");
        if (ghostly_sword > 0 && PThing.Get()->GetDefGlobalIndex() == ghostly_sword)
            return true;
    }
    return false;
}
// ---- harness ----
int main()
{
    g_defs.GhostlySwordIndex = 77;
    CThing thing = { false, 5, 0 }; CTCSummonableCreature tc = { true };
    CGameScriptThing script; script.PThing.Item = 0; script.PThing.Gets = 0;
    CHECK(!script.IsSummonedCreature());                       // no thing
    script.PThing.Item = &thing;
    CHECK(!script.IsSummonedCreature());                       // live, no component, other def
    thing.Summonable = &tc; CHECK(script.IsSummonedCreature());  // summoned
    tc.HasBeenSummoned = false; CHECK(!script.IsSummonedCreature());
    thing.DefGlobalIndex = 77; CHECK(script.IsSummonedCreature()); // the ghostly sword def
    thing.Dead = true; CHECK(!script.IsSummonedCreature());    // dead
    g_defs.GhostlySwordIndex = 0; thing.Dead = false; thing.DefGlobalIndex = 0; CHECK(!script.IsSummonedCreature());   // no such def
    if (fails == 0) printf("008d08a0_TEST PASS\n");
    return fails;
}
