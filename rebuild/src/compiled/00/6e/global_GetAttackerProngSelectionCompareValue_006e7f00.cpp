// 006e7f00 GetAttackerProngSelectionCompareValue
// Leaf: ?GetAttackerProngSelectionCompareValue@?A0x
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags:
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
#pragma pack(push,1)
#include "engine/C3DVector.h"
#pragma pack(pop)
namespace {
float __fastcall GetAttackerProngSelectionCompareValue(const C3DVector& attackerPos, const C3DVector& defenderPos,
                                                       const C3DVector& prongVector, const C3DVector& placementVector)
{
    C3DVector flatPlacement(placementVector.X, placementVector.Y, 0.0f);
    C3DVector flatProng(prongVector.X, prongVector.Y, 0.0f);
    float lengthSquared = flatPlacement.X * flatPlacement.X + flatPlacement.Y * flatPlacement.Y + flatPlacement.Z * flatPlacement.Z;
    if (lengthSquared < 0.0001f)
        return 0.0001f;
    float inverse = 1.0f / (float)sqrt(lengthSquared);
    flatPlacement.X = inverse * flatPlacement.X;
    flatPlacement.Y = inverse * flatPlacement.Y;
    flatPlacement.Z = inverse * flatPlacement.Z;
    lengthSquared = flatProng.X * flatProng.X + flatProng.Y * flatProng.Y + flatProng.Z * flatProng.Z;
    if (lengthSquared < 0.0001f)
        return 0.0001f;
    inverse = 1.0f / (float)sqrt(lengthSquared);
    flatProng.X = inverse * flatProng.X;
    flatProng.Y = inverse * flatProng.Y;
    flatProng.Z = inverse * flatProng.Z;
    return flatProng.Z * flatPlacement.Z + (flatProng.X * flatPlacement.X + flatProng.Y * flatPlacement.Y) + 1.0f;
}
}
// Translation-unit retention pointer; an implementation-only test/build artifact.
float (__fastcall *implementation_entry_006e7f00)(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&) = GetAttackerProngSelectionCompareValue;