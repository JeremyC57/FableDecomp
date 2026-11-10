// Behaviour test for CTCInventoryAbilities::GetPrioritisedAbilityDefs (0x005F4B30): a child gets nothing; an adult gets the
// received abilities' defs up to the last received one, padded with null defs to a multiple of the button count.
#include <stdio.h>
#include <vector>
struct Def { int Id; };
struct DefPtr { const Def* Object; DefPtr() : Object(0) {} explicit DefPtr(const Def* d) : Object(d) {} };
static Def g_defs[8];
struct Creature { bool Child; bool IsChild() const { return Child; } };
struct Abilities {
    Creature* ParentThing;
    std::vector<int> ReceivedAbilityOrder;
    long Buttons;
    DefPtr GetAbilityPDef(int ability) const { return ability ? DefPtr(&g_defs[ability]) : DefPtr(); }
    long GetNumAssignableSpecialMoveButtons() const { return Buttons; }
    void GetPrioritisedAbilityDefs(std::vector<DefPtr>& defs) const
    {
        if (ParentThing->IsChild())
            return;
        long lastReceived = -1;
        for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
            defs.push_back(GetAbilityPDef(ReceivedAbilityOrder[i]));
            if (ReceivedAbilityOrder[i] != 0)
                lastReceived = i;
        }
        std::vector<DefPtr> prioritised;
        long j;
        for (j = 0; j <= lastReceived; ++j)
            prioritised.push_back(defs[j]);
        for (unsigned long k = 0; k < prioritised.size() % GetNumAssignableSpecialMoveButtons(); ++k)
            prioritised.push_back(DefPtr());
        defs = prioritised;
    }
};
int main()
{
    Creature hero; hero.Child = false;
    Abilities a; a.ParentThing = &hero; a.Buttons = 4;
    int order[] = { 3, 0, 5, 0, 0 };
    for (int i = 0; i < 5; ++i) a.ReceivedAbilityOrder.push_back(order[i]);
    std::vector<DefPtr> defs;
    a.GetPrioritisedAbilityDefs(defs);
    // up to slot 2 (ability 5) = 3 defs, padded to 4
    if (defs.size() != 4 || defs[0].Object != &g_defs[3] || defs[1].Object != 0 || defs[2].Object != &g_defs[5] || defs[3].Object != 0) {
        printf("005f4b30_TEST FAIL\n"); return 1;
    }
    hero.Child = true;
    std::vector<DefPtr> none;
    a.GetPrioritisedAbilityDefs(none);
    if (!none.empty()) { printf("005f4b30_TEST FAIL\n"); return 1; }
    printf("005f4b30_TEST PASS\n");
    return 0;
}
