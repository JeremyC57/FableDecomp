// CBezierCurve::GetPointOnCurve(float t) const -- evaluate the cubic Bezier at parameter t:
//     Point1 (1-t)^3 + 3 Point2 t (1-t)^2 + 3 Point3 t^2 (1-t) + Point4 t^3.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009122A0, length 390 bytes (__fastcall, `ret 8`: hidden C3DVector result
//          pointer + t). A header-inline member emitted out of line ahead of the CAIStateGroup_FlyingThingBase TU; its only
//          caller is CAIStateGroup_FlyingThingBase::AIAction_RandomFlyToPos (call site 0x009132F2).
// Name:   ?GetPointOnCurve@CBezierCurve@@QBE?AVC3DVector@@M@Z in both Ego_r.pdb and FableWin.pdb.
// Callees: _CIpow @ 0x00BFEB40 (MSVCR71 import, x2: t^3 and (1-t)^3 through math.h's float pow(float, float) overload, which
//          widens both operands, hence the double 3.0 @ 0x012368F0); constants 3.0f @ 0x0122DED4 and 1.0f @ 0x0122DED8.
//          The C3DVector operators and GFSqr are inline (no code of their own).
// Layout: CBezierCurve (Ego_r.pdb, 0x30 bytes): Point1 +0x00, Point2 +0x0c, Point3 +0x18, Point4 +0x24, all C3DVector.
// Shape:  one left-associated sum of four C3DVector terms, so the right-most (Point4) term is evaluated first.
//          * The squares go through the engine's inline GFSqr(float): for GFSqr(1.0f - t) the argument is evaluated before
//            the object expression Point2 * 3.0f * t, which is why retail loads (1-t) onto the x87 stack before the Point2
//            loads (0x00912337) and only squares it afterwards (0x00912374), spilling the square to the dead t argument
//            slot; a written-out (1-t)*(1-t) is squared at its point of use instead (358 v 390 bytes).
//          * The sum is held in a named local and returned by value: the copy into the hidden result leaves the four
//            extra component spills of the final additions (0x009123DC..0x00912412); `return <expression>;` builds the
//            result in place and is 16 bytes shorter.
//          * (1 - t) is a common subexpression kept in the frame slot that first held t*t.
#define FABLE_RETAIL_CBEZIERCURVE_GETPOINTONCURVE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#include "engine/CBezierCurve.h"
extern "C" double __cdecl pow(double x, double y);     // _CIpow 0x00BFEB40
// Leaf: ?GetPointOnCurve@CBezierCurve@@QBE?AVC3DVector@@M@Z
C3DVector CBezierCurve::GetPointOnCurve(float t) const
{
    C3DVector result = Point1 * pow(1.0f - t, 3.0f)
                     + Point2 * 3.0f * t * GFSqr(1.0f - t)
                     + Point3 * 3.0f * GFSqr(t) * (1.0f - t)
                     + Point4 * pow(t, 3.0f);
    return result;
}