// CTCCrateStack::FillStack(long height, const C3DVector& marker_pos, const C3DVector& marker_dir) -- for a non-empty stack, create the first crate object (the stack's crate def, owned by the main player) at the marker, facing the marker direction turned half round with a random twist of up to +/- 1/16 turn, and remember it; for a stack of two, create the second crate at the parent thing, place it one crate height above the marker with its own twist, stop its physics moving and remember it
// Leaf: ?FillStack@CTCCrateStack@@QAEXJABVC3DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) 0x007B6810, 615 bytes, thiscall `ret 0Ch`. Grade: RELOCATION_MATCH (e05 egod lane).
// Names: Ego_r.pdb ?FillStack@CTCCrateStack@@QAEXJABVC3DVector@@0@Z; Ego_d tc_crate_stack.obj parameters (height, marker_pos,
//   marker_dir) and locals verbatim: function scope random_angle_adder, player_number, RANDOM_ANGLE_VARIATION_RANGE (const float),
//   tc_physics, crate (CThingObject*) -- hence the early `if (!height) return;` -- and the height-2 block crate_random_angle_adder,
//   crate_height, t_pos (both the early-return and the if-block forms match; the Ego_d scopes decide).
// Callees: CTCBase::PeekPlayerManager 0x00686D50, CPlayerManager::GetMainPlayerNumber 0x00449700, CThingObject::Create 0x00703210,
//   CTCBase::GetPWorldSeed 0x00686DA0, GFFloatRandom 0x004978A0 (out of line), fmod 0x00BFEB28 (C3DVector::GetAngleXY's wrap),
//   CTCPhysicsBase slots 66 (+0x108) SetFacingAngleXY / 30 (+0x78) SetPosition, CTCPhysicsBase::SetAsMoving 0x006B0130,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (FirstCrate = crate).
// Shape: the twist is `adder = GFFloatRandom(RANGE, seed); adder -= RANGE / 2;` (FITTED: the one-expression subtraction schedules the
//   fsub ahead of the marker_dir load, 0x007B6884 / 0x007B6888). The old draft's "parameter-home slot reuse" residue was a local-model
//   artefact: gone at the first compile on the shared headers.
// f: the writable 0.6f at 0x0138366C (only reference 0x007B697E) is the block-scope `static float f` of the height-2 block: Ego_d
//   tc_crate_stack.obj FillStack holds an S_LDATA32 `f` (type float, not const) between crate_random_angle_adder and crate_height
//   (the only S_LDATA32 named f in Ego_d); retail keeps it in writable .data with this single reference. VC7.1 mangles it
//   ?f@?6??FillStack@CTCCrateStack@@QAEXJABVC3DVector@@0@Z@4MA. (Name from one PDB source plus the retail data shape; EgoCore not
//   available on this machine.)
// Headers: hdr opt-in FABLE_RETAIL_CTCCRATESTACK_FILLSTACK (FillStack and the three data members); existing FABLE_RETAIL_CTHINGOBJECT_CREATE,
//   FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER / _WORLDSEED, FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN, FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES,
//   FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CTCCRATESTACK
#define FABLE_RETAIL_CTCCRATESTACK_FILLSTACK
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPlayerManager.h"
#include "engine/GFFloatRandom.h"
#include "engine/CTCCrateStack.h"

void CTCCrateStack::FillStack(long height, const C3DVector& marker_pos, const C3DVector& marker_dir)
{
    if (!height)
        return;
    long player_number = PeekPlayerManager().GetMainPlayerNumber();
    CThingObject* crate = CThingObject::Create(CrateDefIndex, marker_pos, player_number, 0, 0, "");
    CTCPhysicsBase* tc_physics = crate->GetPhysicsTC();
    const float RANDOM_ANGLE_VARIATION_RANGE = 0.125f;
    float random_angle_adder = GFFloatRandom(RANDOM_ANGLE_VARIATION_RANGE, GetPWorldSeed());
    random_angle_adder -= RANDOM_ANGLE_VARIATION_RANGE / 2.0f;
    tc_physics->SetFacingAngleXY(marker_dir.GetAngleXY() + random_angle_adder + 0.5f);
    FirstCrate = crate;
    if (height == 2) {
        crate = CThingObject::Create(CrateDefIndex, GetParentThing().GetPos(), player_number, 0, 0, "");
        tc_physics = crate->GetPhysicsTC();
        static float f = 0.6f;                                           // 0x0138366C (Ego_d S_LDATA32 f, float, this block)
        float crate_height = f;
        C3DVector t_pos(marker_pos.X, marker_pos.Y, marker_pos.Z + crate_height);
        tc_physics->SetPosition(t_pos);
        float crate_random_angle_adder = GFFloatRandom(RANDOM_ANGLE_VARIATION_RANGE, GetPWorldSeed());
        crate_random_angle_adder -= RANDOM_ANGLE_VARIATION_RANGE / 2.0f;
        tc_physics->SetFacingAngleXY(marker_dir.GetAngleXY() + crate_random_angle_adder + 0.5f);
        tc_physics->SetAsMoving(false);
        SecondCrate = crate;
    }
}
