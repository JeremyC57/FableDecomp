// GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance) -- true when the two positions are strictly closer than the distance (squared lengths compared)
// Leaf: ?GFWorld3DDistanceSmallerThan@@YI_NABVC3DVector@@0M@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4B7B0, length 68 bytes (+12 int3 pad), `__fastcall` (a ecx, b edx, the
//   distance on the stack, `ret 4`), bool in eax.
// Name: Ego_r.pdb / FableWin.pdb `?GFWorld3DDistanceSmallerThan@@YI_NABVC3DVector@@0M@Z` (name set).
// Shape: `return (a - b).GetLengthSquared() < distance * distance;` (the difference kept on the x87 stack, VC's own
//   X, Z, Y sum order for X*X + Y*Y + Z*Z, as in the landed vector rows; the float compare materialised as
//   `mov eax,1` / `xor eax,eax` returns).
#include "engine/C3DVector.h"

bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance)
{
    return (a - b).GetSquaredMagnitude() < distance * distance;
}