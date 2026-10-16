// behaviour test for 0x0075C210 CTCCameraPointDefinitionBase::SetCoordSystem
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CPhysics { CRightHandedSet o; virtual const CRightHandedSet& GetOrientation() const { return o; } virtual ~CPhysics() {} };
struct CThing { C3DVector pos; CPhysics* m_pPhysics; const C3DVector& GetPosition() const { return pos; } };
struct CP {
    C3DVector CoordBase; CRightHandedSet CoordAxis; bool UsingRelativeCoords, UsingRelativeOrientation;
    void SetCoordSystem(CThing* thing)
    {
        if (UsingRelativeCoords) CoordBase = thing->GetPosition();
        if (UsingRelativeOrientation) CoordAxis = thing->m_pPhysics->GetOrientation();
    }
};
int main()
{
    CPhysics ph; ph.o.Up.X = 0; ph.o.Up.Y = 0; ph.o.Up.Z = 1; ph.o.Forward.X = 1; ph.o.Forward.Y = 2; ph.o.Forward.Z = 3;
    CThing t; t.pos.X = 4; t.pos.Y = 5; t.pos.Z = 6; t.m_pPhysics = &ph;
    CP c; c.CoordBase.X = c.CoordBase.Y = c.CoordBase.Z = 9;
    c.CoordAxis = CRightHandedSet(); c.CoordAxis.Forward.Y = 7;
    c.UsingRelativeCoords = false; c.UsingRelativeOrientation = true;
    c.SetCoordSystem(&t);
    bool ok = c.CoordBase.X == 9 && c.CoordAxis.Forward.Y == 2 && c.CoordAxis.Up.Z == 1;
    c.UsingRelativeCoords = true; c.UsingRelativeOrientation = false; ph.o.Forward.Y = 8;
    c.SetCoordSystem(&t);
    ok = ok && c.CoordBase.Y == 5 && c.CoordBase.Z == 6 && c.CoordAxis.Forward.Y == 2;
    if (!ok) { std::printf("0075c210_TEST FAIL\n"); return 1; }
    std::printf("0075c210_TEST PASS\n");
    return 0;
}