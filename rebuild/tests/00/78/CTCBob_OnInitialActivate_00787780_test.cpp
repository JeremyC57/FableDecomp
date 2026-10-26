// Behaviour test for 00787780 CTCBob::OnInitialActivate: copies the physics orientation set and position, clears ForceVisible.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Forward, Up; };

class CTCPhysicsBase
{ public:
    CRightHandedSet m_set;
    C3DVector       Position;
    virtual const CRightHandedSet& GetRHSet() const { return m_set; }
    virtual ~CTCPhysicsBase() {}
};

class CThing { public: CTCPhysicsBase* PhysicsTC; };

class CTCBob
{ public:
    CThing*         ParentThing;
    CRightHandedSet Rhset;
    C3DVector       Pos;
    bool            ForceVisible;

    void OnInitialActivate()
    {
        CTCPhysicsBase* physics = ParentThing->PhysicsTC;
        Rhset = physics->GetRHSet();
        Pos = physics->Position;
        ForceVisible = false;
    }
};

int main()
{
    CTCPhysicsBase physics;
    C3DVector f = { 1.0f, 2.0f, 3.0f }, u = { 4.0f, 5.0f, 6.0f }, p = { 7.0f, 8.0f, 9.0f };
    physics.m_set.Forward = f; physics.m_set.Up = u; physics.Position = p;
    CThing thing; thing.PhysicsTC = &physics;
    CTCBob bob; bob.ParentThing = &thing; bob.ForceVisible = true;
    bob.OnInitialActivate();
    if (bob.Rhset.Forward.Y != 2.0f || bob.Rhset.Up.Z != 6.0f || bob.Pos.X != 7.0f || bob.Pos.Z != 9.0f || bob.ForceVisible)
    {
        std::printf("00787780_TEST FAIL\n");
        return 1;
    }
    std::printf("00787780_TEST PASS\n");
    return 0;
}