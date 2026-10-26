// Behaviour test for 00942230 (catalog GetActiveCameraPosition): the sphere takes the thing's position and its physics radius.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class CTCPhysicsBase { public: float Radius; };

class CThing
{ public:
    CTCPhysicsBase* PhysicsTC;
    C3DVector       m_pos;
    const C3DVector& GetPosition() const { return m_pos; }
};

struct CThingBoundingSphere
{
    float CentreX, CentreY, CentreZ, Radius;
    CThingBoundingSphere(const CThing* thing)
    {
        const C3DVector& position = thing->GetPosition();
        CentreX = position.X;
        CentreY = position.Y;
        CentreZ = position.Z;
        Radius = thing->PhysicsTC->Radius;
    }
};

int main()
{
    CTCPhysicsBase physics; physics.Radius = 2.5f;
    CThing thing; thing.PhysicsTC = &physics; thing.m_pos.X = 1.0f; thing.m_pos.Y = -2.0f; thing.m_pos.Z = 3.0f;
    CThingBoundingSphere s(&thing);
    if (s.CentreX != 1.0f || s.CentreY != -2.0f || s.CentreZ != 3.0f || s.Radius != 2.5f) { std::printf("00942230_TEST FAIL\n"); return 1; }
    std::printf("00942230_TEST PASS\n");
    return 0;
}