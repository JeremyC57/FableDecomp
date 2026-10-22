#include <stdio.h>
#include <string.h>
#include <math.h>
// Behaviour copy of CCombatAction_ControlledStrafeJump::Initialise (0x0085BD70): reset the physics set to Up (0,0,1) /
// Forward (OriginalFacing.X, OriginalFacing.Y, 0), read the facing back, clamp the dot with RequiredFacing to [-1,1], sign it
// by the 2D cross product, pick the strafe-jump animation by the signed turn in degrees (BACK beyond 135, LEFT/RIGHT beyond 45,
// else FORWARD; BLOCK_ prefix while blocking), turn the creature by (heading - (turnOffset - angle)) when strafe turning is
// enabled and the player is not z-targeting, run the base Initialise, add creature mode 70 when the creature has the mode
// manager, and set AddedRollMode.
struct Vec3 { float X, Y, Z; };
struct Physics {
    Vec3 up, forward;
    float facingAngle; int facingSets;
    void ResetRHSet(const Vec3& u, const Vec3& f) { up = u; forward = f; }
    Vec3 GetFacing() const { return forward; }
    void SetFacingAngle(float a) { facingAngle = a; ++facingSets; }
};
struct Action {
    Vec3 RequiredFacing, OriginalFacing;
    bool AddedRollMode;
    Physics* physics;
    bool blocking, strafeTurning, zTargeting, hasModes;
    char anim[64];
    int addedMode;
    bool baseInit;
    void Initialise()
    {
        Vec3 up = { 0.0f, 0.0f, 1.0f };
        Vec3 fwd = { OriginalFacing.X, OriginalFacing.Y, 0.0f };
        physics->ResetRHSet(up, fwd);
        Vec3 facing = physics->GetFacing();
        float cosAngle = facing.Y * RequiredFacing.Y;
        cosAngle += facing.Z * RequiredFacing.Z;
        cosAngle += facing.X * RequiredFacing.X;
        if (cosAngle < -1.0f) cosAngle = -1.0f;
        else if (cosAngle > 1.0f) cosAngle = 1.0f;
        long sign = (facing.Y * RequiredFacing.X - facing.X * RequiredFacing.Y < 0.0f) ? -1 : 1;
        float angle = (float)acos(cosAngle) * 0.15915493667125702f;
        angle *= sign;
        float degrees = angle * 360.0f;
        float turnOffset = 0.0f;
        strcpy(anim, "MOVEMENT_STRAFE_JUMP_");
        if (blocking) strcat(anim, "BLOCK_");
        if (degrees < -135.0f) { turnOffset = -0.5f; strcat(anim, "BACK"); }
        else if (degrees < -45.0f) { turnOffset = -0.25f; strcat(anim, "LEFT"); }
        else if (degrees > 135.0f) { turnOffset = 0.5f; strcat(anim, "BACK"); }
        else if (degrees > 45.0f) { turnOffset = 0.25f; strcat(anim, "RIGHT"); }
        else strcat(anim, "FORWARD");
        if (strafeTurning && !zTargeting) {
            float delta = turnOffset - angle;
            float heading = (float)atan2((double)physics->forward.X, (double)physics->forward.Y) * 0.15915493667125702f;
            if (heading < 0.0f || heading >= 1.0f) {
                heading = (float)fmod((double)heading, 1.0);
                if (heading < 0.0f) heading += 1.0f;
            }
            physics->SetFacingAngle(heading - delta);
        }
        baseInit = true;
        if (hasModes) addedMode = 70;
        AddedRollMode = true;
    }
};
static void Reset(Action& a, Physics& p, float rx, float ry)
{
    p.facingAngle = -9.0f; p.facingSets = 0;
    a.physics = &p; a.AddedRollMode = false; a.blocking = false; a.strafeTurning = false; a.zTargeting = false;
    a.hasModes = true; a.addedMode = 0; a.baseInit = false; a.anim[0] = 0;
    a.OriginalFacing.X = 0.0f; a.OriginalFacing.Y = 1.0f; a.OriginalFacing.Z = 0.5f;   // forward along +Y
    a.RequiredFacing.X = rx; a.RequiredFacing.Y = ry; a.RequiredFacing.Z = 0.0f;
}
int main()
{
    Physics p; Action a;
    bool ok = true;
    Reset(a, p, 0.0f, 1.0f);                                   // same direction -> FORWARD
    a.Initialise();
    ok = ok && strcmp(a.anim, "MOVEMENT_STRAFE_JUMP_FORWARD") == 0 && a.AddedRollMode && a.addedMode == 70 && a.baseInit;
    ok = ok && p.forward.Z == 0.0f && p.up.Z == 1.0f && p.facingSets == 0;
    Reset(a, p, 0.0f, -1.0f);                                  // opposite -> 180 degrees -> BACK
    a.blocking = true;
    a.Initialise();
    ok = ok && strcmp(a.anim, "MOVEMENT_STRAFE_JUMP_BLOCK_BACK") == 0;
    Reset(a, p, 1.0f, 0.0f);                                   // cross = 1*1 - 0 > 0 -> +90 -> RIGHT
    a.strafeTurning = true;
    a.Initialise();
    ok = ok && strcmp(a.anim, "MOVEMENT_STRAFE_JUMP_RIGHT") == 0 && p.facingSets == 1;
    ok = ok && fabs(p.facingAngle - 0.0f) < 1e-4f;             // heading 0 - (0.25 - 0.25)
    Reset(a, p, -1.0f, 0.0f);                                  // -90 -> LEFT; z-targeting blocks the turn
    a.strafeTurning = true; a.zTargeting = true; a.hasModes = false;
    a.Initialise();
    ok = ok && strcmp(a.anim, "MOVEMENT_STRAFE_JUMP_LEFT") == 0 && p.facingSets == 0 && a.addedMode == 0;
    if (!ok) { printf("0085bd70_TEST FAIL\n"); return 1; }
    printf("0085bd70_TEST PASS\n");
    return 0;
}
