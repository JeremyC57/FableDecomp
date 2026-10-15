#include <cstdio>
#include <cmath>
// Behaviour test for CObstacleNavigatorExternalInterface::PeekNavigatorOrientation @ 0x004DF3F7
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
    C2DVector* Normalise() { float l = (float)std::sqrt(X * X + Y * Y); if (l > 0.0f) { X /= l; Y /= l; } return this; }
};
class CTCPhysicsBase { public: C3DVector m_forward; CTCPhysicsBase() : m_forward(3.0f, 4.0f, 9.0f) {} virtual C3DVector GetForward() const { return m_forward; } };
class CThing { public: CTCPhysicsBase* PhysicsTC; };
class CObstacleNavigatorExternalInterface {
public:
    CThing* Obstacle;
    C2DVector PeekNavigatorOrientation() const
    {
        C2DVector direction(Obstacle->PhysicsTC->GetForward());
        direction.Normalise();
        return direction;
    }
};
int main()
{
    CTCPhysicsBase physics; CThing thing; thing.PhysicsTC = &physics;
    CObstacleNavigatorExternalInterface nav; nav.Obstacle = &thing;
    C2DVector d = nav.PeekNavigatorOrientation();
    if (std::fabs(d.X - 0.6f) > 1e-5f || std::fabs(d.Y - 0.8f) > 1e-5f) { std::printf("004df3f7_TEST FAIL\n"); return 1; }
    std::printf("004df3f7_TEST PASS\n");
    return 0;
}