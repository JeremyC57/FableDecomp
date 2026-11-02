// test: CCreatureAction_CastStrengthMultiStrikeProper::FinishAction -- kill every live dependent thing, then the combat base FinishAction
#include <stdio.h>
#include <vector>
static int kills = 0, baseCalls = 0;
struct Thing { bool Dead; int killed; Thing(bool d) : Dead(d), killed(0) { } bool IsDead() const { return Dead; } void Kill(bool b) { if (b) { ++killed; ++kills; } } };
struct Ptr { Thing* p; Ptr(Thing* t) : p(t) { } Thing* GetPItem() const { return p; } };
struct CombatBase { std::vector<Ptr> DependentThings; void FinishAction() { if (kills >= 0) ++baseCalls; } };
struct Proper : CombatBase {
    void FinishAction() {
        for (std::vector<Ptr>::iterator it = DependentThings.begin(); it != DependentThings.end(); ++it) {
            Thing* thing = it->GetPItem();
            if (thing && !thing->IsDead()) thing->Kill(true);
        }
        CombatBase::FinishAction();
    }
};
int main() {
    Thing a(false), b(true), c(false);
    Proper p; p.DependentThings.push_back(Ptr(&a)); p.DependentThings.push_back(Ptr(0));
    p.DependentThings.push_back(Ptr(&b)); p.DependentThings.push_back(Ptr(&c));
    p.FinishAction();
    if (kills != 2 || a.killed != 1 || b.killed != 0 || c.killed != 1 || baseCalls != 1) { printf("008e63b0_TEST FAIL\n"); return 1; }
    Proper e; e.FinishAction();
    if (baseCalls != 2 || kills != 2) { printf("008e63b0_TEST FAIL empty\n"); return 1; }
    printf("008e63b0_TEST PASS\n");
    return 0;
}