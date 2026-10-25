// NShape::GetFirstPointThatBreaksConvexity(std::vector<C3DVector>& shape) -- walk the shape's consecutive point triples and return the middle point of the first triple whose turn is not between the direction back to the first point and the incoming edge; end() when the whole outline is convex
// Leaf: ?GetFirstPointThatBreaksConvexity@NShape@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00760760, length 271 bytes (__fastcall: shape in ecx; returns the iterator
//   (a C3DVector pointer in STLport) in eax; plain `ret`).
// Identity: Ego_r.pdb / FableWin.pdb ?GetFirstPointThatBreaksConvexity@NShape@@YI?AV?$_Vector_iterator@...@std@@AAV?$vector@
//   VC3DVector@@...@3@@Z (no callers by direct call: referenced from the shape code's tables).
// Callees: NShape::IsVectorBetween2OthersInThePlane(const C3DVector&, const C3DVector&, const C3DVector&) 0x0075FF00
//   (landed; __fastcall, the third vector on the stack).
// Shape: `it = shape.begin(); first = *it; last = shape.end() - 2; for (++it; it != last; ++it) { p0 = *it;
//   p1 = it[1]; p2 = it[2]; toFirst = first - p1; inEdge = p1 - p0; outEdge = p2 - p1; (named, in this order)
//   if (!IsVectorBetween2OthersInThePlane(toFirst, inEdge, outEdge)) return it + 1; }
//   return shape.end();`
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
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
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};

}

namespace NShape {

bool __fastcall IsVectorBetween2OthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& c);   // 0x0075FF00

std::vector<C3DVector>::iterator __fastcall GetFirstPointThatBreaksConvexity(std::vector<C3DVector>& shape);

std::vector<C3DVector>::iterator __fastcall GetFirstPointThatBreaksConvexity(std::vector<C3DVector>& shape)
{
    std::vector<C3DVector>::iterator it = shape.begin();
    C3DVector first = *it;
    std::vector<C3DVector>::iterator last = shape.end() - 2;
    for (++it; it != last; ++it) {
        C3DVector p0 = *it;
        C3DVector p1 = *(it + 1);
        C3DVector p2 = *(it + 2);
        C3DVector toFirst = first - p1;
        C3DVector inEdge = p1 - p0;
        C3DVector outEdge = p2 - p1;
        if (!IsVectorBetween2OthersInThePlane(toFirst, inEdge, outEdge))
            return it + 1;
    }
    return shape.end();
}

}