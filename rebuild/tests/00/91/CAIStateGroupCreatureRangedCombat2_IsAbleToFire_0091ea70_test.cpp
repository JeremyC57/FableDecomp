// Behaviour test for 0x0091EA70 CAIStateGroup_CreatureRangedCombat2::IsAbleToFire() const: a model of the row; false without
// the perceive-thing component or a live target; otherwise true, except for a guard village member whose village (asked
// twice) holds the hero a criminal while the target's fine dialogue is allowed.
#include <cstdio>
struct Village { bool criminal; int calls; bool IsHeroCriminal() { ++calls; return criminal; } };
struct Member { Village* village; int calls; Village* GetPVillage() const { ++const_cast<Member*>(this)->calls; return village; } };
struct Fine { bool allowed; bool IsAllowed() { return allowed; } };
struct Thing { bool perceive, guard, dead; Member* member; Fine* fine; };
struct Group {
    Thing* creature; Thing* target;
    bool IsAbleToFire() const
    {
        if (creature->perceive && target && !target->dead) {
            if (creature->guard && creature->member) {
                Member* member = creature->member;
                if (member->GetPVillage() && member->GetPVillage()->IsHeroCriminal()) {
                    if (target->fine && target->fine->IsAllowed())
                        return false;
                }
            }
            return true;
        }
        return false;
    }
};
int main()
{
    Village v = { true, 0 }; Member m = { &v, 0 }; Fine f = { true };
    Thing c = { true, true, false, &m, 0 }; Thing t = { false, false, false, 0, &f };
    Group g = { &c, &t };
    int ok = 1;
    ok &= !g.IsAbleToFire() && m.calls == 2;                 // criminal + fine allowed: talk, not shoot
    f.allowed = false; ok &= g.IsAbleToFire();
    f.allowed = true; v.criminal = false; ok &= g.IsAbleToFire();
    c.guard = false; v.criminal = true; ok &= g.IsAbleToFire();
    t.dead = true; ok &= !g.IsAbleToFire();
    t.dead = false; c.perceive = false; ok &= !g.IsAbleToFire();
    std::printf(ok ? "0091ea70_TEST PASS\n" : "0091ea70_TEST FAIL\n");
    return ok ? 0 : 1;
}