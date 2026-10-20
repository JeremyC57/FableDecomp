// Behaviour test for 0x008E5370 CTCPhysicsStandard::GetPossibleCollidedWithArea (copy of the source model).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct C2DCoordI { long X, Y; };
static int g_validated = 0;
struct C2DBoxI {
    C2DCoordI TopLeft;
    C2DCoordI BottomRight;
    C2DBoxI() {}
    C2DBoxI(const C2DBoxI& o)
    {
        TopLeft.X = o.TopLeft.X; TopLeft.Y = o.TopLeft.Y;
        BottomRight.X = o.BottomRight.X; BottomRight.Y = o.BottomRight.Y;
    }
    void ValidateOrientationAndFlip() { ++g_validated; }   // stub: retail flips inverted corners
};
struct CTCPhysicsStandard {
    void*     vft;
    void*     ParentThing;
    unsigned char _pad08[0x4];
    C3DVector Position;
    C3DVector OldPosition;
    long      OldPositionLastFrameSet;
    C3DVector Velocity;
    float     Radius;
    C2DBoxI GetPossibleCollidedWithArea();
};
C2DBoxI CTCPhysicsStandard::GetPossibleCollidedWithArea()
{
    float vx = Velocity.X;
    float vy = Velocity.Y;
    float reach = Radius + 1.0f;
    float minX = Position.X - reach + (vx < 0.0f ? vx : 0.0f);
    float maxX = (vx > 0.0f ? vx : 0.0f) + Position.X + reach;
    float minY = Position.Y - reach + (vy < 0.0f ? vy : 0.0f);
    float maxY = (vy > 0.0f ? vy : 0.0f) + reach + Position.Y;
    C2DBoxI box;
    box.TopLeft.X = (long)minX;
    box.TopLeft.Y = (long)maxY;
    box.BottomRight.X = (long)maxX;
    box.BottomRight.Y = (long)minY;
    box.ValidateOrientationAndFlip();
    return box;
}
int main()
{
    CTCPhysicsStandard p;
    p.Position.X = 100.0f; p.Position.Y = 200.0f; p.Position.Z = 0.0f;
    p.Radius = 2.0f;
    p.Velocity.X = 5.0f; p.Velocity.Y = -7.0f; p.Velocity.Z = 0.0f;
    C2DBoxI b = p.GetPossibleCollidedWithArea();
    // reach 3: X in [97, 108], Y in [190, 203]
    if (b.TopLeft.X != 97 || b.BottomRight.X != 108 || b.BottomRight.Y != 190 || b.TopLeft.Y != 203 || g_validated != 1) {
        std::printf("008e5370_TEST FAIL\n"); return 1;
    }
    p.Velocity.X = -4.0f; p.Velocity.Y = 6.0f;
    b = p.GetPossibleCollidedWithArea();
    // X in [93, 103], Y in [197, 209]
    if (b.TopLeft.X != 93 || b.BottomRight.X != 103 || b.BottomRight.Y != 197 || b.TopLeft.Y != 209) {
        std::printf("008e5370_TEST FAIL\n"); return 1;
    }
    std::printf("008e5370_TEST PASS\n");
    return 0;
}