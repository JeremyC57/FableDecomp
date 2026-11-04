// GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance) -- true when the 3D distance between the points is below distance (squared compare)
// Leaf: ?GFWorld3DDistanceSmallerThan@@YI_NABVC3DVector@@0M@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006614F0, 68 bytes + 12 int3, `__fastcall` (a in ecx, b in edx, distance on the stack,
// `ret 4`). Name: Ego_r.pdb `?GFWorld3DDistanceSmallerThan@@YI_NABVC3DVector@@0M@Z` (name set; the catalog label agrees);
// 8 callers (0x00663CE7, 0x00665F4E, 0x0066B3C4, 0x006A6550, 0x0070D99B, 0x009113DD, 0x00935642, 0x00AEEB2F). No callees.
// Shape: the three deltas as named float locals in X, Y, Z order, then `dx*dx + dy*dy + dz*dz < distance*distance` --
// VC7.1 pushes the later-defined delta first, giving retail's Z, Y, X squares (an inline C3DVector operator- temporary
// gives X, Z, Y here); `fcompp ; test ah,41h ; jne` with `mov eax,1` / `xor eax,eax` for the bool. No pragma.
#include "engine/C3DVector.h"

bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    return dx * dx + dy * dy + dz * dz < distance * distance;
}