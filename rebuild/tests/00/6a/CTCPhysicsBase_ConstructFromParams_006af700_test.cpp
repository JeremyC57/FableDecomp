#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct CTCPhysicsBaseParams { long m_header; C3DVector Position; };
class CTCPhysicsBase { public:
    C3DVector Position;
    C3DVector OldPosition;
    void ConstructFromParams(const CTCPhysicsBaseParams& params)
    {
        const C3DVector& position = params.Position;
        OldPosition = position;
        Position = position;
    }
};
int main()
{
    CTCPhysicsBaseParams p; p.m_header = 7; p.Position.X = 1.0f; p.Position.Y = 2.0f; p.Position.Z = 3.0f;
    CTCPhysicsBase b; b.Position.X = b.Position.Y = b.Position.Z = 0.0f; b.OldPosition = b.Position;
    b.ConstructFromParams(p);
    if (b.Position.X != 1.0f || b.Position.Y != 2.0f || b.Position.Z != 3.0f || b.OldPosition.X != 1.0f || b.OldPosition.Z != 3.0f) { std::printf("006af700_TEST FAIL\n"); return 1; }
    std::printf("006af700_TEST PASS\n");
    return 0;
}