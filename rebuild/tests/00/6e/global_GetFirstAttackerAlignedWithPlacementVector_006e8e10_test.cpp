// Behaviour test for 0x006E8E10 (GetFirstAttackerAlignedWithPlacementVector: first attacker within 60 degrees of the
// flattened placement vector, seen from the defender).
#include <cstdio>
#include <cmath>
#include <list>

struct Vec {
    float X, Y, Z;
    Vec(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); if (l > 0.0f) { X /= l; Y /= l; Z /= l; } }
};
inline float Dot(const Vec& a, const Vec& b) { return a.Z * b.Z + (a.X * b.X + a.Y * b.Y); }
struct Creature { Vec Pos; Creature(float x, float y, float z) : Pos(x, y, z) { } const Vec& GetPosition() const { return Pos; } };

static const Creature* GetFirst(const std::list<const Creature*>& attackers, const Vec& defenderPos, const Vec& placement)
{
    float minDot = (float)std::cos(1.0471976115464443);
    for (std::list<const Creature*>::const_iterator it = attackers.begin(); it != attackers.end(); ++it) {
        const Creature* attacker = *it;
        Vec flat(placement.X, placement.Y, 0.0f); flat.Normalise();
        const Vec& p = attacker->GetPosition();
        Vec to(p.X - defenderPos.X, p.Y - defenderPos.Y, 0.0f); to.Normalise();
        if (Dot(to, flat) > minDot) return attacker;
    }
    return 0;
}
#define FAIL { std::printf("006e8e10_TEST FAIL\n"); return 1; }
int main()
{
    Vec defender(10.0f, 10.0f, 5.0f);
    Vec placement(3.0f, 0.0f, 7.0f);                       // +X in the ground plane (Z ignored)
    Creature behind(5.0f, 10.0f, 0.0f), side(10.0f, 20.0f, 0.0f), wide(11.0f, 12.0f, 50.0f), ahead(20.0f, 11.0f, -9.0f);
    std::list<const Creature*> attackers;
    if (GetFirst(attackers, defender, placement) != 0) FAIL;
    attackers.push_back(&behind); attackers.push_back(&side);
    if (GetFirst(attackers, defender, placement) != 0) FAIL;
    attackers.push_back(&wide);                            // ~63 degrees off: rejected
    attackers.push_back(&ahead);                           // ~6 degrees off: accepted
    if (GetFirst(attackers, defender, placement) != &ahead) FAIL;
    attackers.push_front(&ahead);
    if (GetFirst(attackers, defender, placement) != &ahead || attackers.front() != &ahead) FAIL;
    std::printf("006e8e10_TEST PASS\n");
    return 0;
}