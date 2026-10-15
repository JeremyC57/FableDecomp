#include <cstdio>
// Behaviour test for CObstacleNavigatorExternalInterface::PeekNavigatorPosition2D @ 0x004DF434
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {} C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {} };
class CTCPhysicsBase { public: virtual ~CTCPhysicsBase() {} C3DVector m_position; };
class CThing { public: CTCPhysicsBase* PhysicsTC; };
class CObstacleNavigatorExternalInterface {
public:
    CThing* Obstacle;
    C2DVector PeekNavigatorPosition2D() const { return C2DVector(Obstacle->PhysicsTC->m_position); }
};
int main()
{
    CTCPhysicsBase physics; physics.m_position.X = 1.5f; physics.m_position.Y = -2.25f; physics.m_position.Z = 8.0f;
    CThing thing; thing.PhysicsTC = &physics;
    CObstacleNavigatorExternalInterface nav; nav.Obstacle = &thing;
    C2DVector p = nav.PeekNavigatorPosition2D();
    if (p.X != 1.5f || p.Y != -2.25f) { std::printf("004df434_TEST FAIL\n"); return 1; }
    std::printf("004df434_TEST PASS\n");
    return 0;
}
