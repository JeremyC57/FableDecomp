// 00978790 CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(const CThing* a, const CThing* b) const -- order two things by how well they lie in the comparator's direction, weighted down with distance (anonymous-namespace comparator)
// Leaf: ??RCThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0xda29697a@@QBE_NPBVCThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00978790, 286 bytes, `ret 8`; one of two anonymous-namespace TU copies.
// Identity: Ego_r.pdb / FableWin.pdb ??RCThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0x<tu>@@QBE_NPBVCThing@@0@Z
//   (anonymous namespace, one per TU: ?A0xeaff0e08 / ?A0xffab493a; the comparator of a std sort), with the member
//   ?GetWeightFromParams@CThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0x<tu>@@QBEMMM@Z (expanded here).
// Callees: CThing::GetPos() const @ 0x004C73D0, _ftol2 @ 0x00BFEA70, _CIpow @ 0x00BFEB40 (the `pow` intrinsic). 1.0f 0x0122DED8.
// Layout: PDB CThingCompare_InDirectionOfAndNearestToHeuristicBitch { C2DVector Pos +0; C2DVector Dir +8; } (16 bytes).
// Shape: per thing the 2D offset from Pos, its length (sqrt intrinsic, homed in the dead argument slot), the offset divided by
//   that length (`1.0 / len` then two multiplies), the dot with Dir, and GetWeightFromParams(dot, length) = pow((double)dot,
//   float((long)len / 4 * 2 + 1)) through the math.h float overload; the first weight is spilled.
#include "rebuild_abi.h"
extern "C" double FABLE_CDECL pow(double, double);
extern "C" double FABLE_CDECL sqrt(double);
#ifdef _MSC_VER
#pragma intrinsic(pow, sqrt)
#endif

#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#include "engine/C2DVector.h"
#include "engine/CThing.h"

#include "engine/CThingCompare_InDirectionOfAndNearestToHeuristicBitch.h"

bool CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(const CThing* a, const CThing* b) const
{
        C2DVector offsetA = C2DVector(a->GetPos()) - Pos;
        float distanceA = offsetA.GetMagnitude();
        offsetA *= 1.0f / distanceA;
        float dotA = offsetA.Dot(Dir);
        float weightA = GetWeightFromParams(dotA, distanceA);
        C2DVector offsetB = C2DVector(b->GetPos()) - Pos;
        float distanceB = offsetB.GetMagnitude();
        offsetB *= 1.0f / distanceB;
        float dotB = offsetB.Dot(Dir);
        if (GetWeightFromParams(dotB, distanceB) < weightA)
            return true;
        return false;
    }