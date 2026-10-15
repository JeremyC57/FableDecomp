#include <cstdio>
// Behaviour test for CTools::GetRollingAverage<C2DVector> (0x0069BF30): (average * count + value) / (count + 1).
struct C2DVector {
    float x, y;
    C2DVector() { }
    C2DVector(float ax, float ay) : x(ax), y(ay) { }
    C2DVector operator*(float f) const { return C2DVector(x * f, y * f); }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(x + o.x, y + o.y); }
    C2DVector operator/(float f) const { float inv = 1.0f / f; return C2DVector(x * inv, y * inv); }
};
struct CTools {
    template <class T> T GetRollingAverage(const T& average, const T& value, float count)
    {
        return (average * count + value) / (count + 1.0f);
    }
};
int main()
{
    CTools t;
    C2DVector avg(2.0f, 4.0f), v(6.0f, -4.0f);
    C2DVector r = t.GetRollingAverage(avg, v, 3.0f);     // (6+6)/4 = 3, (12-4)/4 = 2
    if (r.x != 3.0f || r.y != 2.0f) { std::printf("0069bf30_TEST FAIL\n"); return 1; }
    r = t.GetRollingAverage(avg, v, 0.0f);               // first sample: the value itself
    if (r.x != 6.0f || r.y != -4.0f) { std::printf("0069bf30_TEST FAIL\n"); return 1; }
    std::printf("0069bf30_TEST PASS\n");
    return 0;
}
