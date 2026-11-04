// CWorldMap::PointIsWithinLineSegment(const C3DVector*, const C3DVector*, const C3DVector*) -- point inside the segment's X/Y bounding box
// Leaf: PointIsWithinLineSegment
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail 0x0063BAB0, 0x8C bytes (world-map theme/height TU). Mangled
// ?PointIsWithinLineSegment@CWorldMap@@IBE_NPBVC3DVector@@00@Z (protected const __fastcall, `this` unused).
// For X then Y: min(a,b) <= p (`a < b ? a : b`, `test ah,5 ; jp` then `test ah,41h ; jp`) and
// max(a,b) >= p (`a > b ? a : b`, `test ah,41h ; jne` then `test ah,1 ; jne`). No float constants;
// `return a && b && c && d` gives the int-valued `mov eax,1` / `xor eax,eax` exits.
#include "engine/C3DVector.h"

class CWorldMap
{
protected:
    bool PointIsWithinLineSegment(const C3DVector* point, const C3DVector* a, const C3DVector* pos) const;
};

bool CWorldMap::PointIsWithinLineSegment(const C3DVector* point, const C3DVector* a, const C3DVector* pos) const
{
    return (a->X < pos->X ? a->X : pos->X) <= point->X
        && (a->X > pos->X ? a->X : pos->X) >= point->X
        && (a->Y < pos->Y ? a->Y : pos->Y) <= point->Y
        && (a->Y > pos->Y ? a->Y : pos->Y) >= point->Y;
}