// Behaviour test for CThingAICreature::GetTargetPosition @ 0x006A9520: position raised by the physics height.
#include <cstdio>
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class CTCPhysicsBase { public: float m_height; };
class CThingAICreature { public:
    CTCPhysicsBase* PhysicsTC; C3DVector m_position;
    const C3DVector& GetPosition() const { return m_position; }
    C3DVector GetTargetPosition() const
    {
        const C3DVector& position = GetPosition();
        C3DVector target;
        target.X = position.X;
        target.Y = position.Y;
        target.Z = PhysicsTC->m_height + position.Z;
        return target;
    }
};
int main()
{
    CTCPhysicsBase p; p.m_height = 1.5f;
    CThingAICreature c; c.PhysicsTC = &p; c.m_position.X = 10.0f; c.m_position.Y = -2.0f; c.m_position.Z = 0.25f;
    C3DVector r = c.GetTargetPosition();
    if (r.X != 10.0f || r.Y != -2.0f || r.Z != 1.75f || c.m_position.Z != 0.25f) { std::printf("006a9520_TEST FAIL\n"); return 1; }
    std::printf("006a9520_TEST PASS\n");
    return 0;
}