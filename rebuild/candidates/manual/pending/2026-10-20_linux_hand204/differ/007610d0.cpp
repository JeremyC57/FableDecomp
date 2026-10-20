// Grade: DIFFER(237v237) -- register swap only: retail keeps the point in edx and the wrapping successor in esi (and loads the point's floats first); the lane swaps them (declaration-order variants of point/next tried). All x87 arithmetic, the hoisted zero products and the frame match.
// NShape::CSubdivisionTriangle::IsPosInShape(const C3DVector& pos) const -- true when the position (flattened onto the ground plane) lies on the inner side of every edge of the convex outline
// Retail: Fable.exe (TLC, VC7.1) @ 0x007610D0, 235 bytes (+ int3 padding), `ret 4`; NShape::CSubdivisionTriangle vtable slot 4
//   (called by the landed NShape::CBasicShape::IsPosInShape 0x007611C0). Ego_r.pdb `?IsPosInShape@CSubdivisionTriangle@NShape@@UBE_NABVC3DVector@@@Z`.
// Leaf: IsPosInShape
// No callees; float pool 0.0f @ 0x0122DEDC.
// Shape: the position is copied once into a flat (X, Y, 0) vector; each edge (point -> next point, wrapping to the first) is
//   flattened the same way, crossed with UpDirection (the products of the zero Z with UpDirection.Y / .X are loop-invariant and
//   hoisted, one into the dead `pos` slot) and dotted with (flat position - point); a negative dot is outside. The walk keeps
//   begin/end in ebx/edi, the point in edx and the wrapping successor in esi.
// Retail layout: the NShape::CShape points vector at +0x08 (12-byte STLport vector of C3DVector), UpDirection +0x20
//   (Ego_r.pdb +0x28: the base's two containers are 4 bytes smaller each).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator^(const C3DVector& o) const   // cross product
    {
        return C3DVector(Y * o.Z - Z * o.Y, Z * o.X - X * o.Z, X * o.Y - Y * o.X);
    }
    float operator*(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }   // dot product
};

template <class T> struct vector {                      // retail STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
};

namespace NShape {
class CShape {
public:
    virtual void v00();
    long              _word04;                          // +0x04
    vector<C3DVector> Points;                           // +0x08
    unsigned char     _pad14[0x20 - 0x14];
};
class CSubdivisionTriangle : public CShape {
public:
    C3DVector UpDirection;                              // +0x20
    virtual bool IsPosInShape(const C3DVector& pos) const;
};

bool CSubdivisionTriangle::IsPosInShape(const C3DVector& pos) const
{
    const C3DVector flatPos(pos.X, pos.Y, 0.0f);
    const C3DVector* end = Points.end();
    const C3DVector* begin = Points.begin();
    const C3DVector* next = begin + 1;
    for (const C3DVector* point = begin; point != end; ++point) {
        C3DVector flatPoint(point->X, point->Y, 0.0f);
        C3DVector side = (C3DVector(next->X, next->Y, 0.0f) - flatPoint) ^ UpDirection;
        if (side * (flatPos - flatPoint) < 0.0f)
            return false;
        ++next;
        if (next == end)
            next = begin;
    }
    return true;
}
}
