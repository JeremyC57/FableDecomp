// NShape::IsTriangleWithPointsInside(const std::vector<C3DVector>& triangle, const std::vector<C3DVector>& points, ECirculationOrder order) -- count the points lying on the inner side of all three edges of the triangle (edge normals flipped for the other circulation order, 1e-4 tolerance); true when more than three are inside
// Grade: DIFFER(324v340) -- integer register allocation only: the x87 sequence (the -epsilon hoisted on st(1) via an extern epsilon, the order-1 `*= -1` on the stack, the dot and fcomp) now matches retail; retail keeps the copied vertex in edx/esi/edi and the points iterator in ebp with the edge-inside counter and triangle.begin() in frame slots, VC7.1 here keeps the counter in edx and begin() in edi and spills the vertex
// Leaf: ?IsTriangleWithPointsInside@NShape@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00760190, length 338 bytes (__fastcall: triangle in ecx, points in edx,
//   order on the stack, `ret 4`).
// Identity: Ego_r.pdb / FableWin.pdb ?IsTriangleWithPointsInside@NShape@@YI_NABV?$vector@VC3DVector@@...@std@@0
//   W4ECirculationOrder@1@@Z; caller 0x0076322D. No callees.
// Globals: 1e-4f 0x0129BA3C (modelled as an extern const float so `-g_Epsilon` is hoisted onto the x87 stack), -1.0f 0x0122DEE0, 0.0f 0x0122DEDC.
// Shape: `for each point p { it = triangle.begin(); prev = *it++; done = false; sides = 0; do { cur = (it == end) ?
//   (done = true, *begin) : *it++; edge = cur - prev; rel = p - prev; normal = (edge.Y, -edge.X, 0); if (order == 1)
//   normal = normal * -1; if (normal.Dot(rel) > -1e-4) ++sides; prev = cur; } while (!done); if (sides == 3)
//   ++inside; } return inside > 3;`
extern const float g_Epsilon;                           // 0x0129BA3C (1e-4f)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
    const float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_iterator;
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
};

}

namespace NShape {

enum ECirculationOrder {
    CIRCULATION_ORDER_0,
    CIRCULATION_ORDER_1
};

bool __fastcall IsTriangleWithPointsInside(const std::vector<C3DVector>& triangle, const std::vector<C3DVector>& points,
                                           ECirculationOrder order);

bool __fastcall IsTriangleWithPointsInside(const std::vector<C3DVector>& triangle, const std::vector<C3DVector>& points,
                                           ECirculationOrder order)
{
    long pointsInside = 0;
    for (std::vector<C3DVector>::const_iterator p = points.begin(); p != points.end(); ++p) {
        std::vector<C3DVector>::const_iterator it = triangle.begin();
        C3DVector prev = *it;
        ++it;
        bool finished = false;
        long sidesInside = 0;
        do {
            std::vector<C3DVector>::const_iterator curIt;
            if (it == triangle.end()) {
                curIt = triangle.begin();
                finished = true;
            } else {
                curIt = it;
                ++it;
            }
            C3DVector cur = *curIt;
            C3DVector edge = cur - prev;
            C3DVector toPoint = *p - prev;
            C3DVector normal(edge.Y, -edge.X, 0.0f);
            if (order == CIRCULATION_ORDER_1)
                normal *= -1.0f;
            if (normal.Dot(toPoint) > -g_Epsilon)
                ++sidesInside;
            prev = cur;
        } while (!finished);
        if (sidesInside == 3)
            ++pointsInside;
    }
    return pointsInside > 3;
}

}
