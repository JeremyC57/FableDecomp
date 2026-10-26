#include <cstdio>
// Model of unary_negate<CVectorFilter_FurtherFromPointThan>::operator() @ 0x008AABC0: true when the point lies within
// the radius of the centre on the XY plane (Z ignored, the boundary counts as within).
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLengthSquared() const { return X * X + Y * Y; }
};
class CVectorFilter_FurtherFromPointThan {
public:
    float     Radius;
    C3DVector CentrePos;
    bool operator()(const C3DVector& pos) const
    {
        C2DVector delta(pos.X - CentrePos.X, pos.Y - CentrePos.Y);
        float radius = Radius;
        return delta.GetLengthSquared() > radius * radius;
    }
};
template <class _Predicate>
class unary_negate {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& __x) : _M_pred(__x) { }
    bool operator()(const C3DVector& __x) const;
};
template <class _Predicate>
bool unary_negate<_Predicate>::operator()(const C3DVector& __x) const
{
    return !_M_pred(__x);
}
int main()
{
    bool ok = true;
    CVectorFilter_FurtherFromPointThan further;
    further.Radius = 5.0f;
    further.CentrePos.X = 1.0f;
    further.CentrePos.Y = 2.0f;
    further.CentrePos.Z = 3.0f;
    unary_negate<CVectorFilter_FurtherFromPointThan> within(further);
    C3DVector inside = { 2.0f, 3.0f, 100.0f };           // far in Z, close in XY
    C3DVector edge = { 4.0f, 6.0f, 3.0f };               // exactly 5 away (3-4-5)
    C3DVector outside = { 7.0f, 2.0f, 3.0f };
    if (!within(inside) || !within(edge) || within(outside)) ok = false;
    if (!ok) { std::printf("008aabc0_TEST FAIL\n"); return 1; }
    std::printf("008aabc0_TEST PASS\n");
    return 0;
}