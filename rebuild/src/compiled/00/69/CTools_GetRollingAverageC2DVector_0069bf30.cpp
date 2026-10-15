// CTools::GetRollingAverage<C2DVector>(const C2DVector&, const C2DVector&, float) -- folds a new sample into a rolling average: (average * count + value) / (count + 1), returned by value
// Leaf: GetRollingAverage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069BF30, length 76 bytes (`ret 10h`: hidden result,
// two vector references, the float count; `this` unused).
// Label: the catalog name says `<C3DVector>`, but the body touches two components and builds an 8-byte
// temporary: this is the C2DVector instantiation (the C3DVector one is 0x0084EAF0, same shape with z).
// Constant: 0x0122DED8 = 1.0f (twice: `count + 1.0f` and the reciprocal `1.0f / (count + 1.0f)`).
// Shape: inline C2DVector operators with a user constructor (hidden-pointer result): `v * f` multiplies
// each component as `x * f` (`fld f ; fmul [x]`), the sum's x is parked in an 8-byte frame temporary
// while y stays on the FPU stack, and `operator/` multiplies by one reciprocal.
struct C2DVector {
    float x, y;
    C2DVector() { }
    C2DVector(float ax, float ay) : x(ax), y(ay) { }
    C2DVector operator*(float f) const { return C2DVector(x * f, y * f); }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(x + o.x, y + o.y); }
    C2DVector operator/(float f) const
    {
        float inv = 1.0f / f;
        return C2DVector(x * inv, y * inv);
    }
};

struct CTools {
    template <class T> T GetRollingAverage(const T& average, const T& value, float count);
};

template <class T>
T CTools::GetRollingAverage(const T& average, const T& value, float count)
{
    return (average * count + value) / (count + 1.0f);
}

template C2DVector CTools::GetRollingAverage<C2DVector>(const C2DVector&, const C2DVector&, float);