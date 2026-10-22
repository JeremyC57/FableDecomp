#include <cstdio>
struct C2DVector {
    float X; float Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    float Dot(const C2DVector& o) const { return X * o.X + Y * o.Y; }
};
struct C2DLineF {
    C2DVector Point1; C2DVector Point2;
    C2DLineF(float x1, float y1, float x2, float y2) : Point1(x1, y1), Point2(x2, y2) {}
    bool OnLeft(const C2DVector& pos) const
    {
        C2DVector dir(Point2.X - Point1.X, Point2.Y - Point1.Y);
        C2DVector normal(-dir.Y, dir.X);
        C2DVector rel(pos.X - Point1.X, pos.Y - Point1.Y);
        return rel.Dot(normal) > 0.0f ? true : false;
    }
};
int main()
{
    C2DLineF line(0.0f, 0.0f, 10.0f, 0.0f);     // pointing +X: "left" is +Y
    if (!line.OnLeft(C2DVector(5.0f, 1.0f)) || line.OnLeft(C2DVector(5.0f, -1.0f)) || line.OnLeft(C2DVector(5.0f, 0.0f))) {
        std::printf("00a56ff0_TEST FAIL\n"); return 1;
    }
    std::printf("00a56ff0_TEST PASS\n");
    return 0;
}