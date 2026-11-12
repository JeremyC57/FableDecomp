// CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature::operator()(CThing* thing) const -- the second for_each functor of
//   CTCPhysicsControlled::UpdateAdditionalImpulse: when the player stands inside a boss creature the hero cannot push (solid to each
//   other, not pushable by the hero, TCI_BOSS, overlapping vertically and in XY, both colliding with things), push the player back
//   along the reverse of its facing vector, turned by a random angle of up to 30 degrees whose sign alternates with the world frame.
// Leaf: ??RCApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature@@QBEXPAVCThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00726160, 469 bytes (`ret 4`, no frame pointer). Name: FableWin.pdb
//   ??RCApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature@@QBEXPAVCThing@@@Z + the Ego_r.pdb _For_each instantiation
//   over this functor (0x00726660, the sole caller) and its 4-byte PDB layout (CTCPhysicsBase* Player); the manifest label
//   CApplyAdditionalImpulseIfInCombatAndTooClose (a 24-byte functor, 0x00727220) is a propagated mislabel.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound @ 0x0040F020 (twice, through the forced-inline CThing::GetTC<T>),
//   CTCSolidToThing::IsSolidToThing(const CThing*) const @ 0x00770DE0 (twice), CThing::GetPos() const @ 0x004C73D0,
//   CWorld::GetFrame() @ 0x0049D870, CWorld::GetWorldSeed() const @ 0x0049D810, GFFloatRandom(float, unsigned long*) @ 0x004978A0,
//   C3DVector::RotateXY(float) @ 0x00A14040; virtuals of CTCPhysicsBase: IsPushableByHero() const (slot 83, +0x14C),
//   CanCollideWithThings() const (slot 57, +0xE4, twice), GetFacingVector() const (slot 70, +0x118, C3DVector by hidden pointer),
//   ApplyControlledImpulse(const C3DVector&) (slot 81, +0x144). Globals: CMainGameComponent::PMainGameComponent 0x013B86A0
//   (PWorld +0x24), float 1/360 @ 0x01238E00.
// Shape: both solid-to-thing tests are CThing::GetTC<CTCSolidToThing>(&solid) (presence bit of TCI_SOLID_TO_THING, `test ch,20h`
//   on the +0x28 bitset word, then Components.Find) into ONE address-taken pointer local; that out-pointer form is what makes
//   VC7.1 load the component into ecx before it re-reads Player->ParentThing for the second test and, further down, read
//   Player (ecx) before thing->PhysicsTC (edx) for the radius sum -- with a reference local for the component both orders flip.
//   The key temporary 0x4D, `radii` and `sign` all live in the dead argument slot [esp+28h]. The XY offset is a C2DVector (only
//   two floats are stored); the Z difference is consumed in place by fabs. The squared XY distance is compared with the radius
//   sum itself (not its square), as retail does. The player position reference is taken before the GetPos call (esi = Player+0Ch).
//   The 30.0f range is the private inline GetPushRotationExtentDegrees() (FableWin.pdb ?GetPushRotationExtentDegrees@...@@ABEMXZ).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCSOLIDTOTHING_ISSOLIDTOTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(fabs, sqrt)
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"

class CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature {
public:
    CTCPhysicsBase* Player;                                  // +0x00 (Ego_r.pdb layout: the only member)
    void operator()(CThing* thing) const;
private:
    float GetPushRotationExtentDegrees() const { return 30.0f; }   // FableWin.pdb ?GetPushRotationExtentDegrees@...@@ABEMXZ (inline in retail: push 41F00000h)
};

void CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature::operator()(CThing* thing) const
{
    CTCSolidToThing* solid;
    if (Player->GetParentThing().GetTC(&solid) && !solid->IsSolidToThing(thing))
        return;
    if (thing->GetTC(&solid) && !solid->IsSolidToThing(&Player->GetParentThing()))
        return;
    if (thing->GetPhysicsTC()->IsPushableByHero())
        return;
    if (!thing->HasTC(TCI_BOSS))
        return;
    const C3DVector& player_pos = Player->GetPosition();
    const C3DVector& pos = thing->GetPos();
    C2DVector delta(pos.X - player_pos.X, pos.Y - player_pos.Y);
    float radii = Player->GetRadius() + thing->GetPhysicsTC()->GetRadius();
    if (fabs(pos.Z - player_pos.Z) < radii) {
        if (Player->CanCollideWithThings() && thing->GetPhysicsTC()->CanCollideWithThings()) {
            if (delta.X * delta.X + delta.Y * delta.Y < radii) {
                C3DVector push = -Player->GetFacingVector();
                const CWorld& world = CMainGameComponent::Get()->PeekWorld();
                float sign = (CWorld::GetFrame() % 2 == 0) ? -1.0f : 1.0f;
                float angle = GFFloatRandom(GetPushRotationExtentDegrees(), world.GetWorldSeed()) * sign;
                float turns = angle * (1.0f / 360.0f);
                push.RotateXY(turns * sign);
                Player->ApplyControlledImpulse(push);
            }
        }
    }
}