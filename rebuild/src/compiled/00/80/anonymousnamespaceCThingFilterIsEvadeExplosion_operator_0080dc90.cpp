// `anonymous namespace'::CThingFilter_IsEvadeExplosion::operator()(const CThing& thing) const -- a rolling thing passes when its flattened travel direction since the last frame is within about 25 degrees of the flattened direction from the explosion line (StartPos..EndPos) to it
// Leaf: ??RCThingFilter_IsEvadeExplosion@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080DC90, 240 bytes (+ int3 pad), thiscall `ret 4`, bool in al.
// Name: Ego_r.pdb public ??RCThingFilter_IsEvadeExplosion@?A0x41ace303@@QBE_NABVCThing@@@Z + FableWin.pdb same symbol (debug 0x02770220);
//   const CThing& (not CThing*). Ego_d.pdb type 0x43D13: `anonymous-namespace'::CThingFilter_IsEvadeExplosion, sizeof 8, base
//   std::unary_function<CThing,bool>, members StartPos / EndPos (const C3DVector&, +0 / +4), methods ctor(const C3DVector&,
//   const C3DVector&), IsTravellingAway, operator(). No Ego_d locals record survives (egod_locals: no complete record): the local
//   names are descriptive.
// Callees: NCreatureMode::IsRolling 0x0085EEB0 (Ego_r ?IsRolling@NCreatureMode@@YI_NABVCThing@@@Z), NPhysicsHelpers::
//   GetLineOutFromClosestPointOnFiniteLineToPoint 0x008F5B50 (Ego_r ?GetLineOutFromClosestPointOnFiniteLineToPoint@NPhysicsHelpers@@
//   YI?AVC3DVector@@ABV2@00@Z), C3DVector::AccurateNormalise 0x00A14440, IsTravellingAway 0x0080C140; DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Source: FableWin debug body 0x02770220 (statement order: IsRolling, GetPhysicsTC()->GetPosition() by reference, move = pos -
//   GetLastPosition(), move.Z = 0, GetAccurateMagnitude, `> DELTA_NOTIONAL_ZERO`, move /= magnitude, GetLineOut(StartPos, EndPos, pos),
//   out.Z = 0, IsNonZero, AccurateNormalise, IsTravellingAway(out, move)). Shared headers C3DVector.h (FABLE_RETAIL_C3DVECTOR_ACCURATE_
//   MAGNITUDE; FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ = FableWin ?IsNonZero 0x01B4F100: retail squares out.Y then out.X at
//   0x0080DD1D..0x0080DD35), CThing.h, CTCPhysicsBase.h (GetPosition / FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION). No flags.
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_CCLASS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
extern "C" double __cdecl cos(double);
#pragma intrinsic(cos)
namespace NCreatureMode { bool __fastcall IsRolling(const CThing& thing); }          // 0x0085EEB0
namespace NPhysicsHelpers {
C3DVector __fastcall GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector& start, const C3DVector& end, const C3DVector& point);   // 0x008F5B50
}
namespace {
class CThingFilter_IsEvadeExplosion : public std::unary_function<CThing, bool> {
public:
    CThingFilter_IsEvadeExplosion(const C3DVector& start_pos, const C3DVector& end_pos) : StartPos(start_pos), EndPos(end_pos) {}
    bool IsTravellingAway(const C3DVector& a, const C3DVector& b) const;   // 0x0080C140
    bool operator()(const CThing& thing) const;
private:
    const C3DVector& StartPos;
    const C3DVector& EndPos;
};
// The same-TU IsTravellingAway (retail 0x0080C140, landed; called, not inlined, at 0x0080DD61) defined before operator() as in retail: with
// its body visible VC7.1's mod/ref sees that the call only reads the two vectors, and keeps the travel X on the x87 stack while homing Y
// (0x0080DCBE..0x0080DCC8); declared only, it keeps Y and homes X (DIFFER 245v242, the residue every earlier draft had).
bool CThingFilter_IsEvadeExplosion::IsTravellingAway(const C3DVector& a, const C3DVector& b) const
{
    return b.Dot(a) > cos((double)0.43633232f);   // the double cos: the shared headers bring <math.h>'s float overload, which adds an
                                                  // fxch pair (the landed row 0x0080C140 text has no <math.h>); with the cast this TU's
                                                  // copy also grades RELOCATION_MATCH at 0x0080C140
}
bool CThingFilter_IsEvadeExplosion::operator()(const CThing& thing) const
{
    if (NCreatureMode::IsRolling(thing)) {
        const C3DVector& pos = thing.GetPhysicsTC()->GetPosition();
        C3DVector move = pos - thing.GetPhysicsTC()->GetLastPosition();
        move.Z = 0.0f;
        float magnitude = move.GetAccurateMagnitude();
        if (magnitude > DELTA_NOTIONAL_ZERO) {
            move /= magnitude;
            C3DVector out = NPhysicsHelpers::GetLineOutFromClosestPointOnFiniteLineToPoint(StartPos, EndPos, pos);
            out.Z = 0.0f;
            if (out.IsNonZero()) {
                out.AccurateNormalise();
                if (IsTravellingAway(out, move))
                    return true;
            }
        }
    }
    return false;
}
}
// Emission: the functor's callers (unary_negate<CThingFilter_IsEvadeExplosion>::operator() inside the explosion search) are not in this
// row; a file-scope member pointer keeps the anonymous-namespace operator() emitted.
bool (CThingFilter_IsEvadeExplosion::*keep_0080dc90)(const CThing&) const = &CThingFilter_IsEvadeExplosion::operator();