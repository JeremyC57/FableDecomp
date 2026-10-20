// Behaviour test for CTCLook::GetTurningAngles (0x00751E80).
#include <cstdio>
#include <math.h>

static float g_epsilon = 1e-4f;
struct C3DVector {
    float X, Y, Z;
    bool operator!=(const C3DVector& o) const
    {
        return (float)fabs(X - o.X) > g_epsilon || (float)fabs(Y - o.Y) > g_epsilon || (float)fabs(Z - o.Z) > g_epsilon;
    }
};
static float AngleDifference(float a, float b) { return b - a; }
static float AngleBetweenPositions(const C3DVector& from, const C3DVector& to) { return (to.X - from.X) * 0.01f; }
inline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
struct CPhysics { float FacingXY; C3DVector Position; float GetFacingAngleXY() { return FacingXY; } };
struct CThing { CPhysics* PhysicsTC; };
struct CAction { bool Blocks; bool BlocksBodyTurning() { return Blocks; } };
struct CThingCreatureBase {
    unsigned char m_flags91;
    CAction* CurrentAction;
    bool Busy;
    bool IsPerformingAnyAction() { return Busy; }
};
template<class T> inline bool IS_THING_ALIVE(T* const& p)
{
    if (p != 0) {
        if (!(p->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCLook {
    CThing* ParentThing;
    CThingCreatureBase* PParentCreature;
    float RecoveryAngleToLookAtXY;
    long BodyTurnState;
    long LookFlags;
    C3DVector PosToLookAt;
    C3DVector DirectionToLookIn;
    bool GetTurningAngles(float* targetAngle, float* turnAngle)
    {
        if (BodyTurnState & 2) {
            if (!(IS_THING_ALIVE(PParentCreature) && PParentCreature->IsPerformingAnyAction() &&
                  PParentCreature->CurrentAction->BlocksBodyTurning()) && !(BodyTurnState & 8)) {
                CPhysics* physics = ParentThing->PhysicsTC;
                float bodyXY = physics->GetFacingAngleXY();
                *targetAngle = RecoveryAngleToLookAtXY;
                if (LookFlags & 0x40) {
                    *targetAngle = WrapAngle((float)atan2(DirectionToLookIn.X, DirectionToLookIn.Y) * 0.15915494f);
                } else if (physics->Position != PosToLookAt) {
                    *targetAngle = AngleBetweenPositions(physics->Position, PosToLookAt);
                }
                *turnAngle = AngleDifference(bodyXY, *targetAngle);
                return true;
            }
        }
        return false;
    }
};

static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }

int main()
{
    CPhysics phys; phys.FacingXY = 0.1f; phys.Position.X = 0; phys.Position.Y = 0; phys.Position.Z = 0;
    CThing thing = { &phys };
    CAction action = { true };
    CThingCreatureBase creature; creature.m_flags91 = 0; creature.CurrentAction = &action; creature.Busy = false;
    CTCLook look; look.ParentThing = &thing; look.PParentCreature = &creature; look.RecoveryAngleToLookAtXY = 0.3f;
    look.BodyTurnState = 2; look.LookFlags = 0;
    look.PosToLookAt.X = 20; look.PosToLookAt.Y = 0; look.PosToLookAt.Z = 0;
    look.DirectionToLookIn.X = 0; look.DirectionToLookIn.Y = -1; look.DirectionToLookIn.Z = 0;
    float target = 0, turn = 0;
    bool ok = true;
    ok = ok && look.GetTurningAngles(&target, &turn) && Near(target, 0.2f) && Near(turn, 0.1f);   // look-at position
    look.PosToLookAt = phys.Position;
    ok = ok && look.GetTurningAngles(&target, &turn) && Near(target, 0.3f) && Near(turn, 0.2f);   // recovery
    look.LookFlags = 0x40;                                   // atan2(0, -1) = pi -> 0.5
    ok = ok && look.GetTurningAngles(&target, &turn) && Near(target, 0.5f);
    look.DirectionToLookIn.X = -1; look.DirectionToLookIn.Y = 0;   // atan2(-1, 0) = -pi/2 -> -0.25 -> wrapped 0.75
    ok = ok && look.GetTurningAngles(&target, &turn) && Near(target, 0.75f);
    creature.Busy = true;                                    // blocking action
    ok = ok && !look.GetTurningAngles(&target, &turn);
    creature.m_flags91 = 1;                                  // dead creature does not block
    ok = ok && look.GetTurningAngles(&target, &turn);
    look.BodyTurnState = 2 | 8;
    ok = ok && !look.GetTurningAngles(&target, &turn);
    look.BodyTurnState = 0;
    ok = ok && !look.GetTurningAngles(&target, &turn);
    if (!ok) { std::printf("00751e80_TEST FAIL\n"); return 1; }
    std::printf("00751e80_TEST PASS\n");
    return 0;
}