// (anonymous namespace)::CThingFilter_IsEvadeExplosion::operator()(const CThing& thing) const -- a rolling thing passes when its flattened travel direction and the flattened direction from the explosion line (StartPos..EndPos) to it satisfy the filter's direction test
// Grade: DIFFER(246v242) -- one extra instruction: ours stores the travel X (fst dword ptr [esp+8]) right after the travel Y spill, retail keeps X only on the x87 stack until the 1/length scale (the frame, the order X / Z=0 / Y, the Y spill and reload, and every other instruction match)
// Leaf: ??RCThingFilter_IsEvadeExplosion@?A
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080DC90, 240 bytes (+16 int3), `ret 4`, bool in al.
// Callees: NCreatureMode::IsRolling(const CThing&) @ 0x0085EEB0 (`__fastcall`), GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector&,
//   const C3DVector&, const C3DVector&) @ 0x008F5B50 (`__fastcall`, hidden result in ecx), C3DVector::AccurateNormalise @ 0x00A14440,
//   IsTravellingAway(const C3DVector&, const C3DVector&) const @ 0x0080C140 (Ego_r.pdb; the manifest
//   "CCompareTargetedPositions::operator()" is propagated). Globals: 1e-4f 0x0129BA3C (an extern const, squared at run time), 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout: PDB CThingFilter_IsEvadeExplosion { C3DVector* StartPos; C3DVector* EndPos; }; the thing's physics component
//   (+0x60) keeps its position at +0xC and the previous frame's at +0x18.
// Shape: the travel vector is (dx, dy, 0) in a frame local; its length (sqrt intrinsic) guards a `1 / len` scale (the z
//   term folds to `inv * 0`); the line-out vector is flattened (z = 0) and normalised only when longer than epsilon.
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
class CTCPhysicsBase { public:
    unsigned char _pad00[0x0c];
    C3DVector Position;                         // +0x0c
    C3DVector PreviousPosition;                 // +0x18
};
class CThing { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
namespace NCreatureMode { bool __fastcall IsRolling(const CThing& thing); }   // 0x0085EEB0
C3DVector __fastcall GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector& start, const C3DVector& end,
                                                                  const C3DVector& point);   // 0x008F5B50
namespace {
class CThingFilter_IsEvadeExplosion { public:
    C3DVector* StartPos;                        // +0x00
    C3DVector* EndPos;                          // +0x04
    bool IsTravellingAway(const C3DVector& lineOut, const C3DVector& travel) const;   // 0x0080C140
    bool operator()(const CThing& thing) const;
};
bool CThingFilter_IsEvadeExplosion::operator()(const CThing& thing) const
{
    if (NCreatureMode::IsRolling(thing)) {
        CTCPhysicsBase* physics = thing.PhysicsTC;
        const C3DVector& pos = physics->Position;
        float dx = pos.X - physics->PreviousPosition.X;
        C3DVector travel;
        travel.Z = 0.0f;
        travel.Y = pos.Y - physics->PreviousPosition.Y;
        travel.X = dx;
        float length = travel.GetMagnitude();
        if (length > DELTA_NOTIONAL_ZERO) {
            travel *= 1.0f / length;
            C3DVector lineOut = GetLineOutFromClosestPointOnFiniteLineToPoint(*StartPos, *EndPos, pos);
            lineOut.Z = 0.0f;
            if (lineOut.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
                lineOut.AccurateNormalise();
                if (IsTravellingAway(lineOut, travel))
                    return true;
            }
        }
    }
    return false;
}
}
