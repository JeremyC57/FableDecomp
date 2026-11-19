// CIsVeryCloseMotionlessCreature::operator()(const CThingCreatureBase& creature) const -- a creature of category 2 whose kind-2 component shows no movement since the last frame (squared step <= 1e-4 squared) and whose ground distance from MyPos is under 1.4 times the sum of its physics radius and MyRadius
// Leaf: ??RCIsVeryCloseMotionlessCreature@@QBE_NABVCThingCreatureBase@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00942270, 210 bytes, `ret 4`.
// Callees: the creature category getter @ 0x00662000 (def +0xe0 -> +0xc4; landed callers name it), the const
//   component-map LowerBound @ 0x004365B0, CThing::GetPosition @ 0x006A4D00 (GOTCHAS chunk 182). Globals: 0.0001f @
//   0x0129BA3C (an extern float, squared), the 1.4f pool constant @ 0x01292874.
// Layout: Ego_r.pdb CIsVeryCloseMotionlessCreature {MyPos +0, MyRadius +0x0c}; retail CThing component bitset +0x20
//   (kind 2 = bit 2), map +0x44, physics body +0x60 (radius +0x34); the kind-2 component keeps this frame's position at
//   +0x0c and the previous one at +0x18.
// Shape: the component comes from an accessor that leaves it unset when absent (retail reads the dead argument slot);
//   the step is `(pos - lastPos).GetLengthSquared()` with the landed C3DVector inline operators; the distance is the
//   square root of the 2D sum; the step test is `!(step > eps*eps)` (`test ah,5 ; jnp`, NaN passes); both tests are
//   nested ifs sharing the `xor al,al` exit.
// Port (NF98, round 8, q11293): the local CThing / CThingPhysical / CVectorMap / CTCMotion / CPhysicsBody / C2DVector stand-ins replaced by
//   the shared headers: the functor is engine/CIsVeryCloseMotionlessCreature.h (Ego_r.pdb 0x009660C0 / FableWin.pdb 0x02BA0D70
//   ??RCIsVeryCloseMotionlessCreature@@QBE_NABVCThingCreatureBase@@@Z: own leaf respelled from the CThing* stand-in), the category getter is
//   ?GetCreatureType@CThingCreatureBase@@QBE?AW4ECreatureType@@XZ (0x00662000, FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE; 2 = HUMAN_ADULT),
//   the kind-2 component is PeekTC<const CTCPhysicsBase> (TCI_PHYSICS; Position +0x0C, OldPosition +0x18 = GetLastPosition), the radius is
//   GetPhysicsTC()->Radius (+0x60 -> +0x34), the position CThingPhysical::PeekPos 0x006A4D00.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_STATTYPE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
class CThingAICreature;
#include "engine/CIsVeryCloseMotionlessCreature.h"

bool CIsVeryCloseMotionlessCreature::operator()(const CThingCreatureBase& creature) const
{
    if (creature.GetCreatureType() == HUMAN_ADULT) {
        const CTCPhysicsBase* physics;
        creature.PeekTC(&physics);
        if (!((physics->GetPosition() - physics->GetLastPosition()).GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)) {
            const C3DVector& pos = creature.PeekPos();
            if (C2DVector(pos.X - MyPos.X, pos.Y - MyPos.Y).GetMagnitude() < (creature.GetPhysicsTC()->GetRadius() + MyRadius) * 1.4f)
                return true;
        }
    }
    return false;
}
