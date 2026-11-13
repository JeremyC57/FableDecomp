// CTCPhysicsControlled::PerformImpulseBasedMovement(std::vector<CThing*>& things, C3DVector& pos) -- move a controlled physics thing along its impulse in steps of about 1/14 unit: each step adjusts the velocity for impassable ground, collides with the nearby things (cylinder / impassable / sloped responses, game interactions) and, when nothing was hit, with the landscape, then advances the position; after the planned steps it keeps stepping (at most twice the plan) until the impulse distance is covered, resizing the last step
// Leaf: ?PerformImpulseBasedMovement@CTCPhysicsControlled@@QAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@AAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00725500, 1324 bytes, thiscall `ret 8`. Ego_r.pdb / FableWin.pdb
//   ?PerformImpulseBasedMovement@CTCPhysicsControlled@@QAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@AAVC3DVector@@@Z
//   (public, non-virtual; called by the landed Move 0x00725C40 after AdjustThingListForDebugHelpers).
// Callees: virtual slots of CTCPhysicsBase 59 CanCollideWithLandscape (+0xEC), 90 HasCollidedWithThingFromTo (+0x168), 92
//   HasCollidedWithLandscapeAt (+0x170) on this and 62 IsPhysicsMeshImpassable (+0xF8) on the other thing's physics TC (slot numbers
//   from engine/CTCPhysicsBase.h's declaration order: 28 CTCBase slots, then 81 ApplyControlledImpulse / 87 Move / 89
//   FillPhysicsDebugInfoForSelf agree with CTCPhysicsControlled's vtable 0x0126616C); CTCPhysicsStandard::UpdateVelocityForImpassability
//   0x007243C0 / IsCollisionWithThingToCauseResponse 0x008E5750 / UpdateCollisionGameInteractions 0x00723AC0; CTCPhysicsBase::
//   IsPhysicsMeshTypeCylinder 0x006B0170 / ImpulseRespondToCollisonWithCylinder 0x006AFA40 / ImpulseRespondToCollisonWithImpassableObject
//   0x006AFAF0 / ModifyResponseForNearVerticalSlopes 0x006B0610 / ImpulseRespondToCollision 0x006AF7D0 / UpdateCollisionGroundIneractions
//   0x006AFF00; __ftol2 0x00BFEA70. Globals: DELTA_NOTIONAL_ZERO 0x0129BA3C, GFastOneOverSqrtTable 0x013CE558 (the inline
//   GFFastOneOverSqrt of Normalise); float pool 14.0f, 1.0f, 0.001f, 0.01f, 3.0f, 0.5f, 0.0f, 0.125f.
// Layout: shared headers; CTCPhysicsControlled is a new overlay header (: CTCPhysicsStandard, ImpulseVelocity +0x88), the physics base /
//   standard headers and CPhysicsCollideResponse gain declaration opt-ins (hdr/README.md). C3DVector with
//   FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER (Normalise = GFFastOneOverSqrt(GFSqr(X)+GFSqr(Y)+GFSqr(Z)): its squares are shared
//   with step.GetMagnitude() but the sum is formed again, 0x007258A2..0x00725928).
// Shape (each item a measured lever): the step is a copy of ImpulseVelocity taken after the two magnitudes (so VC7.1 keeps its X / Z on the
//   x87 stack across __ftol2 and copies only Y, 0x00725539 / 0x007255BD) and divided by the clamped count; the response and the step
//   counter are declared after it (their stores fall inside the first magnitude as retail); `collided` is set in both non-cylinder arms
//   (5 references: c2 priority 60 > pos 27, so it takes bl and pos / things are split as retail -- one assignment ahead of the branch
//   gives pos ebx); the thing is a named local (its eax piece spilled to [esp+28h]); old_pos is a function-scope local assigned in the
//   loop (a loop-local copy is scalarised onto the x87 stack); the special-move nudge is `if (!collided && special_move)` after the
//   if / else, which moves the collision-normal test out of line behind the step-count block (0x007259C7, `jmp 0x00725863`); that test is
//   `0.0f - normal.Dot(C3DVector(0, 0, 1)) < 0.125f` (VC7.1 folds it to `fld Y ; fadd X ; fmul 0.0 ; fsub Z`, 0x007259C7 -- a Dot with
//   (0, 0, -1) keeps a `fmul 1.0`).
#define FABLE_RETAIL_CTCPHYSICSCONTROLLED
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_IMPULSE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_IMPASSABLE
#define FABLE_RETAIL_CTCPHYSICSBASE_IMPULSE
#define FABLE_RETAIL_CPHYSICSCOLLIDERESPONSE_CTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CPhysicsCollideResponse.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCPhysicsStandard.h"
#include "engine/CTCPhysicsControlled.h"

void CTCPhysicsControlled::PerformImpulseBasedMovement(std::vector<CThing*>& things, C3DVector& pos)
{
    bool landscape_collision = false;
    if (things.size() == 0 && !CanCollideWithLandscape())
        return;
    C3DVector moved(0.0f, 0.0f, 0.0f);
    C3DVector old_pos;
    float speed = ImpulseVelocity.GetMagnitude();
    float distance = ImpulseVelocity.GetMagnitude();
    C3DVector step = ImpulseVelocity;
    CPhysicsCollideResponse response;
    long i = 0;
    long steps = (long)(speed * 14.0f);
    long max_steps = steps * 2;
    if (steps < 1)
        steps = 1;
    step /= (float)steps;
    bool moving = speed > DELTA_NOTIONAL_ZERO;
    bool can_continue = true;
    bool special_move = ParentThing->HasTC(TCI_ASSASSIN_RUSH) || ParentThing->HasTC(TCI_BATTLE_CHARGE);
    if (!moving)
        return;
    do {
        UpdateVelocityForImpassability(pos, step);
        C3DVector new_pos = pos + step;
        bool collided = false;
        for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
            CThing* thing = *it;
            CTCPhysicsBase* other = thing->GetPhysicsTC();
            if (HasCollidedWithThingFromTo(*other, pos, new_pos, response)) {
                if (IsCollisionWithThingToCauseResponse(*other)) {
                    if (other->IsPhysicsMeshTypeCylinder())
                        collided = ImpulseRespondToCollisonWithCylinder(response, step);
                    else if (other->IsPhysicsMeshImpassable()) {
                        collided = true;
                        ImpulseRespondToCollisonWithImpassableObject(response, *other, step);
                    } else {
                        collided = true;
                        ModifyResponseForNearVerticalSlopes(response);
                        ImpulseRespondToCollision(response, step);
                    }
                }
                UpdateCollisionGameInteractions(*thing, response);
            }
        }
        if (!collided) {
            if (CanCollideWithLandscape()) {
                if (HasCollidedWithLandscapeAt(new_pos, response)) {
                    new_pos = response.CollidePos;
                    ImpulseRespondToCollision(response, step);
                    landscape_collision = true;
                }
                old_pos = pos;
                pos = new_pos;
                moved += pos - old_pos;
            }
        } else if (0.0f - response.CollideNormal.Dot(C3DVector(0.0f, 0.0f, 1.0f)) < 0.125f)
            can_continue = false;
        if (!collided && special_move)
            step.Z -= 0.001f;
        if (++i >= steps) {
            float moved_distance = moved.GetMagnitude();
            float step_length = step.GetMagnitude();
            if (!(moved_distance < distance) || !(step_length > 0.01f))
                break;
            float remaining = distance - moved_distance;
            if (can_continue) {
                if (remaining < step_length) {
                    step.Normalise();
                    step *= remaining + 0.01f;
                }
                ++steps;
            } else
                moving = false;
            if (steps > max_steps)
                break;
        }
    } while (moving);
    if (landscape_collision)
        UpdateCollisionGroundIneractions(pos);
}
