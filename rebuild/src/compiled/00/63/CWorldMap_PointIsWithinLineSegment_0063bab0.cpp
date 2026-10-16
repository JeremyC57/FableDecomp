// CWorldMap::PointIsWithinLineSegment(const C3DVector*, const C3DVector*, const C3DVector*) -- point inside the segment's X/Y bounding box
// Leaf: PointIsWithinLineSegment
// Retail 0x0063BAB0, 0x8C bytes (world-map theme/height TU). Mangled
// ?PointIsWithinLineSegment@CWorldMap@@IBE_NPBVC3DVector@@00@Z (protected const __fastcall, `this` unused).
// For X then Y: min(a,b) <= p (`a < b ? a : b`, `test ah,5 ; jp` then `test ah,41h ; jp`) and
// max(a,b) >= p (`a > b ? a : b`, `test ah,41h ; jne` then `test ah,1 ; jne`). No float constants;
// `return a && b && c && d` gives the int-valued `mov eax,1` / `xor eax,eax` exits.
struct C3DVector
{
    float X;
    float Y;
    float Z;
};

class CWorldMap
{
protected:
    bool PointIsWithinLineSegment(const C3DVector* point, const C3DVector* a, const C3DVector* b) const;
};

bool CWorldMap::PointIsWithinLineSegment(const C3DVector* point, const C3DVector* a, const C3DVector* b) const
{
    return (a->X < b->X ? a->X : b->X) <= point->X
        && (a->X > b->X ? a->X : b->X) >= point->X
        && (a->Y < b->Y ? a->Y : b->Y) <= point->Y
        && (a->Y > b->Y ? a->Y : b->Y) >= point->Y;
}