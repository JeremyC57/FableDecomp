// Behaviour test for CTCCarryable::UpdateCarriedPosition (0x00705B60): dead things and failed lookups do nothing; the
// orientation is always applied after a successful lookup; the position only when on the map and off by > 1e-4 on an axis.
#include <cstdio>
#include <math.h>
float g_epsilon_0129ba3c = 1e-4f;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CTCPhysicsBase {
    int PosCalls, FaceCalls; C3DVector Pos; float FaceUpX;
    virtual void SetPosition(const C3DVector& position) { ++PosCalls; Pos = position; }
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& set) { ++FaceCalls; FaceUpX = set.Up.X; }
};
struct CThing {
    CTCPhysicsBase* PhysicsTC;
    unsigned char   Dead : 1;
    unsigned char   _91_1 : 7;
    C3DVector Current;
    const C3DVector& GetPosition() const { return Current; }
};
struct CWorldMap {
    bool InMap;
    bool IsPosInMap(const C3DVector& position) const { return InMap; }
};
static CWorldMap g_map;
struct CTCCarryable {
    virtual void v00() {}
    CThing* ParentThing;
    bool Found; C3DVector Carried; bool LastForce; float LastTime;
    const CWorldMap& PeekWorldMap() const { return g_map; }
    bool GetCarriedPositionInfo(C3DVector* position, CRightHandedSet* orientation, bool forceUpdate, float time)
    {
        LastForce = forceUpdate; LastTime = time;
        if (!Found) return false;
        *position = Carried; orientation->Up.X = 0.5f;
        return true;
    }
    void UpdateCarriedPosition(bool forceUpdate);
};

void CTCCarryable::UpdateCarriedPosition(bool forceUpdate)
{
    if (ParentThing->Dead)
        return;
    CRightHandedSet orientation;
    C3DVector position;
    if (!GetCarriedPositionInfo(&position, &orientation, forceUpdate, 1.0f))
        return;
    ParentThing->PhysicsTC->SetFacingAnglesFromRHSet(orientation);
    if (!PeekWorldMap().IsPosInMap(position))
        return;
    const C3DVector& current = ParentThing->GetPosition();
    if ((float)fabs(current.X - position.X) > g_epsilon_0129ba3c
     || (float)fabs(current.Y - position.Y) > g_epsilon_0129ba3c
     || (float)fabs(current.Z - position.Z) > g_epsilon_0129ba3c)
        ParentThing->PhysicsTC->SetPosition(position);
}

int main()
{
    int bad = 0;
    CTCPhysicsBase phys; phys.PosCalls = phys.FaceCalls = 0;
    CThing thing; thing.PhysicsTC = &phys; thing.Dead = 0; thing._91_1 = 0;
    thing.Current.X = 1.0f; thing.Current.Y = 2.0f; thing.Current.Z = 3.0f;
    CTCCarryable c; c.ParentThing = &thing; c.Found = true; c.Carried = thing.Current; g_map.InMap = true;
    c.UpdateCarriedPosition(true);                   // same position: orientation only
    if (phys.FaceCalls != 1 || phys.PosCalls != 0 || !c.LastForce || c.LastTime != 1.0f || phys.FaceUpX != 0.5f) bad |= 1;
    c.Carried.Z = 3.5f;
    c.UpdateCarriedPosition(false);                  // z differs: moved
    if (phys.FaceCalls != 2 || phys.PosCalls != 1 || phys.Pos.Z != 3.5f || c.LastForce) bad |= 2;
    c.Carried.Z = 3.0f; c.Carried.X = 0.9f;
    c.UpdateCarriedPosition(false);                  // x differs (negative delta)
    if (phys.PosCalls != 2 || phys.Pos.X != 0.9f) bad |= 4;
    c.Carried.X = 1.00001f;
    c.UpdateCarriedPosition(false);                  // within epsilon
    if (phys.PosCalls != 2) bad |= 8;
    c.Carried.Y = 5.0f; g_map.InMap = false;
    c.UpdateCarriedPosition(false);                  // off the map: no move
    if (phys.PosCalls != 2 || phys.FaceCalls != 5) bad |= 16;
    g_map.InMap = true; c.Found = false;
    c.UpdateCarriedPosition(false);                  // lookup failed
    if (phys.FaceCalls != 5 || phys.PosCalls != 2) bad |= 32;
    c.Found = true; thing.Dead = 1;
    c.UpdateCarriedPosition(false);                  // dead
    if (phys.FaceCalls != 5 || phys.PosCalls != 2) bad |= 64;
    if (bad) { std::printf("00705b60_TEST FAIL\n"); return 1; }
    std::printf("00705b60_TEST PASS\n");
    return 0;
}