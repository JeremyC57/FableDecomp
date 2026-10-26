// CWorldMap::PointIsWithinCellTR(const C3DVector*, const C3DVector*) -- point lies in the cell's top-right triangle
// Leaf: PointIsWithinCellTR
// Retail 0x000063ba30, 0x79 bytes (world-map theme/height TU). Mangled ?PointIsWithinCellTR@CWorldMap@@IBE_NPBVC3DVector@@0@Z.
// Same shape as PointIsWithinCellTL (0x0063B920): __ftol2 (0x00BFEA70) truncation of the point's X/Y must equal the
// cell corner (truncated X compared on the left so VC7.1 stores and reloads it from the dead first-parameter
// slot), then the fractional parts fx, fy are tested: fx >= fy (`test ah,41h ; jp` on dy vs dx). `return a && b && c` gives the int-valued exits.
// Name note: the four cell rows carry propagated TL/BR labels; the name here is inferred from the triangle tested
// (origin corner = TL, y growing down), so the landing session should confirm it against the PDB symbol order.
class C3DVector
{ public:
    float X;
    float Y;
    float Z;
};

class CWorldMap
{
protected:
    // ?PointIsWithinCellXX@CWorldMap@@IBE_NPBVC3DVector@@0@Z -- protected const __fastcall, `this` unused
    bool PointIsWithinCellTL(const C3DVector* point, const C3DVector* cell) const;
    bool PointIsWithinCellTR(const C3DVector* point, const C3DVector* cell) const;
    bool PointIsWithinCellBL(const C3DVector* point, const C3DVector* cell) const;
    bool PointIsWithinCellBR(const C3DVector* point, const C3DVector* cell) const;
};

bool CWorldMap::PointIsWithinCellTR(const C3DVector* point, const C3DVector* cell) const
{
    float blockX = (float)(long)point->X;
    return blockX == cell->X
        && cell->Y == (float)(long)point->Y
        && (point->X - blockX) >= (point->Y - (float)(long)point->Y);
}