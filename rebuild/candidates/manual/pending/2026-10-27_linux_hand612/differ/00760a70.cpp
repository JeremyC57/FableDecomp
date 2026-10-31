// NShape::CSubdivisionTriangle::GetUpDirection() -- set UpDirection to +Z, flipped when the triangle's winding faces down
// Grade: DIFFER(332v332) -- one instruction: the epilogue's pop esi sits between the last fmul and faddp of the dot product (retail: one fxch/fmul earlier, after the first faddp); corrected own symbol (private AAE, class key V)
// Leaf: ?GetUpDirection@CSubdivisionTriangle@NShape@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00760A70, 332 bytes, `ret`. Ego_r/FableWin symbol ?GetUpDirection@CSubdivisionTriangle@NShape@@AAEXXZ
//   (private; class key V for NShape::CShape and NShape::CSubdivisionTriangle in pdb_class_keys.tsv). No callees.
//   Pools: 0.5f 0x0122F59C, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0.
// Layout: the landed NShape::CShape model (UpdateDeletePoint 0x00762010): vptr +0, Owner +0x04, Points (STLport
//   vector<C3DVector>, 12 bytes) +0x08, SelectedPoints +0x14; Ego_r CSubdivisionTriangle::UpDirection follows at +0x20 (the
//   Ego_r offsets 40/52 are Dinkumware-sized). No shared engine header exists for these classes.
// Shape: the corners walk an iterator (`a = it[0]; ++it; b = it[0]; c = it[1];`, retail `add eax,0Ch`) and C3DVector needs a
//   memberwise user copy ctor; UpDirection is assigned a (0,0,1) temporary; the three corners are copied into locals; the side
//   vector is cross(b - (a + c) * 0.5, Up) and the up direction is negated (`*= -1`) when dot(c - b, side) < 0.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};
static inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
static inline float DotProduct(const C3DVector& a, const C3DVector& b)
{
    return a.X * b.X + (a.Y * b.Y + a.Z * b.Z);
}
namespace std {
template <class T> class allocator { public: };
template <class T, class A = allocator<T> > class vector { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
};
}
class CTCShapeManager;
namespace NShape {
class CShape { public:
    virtual ~CShape();
    CTCShapeManager*         Owner;            // +0x04
    std::vector<C3DVector>   Points;           // +0x08
    std::vector<C3DVector*>  SelectedPoints;   // +0x14
};
class CClosedShapeBase : public CShape { public: };
class CSubdivisionTriangle : public CClosedShapeBase {
    void GetUpDirection();
public:
    C3DVector UpDirection;                     // +0x20
};

void CSubdivisionTriangle::GetUpDirection()
{
    UpDirection = C3DVector(0.0f, 0.0f, 1.0f);
    const C3DVector* it = Points.begin();
    C3DVector a = it[0];
    ++it;
    C3DVector b = it[0];
    C3DVector c = it[1];
    C3DVector side = CrossProduct(b - (a + c) * 0.5f, UpDirection);
    if (DotProduct(c - b, side) < 0.0f)
        UpDirection *= -1.0f;
}
}
