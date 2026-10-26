// Behaviour test for CTCPhysicsNavigator::GetPossibleCollidedWithAreaWithImpulse (0x007267C0).
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class C2DCoordI { public: long X, Y; };
class C2DBoxI { public:
    C2DCoordI TopLeft;
    C2DCoordI BottomRight;
    void ValidateOrientationAndFlip()
    {
        if (BottomRight.X < TopLeft.X) { long t = TopLeft.X; TopLeft.X = BottomRight.X; BottomRight.X = t; }
        if (BottomRight.Y < TopLeft.Y) { long t = TopLeft.Y; TopLeft.Y = BottomRight.Y; BottomRight.Y = t; }
    }
};

template <class T> inline T Min(T a, T b) { return a < b ? a : b; }
template <class T> inline T Max(T a, T b) { return a > b ? a : b; }

struct CTCPhysicsNavigator {
    C3DVector Position;
    C3DVector Velocity;
    float     Radius;
    C2DBoxI GetPossibleCollidedWithAreaWithImpulse(const C3DVector& impulse) const
    {
        float moveX = Velocity.X + impulse.X;
        float moveY = Velocity.Y + impulse.Y;
        float radius = Radius + 1.0f;
        float minX = Position.X - radius + Min(moveX, 0.0f);
        float maxX = Position.X + Max(moveX, 0.0f) + radius;
        float minY = Position.Y - radius + Min(moveY, 0.0f);
        float maxY = Position.Y + Max(moveY, 0.0f) + radius;
        C2DBoxI box;
        box.TopLeft.X = (long)minX;
        box.TopLeft.Y = (long)maxY;
        box.BottomRight.X = (long)maxX;
        box.BottomRight.Y = (long)minY;
        box.ValidateOrientationAndFlip();
        return box;
    }
};

int main()
{
    CTCPhysicsNavigator p;
    p.Position.X = 10.0f; p.Position.Y = 20.0f; p.Position.Z = 0.0f;
    p.Velocity.X = -1.0f; p.Velocity.Y = 2.0f; p.Velocity.Z = 0.0f;
    p.Radius = 1.0f;
    C3DVector impulse = { 1.0f, 1.0f, 0.0f };   // the caller's (1, 1, 0)
    // radius 2; moveX 0 -> X [8, 12]; moveY 3 -> Y [18, 25]
    C2DBoxI b = p.GetPossibleCollidedWithAreaWithImpulse(impulse);
    bool ok = b.TopLeft.X == 8 && b.BottomRight.X == 12 && b.TopLeft.Y == 18 && b.BottomRight.Y == 25;
    impulse.X = -5.0f;   // moveX -6 -> X [2, 12]
    b = p.GetPossibleCollidedWithAreaWithImpulse(impulse);
    ok = ok && b.TopLeft.X == 2 && b.BottomRight.X == 12;
    if (!ok) { std::printf("007267c0_TEST FAIL\n"); return 1; }
    std::printf("007267c0_TEST PASS\n");
    return 0;
}