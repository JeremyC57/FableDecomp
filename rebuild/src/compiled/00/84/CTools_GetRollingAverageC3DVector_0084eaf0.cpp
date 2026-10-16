// CTools::GetRollingAverage<C3DVector>(const C3DVector&, const C3DVector&, float) -- folds a new sample into a rolling average: (average * count + value) / (count + 1), returned by value
// Leaf: GetRollingAverage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084EAF0, length 101 bytes (`ret 10h`: hidden result, two vector
// references, the float count; `this` unused). The C3DVector twin of the landed C2DVector instantiation 0x0069BF30.
// Constant: 0x0122DED8 = 1.0f (twice: `count + 1.0f` and the reciprocal `1.0f / (count + 1.0f)`).
// Shape: inline C3DVector operators with a user constructor (hidden-pointer result); `operator/` multiplies by one
// reciprocal.
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector operator*(float f) const { return C3DVector(x * f, y * f, z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
    C3DVector operator/(float f) const
    {
        float inv = 1.0f / f;
        return C3DVector(x * inv, y * inv, z * inv);
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

template C3DVector CTools::GetRollingAverage<C3DVector>(const C3DVector&, const C3DVector&, float);