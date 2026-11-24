// CWorldMap::PointIsWithinCellBR(const C3DVector*, const C3DVector*) -- point lies in the cell's bottom-right triangle
// Leaf: ?PointIsWithinCellBR@CWorldMap@@IBE_NPBVC3DVector@@0@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail 0x000063ba30, 0x79 bytes (world-map theme/height TU). Mangled ?PointIsWithinCellBR@CWorldMap@@IBE_NPBVC3DVector@@0@Z.
// Same shape as PointIsWithinCellBL (0x0063B920): __ftol2 (0x00BFEA70) truncation of the point's X/Y must equal the
// cell corner (truncated X compared on the left so VC7.1 stores and reloads it from the dead first-parameter
// slot), then the fractional parts fx, fy are tested: fx >= fy (`test ah,41h ; jp` on dy vs dx). `return a && b && c` gives the int-valued exits.
// Name (NF345, 2026-11-24): the four cell rows were named from the triangle geometry; the PDB-order evidence settles them:
//   ego_r.exe/Ego_r.pdb FindGroundCollisionPointWithinCell and FindThemeCollisionPointWithinCell call TR, BL, TL, BR in that order, and the
//   retail callers 0x0063D0A0 / 0x0063D4A0 call 0x0063B890, 0x0063B920, 0x0063B9B0, 0x0063BA30 in that order; Ego_r.pdb's object order is
//   TR < BL < TL < BR (sizes 0x90, 0x90, 0x80, 0x80; retail 129, 129, 123, 123), and the two sum-test bodies (fadd) are TR and BL in both.
//   With y growing up, fx+fy > 1 is the top-right triangle, fx+fy <= 1 bottom-left, fx < fy top-left, fx >= fy bottom-right.
#include "engine/C3DVector.h"

class CWorldMap
{
protected:
    // ?PointIsWithinCellXX@CWorldMap@@IBE_NPBVC3DVector@@0@Z -- protected const __fastcall, `this` unused
    bool PointIsWithinCellTL(const C3DVector* point, const C3DVector* cell) const;
    bool PointIsWithinCellTR(const C3DVector* point, const C3DVector* cell) const;
    bool PointIsWithinCellBL(const C3DVector* point, const C3DVector* cell) const;
    bool PointIsWithinCellBR(const C3DVector* point, const C3DVector* cell) const;
};

bool CWorldMap::PointIsWithinCellBR(const C3DVector* point, const C3DVector* cell) const
{
    float blockX = (float)(long)point->X;
    return blockX == cell->X
        && cell->Y == (float)(long)point->Y
        && (point->X - blockX) >= (point->Y - (float)(long)point->Y);
}