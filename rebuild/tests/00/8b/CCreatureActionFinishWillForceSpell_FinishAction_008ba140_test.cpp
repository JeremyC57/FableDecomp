// behaviour test for 0x008ba140 CCreatureAction_FinishWillForceSpell::FinishAction(): the combat FinishAction first, then
// the creature's CTCSpecialAbilities releases its ability mutex and current ability; a live removed item is
// re-drawn with an unsheathe action ("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE", 100, 250, no follow-up)
#include <cstdio>
#include <cstring>
static int gStrings = 0, gOrder = 0;
struct Str { const char* s; Str(const char* t) : s(t) { ++gStrings; } ~Str() { --gStrings; } };
struct Abilities { bool AbilityMutexLock; long CurrentAbility; };
typedef Abilities Comp;
struct Thing { bool dead; Comp* comp; bool IsDead() const { return dead; } bool GetTC(Comp** out) { if (!comp) return false; *out = comp; return true; } };
struct Unsheathe { Thing* creature; char anim[48]; Thing* item; long a, b; void* follow;
    Unsheathe(Thing& c, const Str& n, Thing& i, long x, long y, void* f) : creature(&c), item(&i), a(x), b(y), follow(f) { std::strcpy(anim, n.s); } };
struct Creature : Thing { Unsheathe last; int sets;
    Creature(Comp* c) : last(*this, Str(""), *this, 0, 0, 0), sets(0) { dead = false; comp = c; }
    bool SetCurrentAction(const Unsheathe& u) { last = u; ++sets; return true; } };
struct Action {
    Creature* PCreature; Thing* PItemRemoved; int finishAt;
    void BaseFinishAction() { finishAt = ++gOrder; }
    void FinishAction()
    {
        BaseFinishAction();
        Comp* abilities;
        if (PCreature->GetTC(&abilities)) {
            abilities->AbilityMutexLock = false;
            abilities->CurrentAbility = 0;   // EHeroAbility_None
        }
        if (PItemRemoved && !PItemRemoved->IsDead()) {
            Str animName("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE");
            PCreature->SetCurrentAction(Unsheathe(*PCreature, animName, *PItemRemoved, 100, 250, 0));
        }
    }
};
int main()
{
    int fails = 0;
    Comp comp; comp.AbilityMutexLock = true; comp.CurrentAbility = 5;
    Creature hero(&comp);
    Thing sword = { false, 0 };
    Action act = { &hero, &sword, 0 };
    act.FinishAction();
    if (comp.AbilityMutexLock || comp.CurrentAbility != 0) fails |= 1;
    if (act.finishAt != 1 || hero.sets != 1 || hero.last.creature != &hero || hero.last.item != &sword) fails |= 2;
    if (std::strcmp(hero.last.anim, "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE") != 0 || hero.last.a != 100 || hero.last.b != 250
        || hero.last.follow || gStrings != 0) fails |= 4;
    sword.dead = true; act.FinishAction();
    act.PItemRemoved = 0; act.FinishAction();
    if (hero.sets != 1 || act.finishAt != 3) fails |= 8;
    if (fails) { std::printf("008ba140_TEST FAIL %d\n", fails); return 1; }
    std::printf("008ba140_TEST PASS\n");
    return 0;
}