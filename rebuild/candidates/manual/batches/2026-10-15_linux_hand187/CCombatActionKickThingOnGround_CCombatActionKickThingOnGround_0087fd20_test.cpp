#include <cstdio>
// Behaviour test for the CCombatAction_KickThingOnGround constructor @ 0x0087FD20 (kick vector and value).
struct V { float X, Y, Z; V() { } V(float x, float y, float z) : X(x), Y(y), Z(z) { }
    V operator-(const V& o) const { return V(X - o.X, Y - o.Y, Z - o.Z); }
    V operator+(const V& o) const { return V(X + o.X, Y + o.Y, Z + o.Z); } };
struct Phys { V cur, prev; const V& GetPosition(V* out) { *out = cur; return *out; } };
struct Thing { Phys* p; float GetDistanceTo(const V& v) { return v.X + v.Y + v.Z; } };
struct Kick {
    V KickVector; float KickValue;
    Kick(Thing* creature, Thing* target)
    {
        KickVector = (target->p->cur - target->p->prev) + (creature->p->cur - creature->p->prev);
        { V position; KickValue = creature->GetDistanceTo(creature->p->GetPosition(&position)); }
    }
};
int main()
{
    Phys pc; pc.cur = V(1, 2, 3); pc.prev = V(0, 1, 1);
    Phys pt; pt.cur = V(10, 10, 10); pt.prev = V(8, 9, 7);
    Thing c; c.p = &pc; Thing t; t.p = &pt;
    Kick k(&c, &t);
    if (k.KickVector.X != 3 || k.KickVector.Y != 2 || k.KickVector.Z != 5 || k.KickValue != 6) { std::printf("0087fd20_TEST FAIL\n"); return 1; }
    std::printf("0087fd20_TEST PASS\n");
    return 0;
}
