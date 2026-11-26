// Mass::ForceToVel(C2DVector pos, C2DVector force) -- the velocity this force would add at pos.
// Leaf: ?ForceToVel@Mass@@QAE?AVC2DVector@@V2@0@Z
// Identity: the Mass.h declaration and the landed siblings GetVel 0x008EEA30 / ApplyImpulse 0x008EEA80.
// Retail Fable.exe 0x008EEB40. The grouped Dot (FABLE_RETAIL_C2DVECTOR_DOT_GROUP) is the spelling the
// 2026-11-18 Collide candidate graded MATCH for this body.
#include "fable_compiler.h"
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_DOT_GROUP
#include "engine/Mass.h"
inline C2DVector __fastcall Right(C2DVector c) { return C2DVector(c.Y, -c.X); }
C2DVector Mass::ForceToVel(C2DVector pos, C2DVector force)
{
    return force * IMass + Right(pos - Pos) * Right(pos - Pos).Dot(force) * AIMass;
}
