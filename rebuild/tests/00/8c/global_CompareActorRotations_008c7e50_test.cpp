#include <cstdio>
#include <cmath>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#endif
// Model of CompareActorRotations(creature, target) @ 0x008C7E50: the target's XY bearing from the creature as a 0..1
// turn fraction (atan2(dx, dy) / 2pi, wrapped), compared with the body's facing through AngleDifference; true when
// more than 70 degrees apart.
struct C3DVector { float X, Y, Z; };
class CPhysicsBody {
public:
    float Facing;
    virtual ~CPhysicsBody() { }
    virtual float GetFacingAngleXY() { return Facing; }
};
class CThing {
public:
    C3DVector     Position;
    CPhysicsBody* m_pBody;
    const C3DVector& GetPosition() const { return Position; }
    CPhysicsBody* GetBody() const { return m_pBody; }
};
class CThingCreatureBase : public CThing { };
static float __stdcall AngleDifference(float from, float to)   // stand-in for 0x00A441E0: signed shortest turn
{
    float d = to - from;
    while (d >= 0.5f) d -= 1.0f;
    while (d < -0.5f) d += 1.0f;
    return d;
}
static const float INV_TWO_PI = 0.15915494f;
static const float MAX_FACING_DIFFERENCE = 0.19444445f;
inline float Fabsf(float x) { return (float)fabs(x); }
inline float WrapTurnFraction(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
bool __fastcall CompareActorRotations(CThingCreatureBase* creature, CThing* target)
{
    const C3DVector& from = creature->GetPosition();
    const C3DVector& to = target->GetPosition();
    CPhysicsBody* body = creature->GetBody();
    float angle = (float)atan2(to.X - from.X, to.Y - from.Y) * INV_TWO_PI;
    angle = WrapTurnFraction(angle);
    float difference = AngleDifference(body->GetFacingAngleXY(), angle);
    if (Fabsf(difference) > MAX_FACING_DIFFERENCE)
        return true;
    return false;
}
int main()
{
    bool ok = true;
    CPhysicsBody body;
    body.Facing = 0.0f;                                 // facing +Y
    CThingCreatureBase creature;
    creature.Position.X = 1.0f; creature.Position.Y = 1.0f; creature.Position.Z = 0.0f;
    creature.m_pBody = &body;
    CThing target;
    target.m_pBody = 0;
    target.Position.Z = 5.0f;
    target.Position.X = 1.0f; target.Position.Y = 11.0f;       // dead ahead
    if (CompareActorRotations(&creature, &target)) ok = false;
    target.Position.X = 0.0f; target.Position.Y = 11.0f;       // slightly left: bearing wraps to ~0.984
    if (CompareActorRotations(&creature, &target)) ok = false;
    target.Position.X = 11.0f; target.Position.Y = 1.0f;       // 90 degrees right
    if (!CompareActorRotations(&creature, &target)) ok = false;
    target.Position.X = 1.0f; target.Position.Y = -9.0f;       // behind
    if (!CompareActorRotations(&creature, &target)) ok = false;
    body.Facing = 0.25f;                                       // now facing +X: the right-hand target is ahead
    target.Position.X = 11.0f; target.Position.Y = 2.0f;
    if (CompareActorRotations(&creature, &target)) ok = false;
    target.Position.X = 6.0f; target.Position.Y = 11.0f;       // atan2(5, 10) = 26.6 deg: 63 deg off, inside 70
    if (CompareActorRotations(&creature, &target)) ok = false;
    target.Position.X = 3.0f; target.Position.Y = 11.0f;       // atan2(2, 10) = 11.3 deg: 79 deg off
    if (!CompareActorRotations(&creature, &target)) ok = false;
    if (!ok) { std::printf("008c7e50_TEST FAIL\n"); return 1; }
    std::printf("008c7e50_TEST PASS\n");
    return 0;
}