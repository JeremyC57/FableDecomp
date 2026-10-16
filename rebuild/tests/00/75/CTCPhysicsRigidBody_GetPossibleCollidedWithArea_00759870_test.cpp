#include <cstdio>
// Behaviour model of CTCPhysicsRigidBody::GetPossibleCollidedWithArea (0x00759870)
struct C2DCoordI { long X, Y; };
struct C2DBoxI { C2DCoordI TopLeft, BottomRight;
    void ValidateOrientationAndFlip() {
        if (BottomRight.X < TopLeft.X) { long t = TopLeft.X; TopLeft.X = BottomRight.X; BottomRight.X = t; }
        if (BottomRight.Y < TopLeft.Y) { long t = TopLeft.Y; TopLeft.Y = BottomRight.Y; BottomRight.Y = t; }
    } };
static C2DBoxI Area(float x, float y, float circleRadius)
{
    float radius = circleRadius * 3.0f;
    C2DBoxI box;
    box.TopLeft.X = (long)(x - radius); box.TopLeft.Y = (long)(y - radius);
    box.BottomRight.X = (long)(radius + x); box.BottomRight.Y = (long)(radius + y);
    box.ValidateOrientationAndFlip();
    return box;
}
int main()
{
    C2DBoxI b = Area(10.0f, 20.0f, 2.0f);
    if (b.TopLeft.X != 4 || b.TopLeft.Y != 14 || b.BottomRight.X != 16 || b.BottomRight.Y != 26) { std::printf("00759870_TEST FAIL\n"); return 1; }
    b = Area(0.0f, 0.0f, -1.0f);
    if (b.TopLeft.X != -3 || b.BottomRight.X != 3) { std::printf("00759870_TEST FAIL\n"); return 1; }
    std::printf("00759870_TEST PASS\n");
    return 0;
}