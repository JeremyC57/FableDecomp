// std::unary_negate::operator()(const C3DVector& x) const -- not1(CVectorFilter_FurtherFromPointThan): is the point within the radius of the centre on the XY plane
// Leaf: ??R?$unary_negate@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AABC0, length 77 bytes (+ int3 padding), `ret 4`.
// Identity: one caller, 0x008AAD13 in the STLport __find_if 0x008AAC50 that the remove_if 0x008AC060 calls; the
//   remove_copy_if 0x008AB710 of the same remove_if inlines the same test. FableWin.pdb names the instance
//   `std::unary_negate<CVectorFilter_FurtherFromPointThan>::operator()(const C3DVector&) const` (with
//   _Find_if/_Remove_if/remove_if over unary_negate<CVectorFilter_FurtherFromPointThan>); Ego_r.pdb lays the filter out
//   as { float Radius +0; C3DVector CentrePos +4 } (16 bytes). The catalog label "GFWorld3DDistanceSmallerThan" is
//   descriptive.
// Shape: `return !_M_pred(x);` with the filter's operator() inlined: the XY delta as a C2DVector (two float locals)
//   and Radius in a float local, all kept on the x87 stack (three `fstp st(0)` after the compare), then
//   delta.GetLengthSquared() (X*X first) against radius*radius. Two named float locals dx/dy square Y first; the
//   two-float C2DVector restores retail's X-first order. The inlined
//   result is materialised (`mov eax,1` / `xor eax,eax`) and then negated with `test al,al ; sete cl` (VC7.1 does not
//   fold the inlined bool into the negation).
struct C3DVector {
    float X, Y, Z;
};

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLengthSquared() const { return X * X + Y * Y; }
};

class CVectorFilter_FurtherFromPointThan {      // Ego_r.pdb, 16 bytes
public:
    float     Radius;                           // +0x00
    C3DVector CentrePos;                        // +0x04
    bool operator()(const C3DVector& pos) const
    {
        C2DVector delta(pos.X - CentrePos.X, pos.Y - CentrePos.Y);
        float radius = Radius;
        return delta.GetLengthSquared() > radius * radius;
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
    bool operator()(const C3DVector& __x) const;
};

template <class _Predicate>
bool unary_negate<_Predicate>::operator()(const C3DVector& __x) const
{
    return !_M_pred(__x);
}
template class unary_negate<CVectorFilter_FurtherFromPointThan>;
}