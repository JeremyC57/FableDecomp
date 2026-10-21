// Behaviour test for CCombatManager::IsTargetOverriddenByAttemptedFacingDirection (0x006EA5F0): true when the target
// is outside the cone (default 60 degrees, or the controlling def's override) around the attempted facing.
#include <cstdio>
#include <cmath>

static float g_Epsilon = 1e-4f;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    void Normalise() { float l = std::sqrt(GetLengthSquared()); X /= l; Y /= l; Z /= l; }
};
static float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CThing { C3DVector Pos; bool HasOverride; float OverrideAngle; CThing* Carried; bool Dead; const C3DVector& GetPosition() const { return Pos; } };
struct CCombatManager {
    bool IsTargetOverriddenByAttemptedFacingDirection(const CThing& creature, const CThing& target, const C3DVector& facing) const
    {
        C3DVector targetPosition = target.GetPosition();
        C3DVector direction = targetPosition - creature.GetPosition();
        if (direction.GetLengthSquared() > g_Epsilon * g_Epsilon) {
            direction.Normalise();
            const CThing* controller = &creature;
            if (creature.Carried && !creature.Carried->Dead)
                controller = creature.Carried;
            float angle = 60.0f;
            bool found = controller->HasOverride;
            if (found)
                angle = controller->OverrideAngle;
            if (Dot(direction, facing) < std::cos(angle * (1.0f / 360.0f) * 6.2831854820251465))
                return true;
        }
        return false;
    }
};

int main()
{
    CThing me = { C3DVector(0, 0, 0), false, 0.0f, 0, false }, foe = { C3DVector(1, 1, 0), false, 0.0f, 0, false };
    CCombatManager m;
    C3DVector facingX(1, 0, 0), facingY(0, 1, 0);
    bool ok = !m.IsTargetOverriddenByAttemptedFacingDirection(me, foe, facingX);      // 45 degrees off: inside 60
    foe.Pos = C3DVector(-1, 1, 0);
    ok = ok && m.IsTargetOverriddenByAttemptedFacingDirection(me, foe, facingX);       // 135 degrees: outside
    me.HasOverride = true; me.OverrideAngle = 170.0f;
    ok = ok && !m.IsTargetOverriddenByAttemptedFacingDirection(me, foe, facingX);      // wide cone
    CThing horse = { C3DVector(0, 0, 0), true, 10.0f, 0, false }; me.Carried = &horse;
    foe.Pos = C3DVector(1, 1, 0);
    ok = ok && m.IsTargetOverriddenByAttemptedFacingDirection(me, foe, facingY);       // 45 off, 10-degree cone
    foe.Pos = me.Pos;
    ok = ok && !m.IsTargetOverriddenByAttemptedFacingDirection(me, foe, facingY);      // degenerate
    if (!ok) { std::printf("006ea5f0_TEST FAIL\n"); return 1; }
    std::printf("006ea5f0_TEST PASS\n");
    return 0;
}
