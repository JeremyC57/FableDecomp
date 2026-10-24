// std::remove_copy_if(C3DVector* first, C3DVector* last, C3DVector* result, std::unary_negate<CVectorFilter_FurtherFromPointThan> pred) -- copies every point further (in XY) than a distance from a centre point to result; the tail of STLport remove_if
// Leaf: ??$remove_copy_if@PAVC3DVector@@PAV1@V?$unary_negate@VCVectorFilter_FurtherFromPointThan@@@std@@@std@@YIPAVC3DVector@@PAV1@00V?$unary_negate@VCVectorFilter_FurtherFromPointThan@@@0@@Z
// Grade: MATCH (near-miss retry agent901) -- squared length summed in statement order (`lengthSq = dx * dx; lengthSq += dy * dy;`) so VC7.1 squares dx first as retail
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AB710, 102 bytes, `ret 14h`. Caller: STLport remove_if 0x008AC060 (find_if 0x008AAC50, then
// remove_copy_if(++next, last, first, pred)); 0x009742FC (CAIStateGroup_JackOfBladesBase::Summon). The catalog's `_Find_if<...>` is wrong.
// STLport remove_copy_if, /Gr `__fastcall` (first ecx, last edx, result + the 16-byte predicate on the stack); VC hoists the
// loop-invariant `m_distance * m_distance` onto the x87 stack before the loop.
// Tried: dx*dx + dy*dy, dy*dy + dx*dx, a C3DVector operator- temporary, `r*r < lenSq` (worse) -- the sum order never flips.
namespace std {
template <class Arg, class Result> struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
};
template <class Predicate> class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
protected:
    Predicate m_pred;
public:
    explicit unary_negate(const Predicate& x) : m_pred(x) {}
    bool operator()(const typename Predicate::argument_type& x) const { return !m_pred(x); }
};
template <class InputIter, class OutputIter, class Predicate>
OutputIter __fastcall remove_copy_if(InputIter first, InputIter last, OutputIter result, Predicate pred)
{
    for ( ; first != last; ++first)
        if (!pred(*first)) {
            *result = *first;
            ++result;
        }
    return result;
}
}
class C3DVector {
public:
    float x, y, z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : x(v.x), y(v.y), z(v.z) {}
};
class CVectorFilter_FurtherFromPointThan : public std::unary_function<C3DVector, bool> {
public:
    float m_distance;                             // +0x00
    C3DVector m_point;                            // +0x04
    CVectorFilter_FurtherFromPointThan(const C3DVector& point, float distance) : m_distance(distance), m_point(point) {}
    bool operator()(const C3DVector& v) const
    {
        float dx = v.x - m_point.x;
        float dy = v.y - m_point.y;
        float lengthSq = dx * dx;
        lengthSq += dy * dy;
        return lengthSq > m_distance * m_distance;
    }
};
template C3DVector* __fastcall std::remove_copy_if(C3DVector*, C3DVector*, C3DVector*, std::unary_negate<CVectorFilter_FurtherFromPointThan>);