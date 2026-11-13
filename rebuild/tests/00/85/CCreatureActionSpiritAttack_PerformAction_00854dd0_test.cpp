// Behaviour test for CCreatureAction_SpiritAttack::PerformAction (retail 0x00854DD0): a standalone copy of the model.
// Checks: PTarget follows the targeting TC's thing; the hit needs a living creature target within the spirit's melee range plus
// the target's physics radius; the hit is delivered from one unit behind the creature along the unit creature-to-target
// direction with attribute-0 damage 1.0, blockable only; experience unless blocked or negated; OnStrike always.
#include <cstdio>
#include <cmath>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetSquaredMagnitude() const { return Z * Z + Y * Y + X * X; }
    C3DVector& Normalise() { float f = 1.0f / (float)std::sqrt(GetSquaredMagnitude()); X *= f; Y *= f; Z *= f; return *this; }
};

class CHitParameters { public:
    const void* Location; const void* ThingHitting; const void* Weapon;
    C3DVector Position, Direction;
    bool KnockDown, Blockable, Flourish, BlockCounter, Blocked, HitNegated;
    long Attribute; float Damage; int DamageCalls;
    CHitParameters(const void* location, const void* hitting, const void* weapon, const C3DVector& position, const C3DVector& direction,
                   bool knockDown, bool blockable, bool flourish, bool blockCounter)
        : Location(location), ThingHitting(hitting), Weapon(weapon), Position(position), Direction(direction), KnockDown(knockDown),
          Blockable(blockable), Flourish(flourish), BlockCounter(blockCounter), Blocked(false), HitNegated(false), Attribute(-1),
          Damage(0.0f), DamageCalls(0) {}
    void AddDamageWithAttribute(long attribute, float damage) { Attribute = attribute; Damage = damage; ++DamageCalls; }
    bool IsBlocked() const { return Blocked; }
    bool IsHitNegated() const { return HitNegated; }
};

class CThing { public:
    C3DVector Pos; float Radius; bool Dead; unsigned long TypeFlags;
    int Hits; bool BlockNext, NegateNext; CHitParameters* LastHitCopy; CHitParameters Last;
    CThing() : Pos(0, 0, 0), Radius(0), Dead(false), TypeFlags(0xf), Hits(0), BlockNext(false), NegateNext(false), LastHitCopy(0),
               Last(0, 0, 0, C3DVector(0, 0, 0), C3DVector(0, 0, 0), false, false, false, false) {}
    const C3DVector& GetPos() const { return Pos; }
    bool IsDead() const { return Dead; }
    unsigned long GetTypeFlags() const { return TypeFlags; }
    float GetRadius() const { return Radius; }
    bool IsInRange(const C3DVector& pos, float range) const { C3DVector d = Pos - pos; return d.GetSquaredMagnitude() < range * range; }
    void OnHit(CHitParameters& hit) { ++Hits; if (BlockNext) hit.Blocked = true; if (NegateNext) hit.HitNegated = true; Last = hit; }
};

class CTCCoopSpirit { public:
    float MeleeTargetRange; long Experience; int Strikes;
    CTCCoopSpirit() : MeleeTargetRange(0), Experience(0), Strikes(0) {}
    float GetMeleeTargetRange() const { return MeleeTargetRange; }
    void AddScore(long amount) { Experience += amount; }
    void OnStrike() { ++Strikes; }
};

struct CCreature : CThing { CThing* Targeted; CTCCoopSpirit Spirit; CCreature() : Targeted(0) {} };

class CCreatureAction_SpiritAttack { public:
    CCreature* PCreature; CThing* PTarget; int Effects;
    CCreatureAction_SpiritAttack(CCreature* creature) : PCreature(creature), PTarget(0), Effects(0) {}
    void CreateAttackEffect() { ++Effects; }
    void PerformAction()
    {
        CThing* target = PCreature->Targeted;
        PTarget = target;
        if (target && !target->IsDead() && (target->GetTypeFlags() & 8)) {
            CTCCoopSpirit* tc_spirit = &PCreature->Spirit;
            float range = tc_spirit->GetMeleeTargetRange();
            if (target->IsInRange(PCreature->GetPos(), range + target->GetRadius())) {
                CreateAttackEffect();
                C3DVector direction = target->GetPos() - PCreature->GetPos();
                direction.Normalise();
                CHitParameters hit(0, PCreature, 0, PCreature->GetPos() - direction, direction, false, true, false, false);
                hit.AddDamageWithAttribute(0, 1.0f);
                target->OnHit(hit);
                if (!hit.IsBlocked() && !hit.IsHitNegated())
                    tc_spirit->AddScore(1);
            }
        }
        PCreature->Spirit.OnStrike();
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    bool ok = true;
    {   // a living creature target inside range: effect, one hit from one unit behind the creature, experience, strike
        CCreature creature; CThing target;
        creature.Pos = C3DVector(1.0f, 2.0f, 3.0f); creature.Spirit.MeleeTargetRange = 6.0f;
        target.Pos = C3DVector(4.0f, 6.0f, 3.0f); target.Radius = 0.5f;          // offset (3,4,0), distance 5
        creature.Targeted = &target;
        CCreatureAction_SpiritAttack action(&creature);
        action.PerformAction();
        ok = ok && action.PTarget == &target && action.Effects == 1 && target.Hits == 1;
        ok = ok && Near(target.Last.Direction.X, 0.6f) && Near(target.Last.Direction.Y, 0.8f) && Near(target.Last.Direction.Z, 0.0f);
        ok = ok && Near(target.Last.Position.X, 0.4f) && Near(target.Last.Position.Y, 1.2f) && Near(target.Last.Position.Z, 3.0f);
        ok = ok && target.Last.ThingHitting == &creature && target.Last.Location == 0 && target.Last.Weapon == 0;
        ok = ok && !target.Last.KnockDown && target.Last.Blockable && !target.Last.Flourish && !target.Last.BlockCounter;
        ok = ok && target.Last.Attribute == 0 && Near(target.Last.Damage, 1.0f) && target.Last.DamageCalls == 1;
        ok = ok && creature.Spirit.Experience == 1 && creature.Spirit.Strikes == 1;
    }
    {   // the target's physics radius extends the range: distance 5, range 4.8 + radius 0.5 hits, radius 0.1 does not
        CCreature creature; CThing target;
        creature.Pos = C3DVector(0.0f, 0.0f, 0.0f); creature.Spirit.MeleeTargetRange = 4.8f;
        target.Pos = C3DVector(0.0f, 3.0f, 4.0f); target.Radius = 0.5f;
        creature.Targeted = &target;
        CCreatureAction_SpiritAttack action(&creature);
        action.PerformAction();
        ok = ok && target.Hits == 1 && action.Effects == 1 && creature.Spirit.Experience == 1;
        ok = ok && Near(target.Last.Direction.Y, 0.6f) && Near(target.Last.Direction.Z, 0.8f) && Near(target.Last.Position.Z, -0.8f);
        target.Radius = 0.1f;
        action.PerformAction();
        ok = ok && target.Hits == 1 && action.Effects == 1 && creature.Spirit.Experience == 1 && creature.Spirit.Strikes == 2;
        ok = ok && action.PTarget == &target;
    }
    {   // blocked and negated hits land but give no experience
        CCreature creature; CThing target;
        creature.Spirit.MeleeTargetRange = 10.0f; target.Pos = C3DVector(2.0f, 0.0f, 0.0f); creature.Targeted = &target;
        CCreatureAction_SpiritAttack action(&creature);
        target.BlockNext = true;
        action.PerformAction();
        ok = ok && target.Hits == 1 && creature.Spirit.Experience == 0 && creature.Spirit.Strikes == 1;
        target.BlockNext = false; target.NegateNext = true;
        action.PerformAction();
        ok = ok && target.Hits == 2 && creature.Spirit.Experience == 0 && creature.Spirit.Strikes == 2;
        target.NegateNext = false;
        action.PerformAction();
        ok = ok && target.Hits == 3 && creature.Spirit.Experience == 1 && creature.Spirit.Strikes == 3 && action.Effects == 3;
    }
    {   // dead targets, non-creatures and no target at all: only the retarget and the strike
        CCreature creature; CThing target;
        creature.Spirit.MeleeTargetRange = 10.0f; target.Pos = C3DVector(1.0f, 0.0f, 0.0f); creature.Targeted = &target;
        CCreatureAction_SpiritAttack action(&creature);
        target.Dead = true;
        action.PerformAction();
        ok = ok && target.Hits == 0 && action.Effects == 0 && action.PTarget == &target && creature.Spirit.Strikes == 1;
        target.Dead = false; target.TypeFlags = 7;
        action.PerformAction();
        ok = ok && target.Hits == 0 && action.Effects == 0 && creature.Spirit.Strikes == 2;
        creature.Targeted = 0;
        action.PerformAction();
        ok = ok && action.PTarget == 0 && action.Effects == 0 && creature.Spirit.Strikes == 3 && creature.Spirit.Experience == 0;
    }
    if (!ok) { std::printf("00854dd0_TEST FAIL\n"); return 1; }
    std::printf("00854dd0_TEST PASS\n");
    return 0;
}