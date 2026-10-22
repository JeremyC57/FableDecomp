#include <cstdio>
struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    float GetLengthSquared() const { return X * X + Y * Y; }
};
struct C2DLineF {
    C2DVector Point1;
    C2DVector Point2;
    int IsWithinDist(const C2DVector& pos, float dist) const;
};
int C2DLineF::IsWithinDist(const C2DVector& pos, float dist) const
{
    C2DVector mid = (Point2 + Point1) * 0.5f;
    C2DVector dir = Point1 - Point2;
    C2DVector rel = pos - mid;
    float lengthSquared = dir.GetLengthSquared();
    if (lengthSquared * 0.25f + dist * dist > rel.GetLengthSquared())
        return 1;
    return 0;
}
int main()
{
    C2DLineF l;
    l.Point1 = C2DVector(0.0f, 0.0f);
    l.Point2 = C2DVector(4.0f, 0.0f);   // mid (2,0), half length 2
    // bound = 4 + dist^2
    int a = l.IsWithinDist(C2DVector(2.0f, 1.0f), 0.0f);   // 1 < 4
    int b = l.IsWithinDist(C2DVector(2.0f, 3.0f), 0.0f);   // 9 < 4 no
    int c = l.IsWithinDist(C2DVector(2.0f, 3.0f), 3.0f);   // 9 < 13
    int d = l.IsWithinDist(C2DVector(4.0f, 0.0f), 0.0f);   // 4 < 4 no
    if (a != 1 || b != 0 || c != 1 || d != 0) { std::printf("00a57a40_TEST FAIL\n"); return 1; }
    std::printf("00a57a40_TEST PASS\n");
    return 0;
}