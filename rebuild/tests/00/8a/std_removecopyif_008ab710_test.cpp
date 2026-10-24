// Behaviour test for 0x008AB710 std::remove_copy_if<C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan> >:
// copies (in order) exactly the points whose XY distance from the filter point exceeds the filter distance.
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
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

static C3DVector P(float x, float y, float z) { C3DVector v; v.x = x; v.y = y; v.z = z; return v; }

int main()
{
    int ok = 1;
    C3DVector pts[6];
    pts[0] = P(0.0f, 0.0f, 0.0f);     // at the point
    pts[1] = P(3.0f, 4.0f, 9.0f);     // 5 away (z ignored)
    pts[2] = P(1.0f, 1.0f, -50.0f);   // 1.41 away
    pts[3] = P(-6.0f, 0.0f, 0.0f);    // 6 away
    pts[4] = P(2.0f, 2.0f, 0.0f);     // 2.83 away
    pts[5] = P(0.0f, -2.5f, 0.0f);    // 2.5 away
    CVectorFilter_FurtherFromPointThan further(P(0.0f, 0.0f, 100.0f), 2.6f);
    C3DVector out[6];
    C3DVector* end = std::remove_copy_if(pts, pts + 6, out, std::unary_negate<CVectorFilter_FurtherFromPointThan>(further));
    if (end - out != 3) ok = 0;
    else {
        if (out[0].x != 3.0f || out[0].y != 4.0f || out[0].z != 9.0f) ok = 0;
        if (out[1].x != -6.0f) ok = 0;
        if (out[2].x != 2.0f || out[2].y != 2.0f) ok = 0;
    }
    // empty range
    if (std::remove_copy_if(pts, pts, out, std::unary_negate<CVectorFilter_FurtherFromPointThan>(further)) != out) ok = 0;
    printf("008ab710_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}