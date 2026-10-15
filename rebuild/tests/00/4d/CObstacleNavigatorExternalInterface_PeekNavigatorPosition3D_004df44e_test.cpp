#include <cstdio>
// Behaviour test for CObstacleNavigatorExternalInterface::PeekNavigatorPosition3D @ 0x004DF44E
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
class CTCPhysicsBase { public: virtual ~CTCPhysicsBase() {} C3DVector m_position; };
class CThing { public: CTCPhysicsBase* PhysicsTC; };
class CObstacleNavigatorExternalInterface {
public:
    CThing* Obstacle;
    C3DVector PeekNavigatorPosition3D() const { return Obstacle->PhysicsTC->m_position; }
};
int main()
{
    CTCPhysicsBase physics; physics.m_position.X = 1.5f; physics.m_position.Y = -2.25f; physics.m_position.Z = 8.0f;
    CThing thing; thing.PhysicsTC = &physics;
    CObstacleNavigatorExternalInterface nav; nav.Obstacle = &thing;
    C3DVector p = nav.PeekNavigatorPosition3D();
    if (p.X != 1.5f || p.Y != -2.25f || p.Z != 8.0f) { std::printf("004df44e_TEST FAIL\n"); return 1; }
    std::printf("004df44e_TEST PASS\n");
    return 0;
}