#include <cstdio>
#include <cmath>
inline float LengthOf(float value) { return (float)std::sqrt((double)value); }
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return LengthOf(X * X + Y * Y + Z * Z); }
};
struct CIsOutsideRadius {
    float Radius;
    C3DVector Centre;
    CIsOutsideRadius(float r, const C3DVector& c) : Radius(r), Centre(c) {}
    CIsOutsideRadius(const CIsOutsideRadius& o) : Radius(o.Radius), Centre(o.Centre) {}
    bool operator()(const C3DVector& p) const { C3DVector offset = p - Centre; return offset.GetLength() > Radius; }
};
// The candidate logic: SGI remove_copy_if.
template <class InputIter, class OutputIter, class Predicate>
OutputIter RemoveCopyIf(InputIter first, InputIter last, OutputIter result, Predicate pred)
{
    for (; first != last; ++first)
        if (!pred(*first)) {
            *result = *first;
            ++result;
        }
    return result;
}
int main()
{
    C3DVector points[5];
    points[0] = C3DVector(1, 0, 0);
    points[1] = C3DVector(5, 0, 0);
    points[2] = C3DVector(0, 2, 0);
    points[3] = C3DVector(0, 0, -3.5f);
    points[4] = C3DVector(1, 1, 1);
    C3DVector out[5];
    C3DVector* end = RemoveCopyIf(points, points + 5, out, CIsOutsideRadius(2.0f, C3DVector(0, 0, 0)));
    bool ok = end == out + 3 && out[0].X == 1 && out[1].Y == 2 && out[2].Z == 1;
    // an off-centre ring keeps only the points near (5, 0, 0)
    C3DVector* end2 = RemoveCopyIf(points, points + 5, out, CIsOutsideRadius(1.0f, C3DVector(5, 0, 0)));
    if (end2 != out + 1 || out[0].X != 5)
        ok = false;
    if (!ok) { std::printf("008c2ed0_TEST FAIL\n"); return 1; }
    std::printf("008c2ed0_TEST PASS\n");
    return 0;
}