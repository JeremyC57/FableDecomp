// std::__find_if(C3DVector* first, C3DVector* last, unary_negate<CVectorFilter_FurtherFromPointThan> pred, const random_access_iterator_tag&) -- STLport random-access find_if: the first point within the filter's XY radius of its centre (four predicate tests per pass, then the 3/2/1 remainder), or last
// Leaf: ??$__find_if@PAVC3DVector@@V?$unary_negate@VCVectorFilter_FurtherFromPointThan@@@std@@@std@@YIPAVC3DVector@@PAV1@0V?$unary_negate@VCVectorFilter_FurtherFromPointThan@@@0@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AAC50, 411 bytes, /Gr `__fastcall` (first in ecx, last in edx; the 16-byte
//   predicate by value and the tag reference on the stack), `ret 14h`.
// Identity (proven): bound under this spelling; called by the landed remove_if<C3DVector*, unary_negate<
//   CVectorFilter_FurtherFromPointThan> > 0x008AC060 (0x008AC08B) and from 0x008AB706 / 0x009742C7; its one out-of-line
//   predicate call is the landed unary_negate<CVectorFilter_FurtherFromPointThan>::operator() 0x008AABC0 (FableWin.pdb
//   names _Find_if / remove_if over that predicate; Ego_r.pdb CVectorFilter_FurtherFromPointThan {float Radius; C3DVector
//   CentrePos}). The catalog's "find_if_3d_distance" is descriptive.
// Callees: unary_negate<CVectorFilter_FurtherFromPointThan>::operator()(const C3DVector&) const 0x008AABC0 (the fourth test
//   of each pass; VC7.1 inlines the other seven).
// Model: the landed __find_if stand-in body of 0x008AC060 (STLport 4.6 random-access __find_if verbatim).
// Matching lever (this session): the predicate in its PDB vocabulary -- `C2DVector delta(pos - CentrePos);
//   return delta.GetSquaredMagnitude() > Radius * Radius;` with Ego_r's `const C3DVector C3DVector::operator-(const C3DVector&)
//   const`, `C2DVector::C2DVector(const C3DVector&)` and `C2DVector::GetSquaredMagnitude() const`. The 3D subtraction (its Z
//   lane dead-code eliminated) re-keys the x87 operands so the FIRST inlined copy squares dx before dy, as retail; the earlier
//   open-coded 2D delta (`C2DVector delta(pos.X - CentrePos.X, ...)`, dx/dy locals, radius locals) squared dy first there.
//   The landed out-of-line 0x008AABC0 still grades MATCH with this predicate body (probe), so both TUs can share it.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
};

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    float GetSquaredMagnitude() const { return X * X + Y * Y; }
};

class CVectorFilter_FurtherFromPointThan {      // Ego_r.pdb, 16 bytes
public:
    float     Radius;                           // +0x00
    C3DVector CentrePos;                        // +0x04
    bool operator()(const C3DVector& pos) const
    {
        C2DVector delta(pos - CentrePos);
        return delta.GetSquaredMagnitude() > Radius * Radius;
    }
};

namespace std {
template <class _Arg, class _Result>
struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};

template <class _Predicate>
class unary_negate : public unary_function<C3DVector, bool> {
protected:
    _Predicate _M_pred;
public:
    bool operator()(const C3DVector& __x) const;                // 0x008AABC0
};

template <class _Predicate>
bool unary_negate<_Predicate>::operator()(const C3DVector& __x) const
{
    return !_M_pred(__x);
}

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0 ; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}

template C3DVector* __fastcall __find_if(C3DVector*, C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan>,
                                         const random_access_iterator_tag&);
}