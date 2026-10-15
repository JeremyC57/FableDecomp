// NShape::CShape::GetBounds(C2DBoxF& box) const -- the 2-D (X/Y) bounding box of the shape's points: seed with the first point, then widen by every later point
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00760BC0, length 122 bytes (`ret 4`). No callees.
// Layout as the landed NShape::CShape members (0x00761200 / 0x00761BD0): points vector (VC6 first/last/end) at +0x08, 12-byte C3DVector.
// Shape: the first point's X and Y are named float locals (both loaded onto the FPU, then four fstp/fst stores);
// direct member copies become integer moves. `<` gives `test ah,5 ; jp`, `>` gives `test ah,41h ; jne`; the end
// pointer is re-read every pass (the box stores may alias it). The first point is used unconditionally.
// Leaf: GetBounds
struct C3DVector { float X, Y, Z; };
struct C2DBoxF { float MinX, MinY, MaxX, MaxY; };
template <class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T* begin() const { return m_pFirst; }
    T* end() const { return m_pLast; }
};
struct CTCShapeManager;
namespace NShape {
struct CShape {
    virtual void v00();
    CTCShapeManager*          Owner;
    CRetailVector<C3DVector>  Points;
    CRetailVector<C3DVector*> SelectedPoints;
    void GetBounds(C2DBoxF& box) const;
};
}
void NShape::CShape::GetBounds(C2DBoxF& box) const
{
    const C3DVector* it = Points.begin();
    float x = it->X;
    float y = it->Y;
    box.MinX = x;
    box.MaxX = x;
    box.MinY = y;
    box.MaxY = y;
    for (++it; it != Points.end(); ++it) {
        if (it->X < box.MinX) box.MinX = it->X;
        if (it->Y < box.MinY) box.MinY = it->Y;
        if (it->X > box.MaxX) box.MaxX = it->X;
        if (it->Y > box.MaxY) box.MaxY = it->Y;
    }
}
