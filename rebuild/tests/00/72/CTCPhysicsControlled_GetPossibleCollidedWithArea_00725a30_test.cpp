// Behaviour test for CTCPhysicsControlled::GetPossibleCollidedWithArea (0x00725A30).
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
class CThing { public: unsigned long UsingComponents[9]; };

template <class T> inline T Min(T a, T b) { return a < b ? a : b; }
template <class T> inline T Max(T a, T b) { return a > b ? a : b; }

struct CTCPhysicsControlled {
    CThing*   ParentThing;
    C3DVector Position;
    C3DVector Velocity;
    float     Radius;
    C3DVector ImpulseVelocity;
    C2DBoxI GetPossibleCollidedWithArea() const
    {
        float margin = 1.0f;
        if (ParentThing->UsingComponents[5] & 0x300)
            margin = 5.0f;
        float moveX = ImpulseVelocity.X + Velocity.X;
        float moveY = ImpulseVelocity.Y + Velocity.Y;
        float radius = margin + Radius;
        float minX = Position.X - radius + Min(moveX, 0.0f);
        float maxX = Position.X + Max(moveX, 0.0f) + radius;
        float minY = Position.Y - radius + Min(moveY, 0.0f);
        float maxY = radius + (Position.Y + Max(moveY, 0.0f));
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
    CThing thing = { { 0 } };
    CTCPhysicsControlled p;
    p.ParentThing = &thing;
    p.Position.X = 100.0f; p.Position.Y = 50.0f; p.Position.Z = 0.0f;
    p.Velocity.X = 3.0f; p.Velocity.Y = -2.0f; p.Velocity.Z = 0.0f;
    p.ImpulseVelocity.X = 1.0f; p.ImpulseVelocity.Y = -2.0f; p.ImpulseVelocity.Z = 0.0f;
    p.Radius = 2.0f;
    // margin 1: radius 3; moveX 4 -> X [97, 107]; moveY -4 -> Y [43, 53]
    C2DBoxI b = p.GetPossibleCollidedWithArea();
    bool ok = b.TopLeft.X == 97 && b.BottomRight.X == 107 && b.TopLeft.Y == 43 && b.BottomRight.Y == 53;
    thing.UsingComponents[5] = 0x200;   // component 169: margin 5 -> radius 7
    b = p.GetPossibleCollidedWithArea();
    ok = ok && b.TopLeft.X == 93 && b.BottomRight.X == 111 && b.TopLeft.Y == 39 && b.BottomRight.Y == 57;
    if (!ok) { std::printf("00725a30_TEST FAIL\n"); return 1; }
    std::printf("00725a30_TEST PASS\n");
    return 0;
}