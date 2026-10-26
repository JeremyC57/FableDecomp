// CWorldMap::PointIsWithinCellTL(const C3DVector*, const C3DVector*) -- point lies in the cell's top-left triangle
// Retail 0x0063B920, 0x7F bytes (world-map theme/height TU). Mangled ?PointIsWithinCellTL@CWorldMap@@IBE_NPBVC3DVector@@0@Z.
// `point`'s X/Y are truncated with __ftol2 (0x00BFEA70) to find its block; the block must equal `cell`'s corner, and
// the fractional parts must satisfy fx + fy <= 1 (pool 1.0f at 0x0122DED8; `test ah,41h ; jp`). The truncated X is
// kept as a float in the dead first-parameter slot; Y is truncated again for the fraction. `return a && b && c`
// gives the int-valued `mov eax,1` / `xor eax,eax` exits.
// Name note: 0x0063B890 / 0x0063B920 / 0x0063B9B0 / 0x0063BA30 all carry propagated TL/BR labels; the
// geometry (origin corner = TL: fx+fy <= 1) identifies this one as TL.
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

bool CWorldMap::PointIsWithinCellTL(const C3DVector* point, const C3DVector* cell) const
{
    float blockX = (float)(long)point->X;
    return blockX == cell->X
        && cell->Y == (float)(long)point->Y
        && (point->X - blockX) + (point->Y - (float)(long)point->Y) <= 1.0f;
}