// CTCPhysicsBase::GetPosition(float interpolate) const -- the interpolated position OldPosition + (Position - OldPosition) * interpolate, returned by value
// Leaf: ?GetPosition@CTCPhysicsBase@@QBE?AVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D150, length 84 bytes; 7 callers (0x0049DB74, 0x00663E12, 0x006A1B50, ...).
// Name (agentNF3, 2026-11-10): Ego_r.pdb ?GetPosition@CTCPhysicsBase@@QBE?AVC3DVector@@M@Z (the interpolated GetPosition overload,
//   const; engine/CTCPhysicsBase.h FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION declares it, and its _BODY opt-in carries this
//   same expression for the TUs that expand it: 0x007EA2F0). Was landed as the descriptive non-const GetInterpolatedPosition on a
//   local layout; the chunk label "CTCPhysicsStandard::GetFacingVector" is a propagated BSim name.
// Layout: engine/CTCPhysicsBase.h retail branch: Position +0x0C, OldPosition +0x18.
// Callees: none (all operators inline).
// Shape: three inline C3DVector operators (each `return C3DVector(x, y, z)` through a value constructor) on a 0x18-byte frame:
// `fld b ; fsub a` x3 (the difference temporary, Z spilled), `fmul t` per component (the scaled temporary, X/Y spilled), then
// `fld scaled ; fadd a` x3 straight into the hidden result (operator+ with the scaled vector on the LEFT: `(b - a) * t + a`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"

C3DVector CTCPhysicsBase::GetPosition(float interpolate) const
{
    return (Position - OldPosition) * interpolate + OldPosition;
}
