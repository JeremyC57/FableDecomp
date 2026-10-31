// Behaviour test for C2DLineF::GetInfiniteLineLineIntersection (0x00A57960): a copy of the matched model and body.
#include <cstdio>
#include <cmath>
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
class C2DLineF {
public:
    C2DVector Point1;
    C2DVector Point2;
    bool GetInfiniteLineLineIntersection(const C2DLineF& other, C2DVector& intersection) const;
};
bool C2DLineF::GetInfiniteLineLineIntersection(const C2DLineF& other, C2DVector& intersection) const
{
    C2DVector p1 = Point1, p2 = Point2, p3 = other.Point1, p4 = other.Point2;
    float c1 = p1.X * p2.Y - p2.X * p1.Y;
    float c2 = p3.X * p4.Y - p4.X * p3.Y;
    float dy2 = p3.Y - p4.Y;
    float dx1 = p1.X - p2.X;
    float dx2 = p3.X - p4.X;
    float dy1 = p1.Y - p2.Y;
    float det = dx1 * dy2 - dy1 * dx2;
    if (det == 0.0f)
        return false;
    float inv = 1.0f / det;
    intersection.X = (dx2 * c1 - dx1 * c2) * inv;
    intersection.Y = (dy2 * c1 - dy1 * c2) * inv;
    return true;
}
static void Set(C2DLineF& l, float a, float b, float c, float d) { l.Point1.X = a; l.Point1.Y = b; l.Point2.X = c; l.Point2.Y = d; }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    C2DLineF a, b; C2DVector p; p.X = 7.0f; p.Y = 7.0f;
    Set(a, 0.0f, 0.0f, 2.0f, 2.0f); Set(b, 0.0f, 2.0f, 2.0f, 0.0f);
    if (!a.GetInfiniteLineLineIntersection(b, p) || !Near(p.X, 1.0f) || !Near(p.Y, 1.0f)) { std::printf("00a57960_TEST FAIL\n"); return 1; }
    // infinite lines: the segments do not overlap but the lines cross at (3, 0)
    Set(a, 0.0f, 0.0f, 1.0f, 0.0f); Set(b, 3.0f, 1.0f, 3.0f, 2.0f);
    if (!a.GetInfiniteLineLineIntersection(b, p) || !Near(p.X, 3.0f) || !Near(p.Y, 0.0f)) { std::printf("00a57960_TEST FAIL\n"); return 1; }
    // parallel lines: false and the output untouched
    p.X = 5.0f; p.Y = 6.0f;
    Set(a, 0.0f, 0.0f, 1.0f, 1.0f); Set(b, 0.0f, 1.0f, 1.0f, 2.0f);
    if (a.GetInfiniteLineLineIntersection(b, p) || p.X != 5.0f || p.Y != 6.0f) { std::printf("00a57960_TEST FAIL\n"); return 1; }
    std::printf("00a57960_TEST PASS\n");
    return 0;
}