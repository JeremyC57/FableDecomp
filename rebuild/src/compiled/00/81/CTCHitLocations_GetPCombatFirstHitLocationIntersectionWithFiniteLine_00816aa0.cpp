// CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector&, const C3DVector&, C3DVector&, C3DVector&, float) const -- the const overload: the same query through the non-const one
// Leaf: ?GetPCombatFirstHitLocationIntersectionWithFiniteLine@CTCHitLocations@@QBEPBVCHitLocation@NHitLocations@@ABVC3DVector@@0AAV4@1M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00816AA0, 5 bytes: `jmp 0x008168B0` (the non-const QAE overload; this and the five stack arguments
//   are passed through unchanged). Ego_r.pdb orders the two the same way (QAE 0x0080F130, QBE 0x0080F330).
// NF123 (2026-11-20, q4687): re-spelled from the extern "C" stand-in @FableThunk_816aa0@4 -> @FableImpl_816aa0@4 to Ego_r.pdb 0x0080F330 +
//   FableWin.pdb 0x024E6820 ?GetPCombatFirstHitLocationIntersectionWithFiniteLine@CTCHitLocations@@QBEPBVCHitLocation@NHitLocations@@ABVC3DVector@@0AAV4@1M@Z
//   on the shared engine/CTCHitLocations.h; the landed caller 0x008771A0 binds it, and the jump now binds the PDB non-const spelling of
//   0x008168B0 (the landed 0x007BFE30 binds it).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCHitLocations.h"

const NHitLocations::CHitLocation* CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector& start,
    const C3DVector& end, C3DVector& intersection, C3DVector& normal, float radius) const
{
    return const_cast<CTCHitLocations*>(this)->GetPCombatFirstHitLocationIntersectionWithFiniteLine(start, end, intersection, normal, radius);
}
