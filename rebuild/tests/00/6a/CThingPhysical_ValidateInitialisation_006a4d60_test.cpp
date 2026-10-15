#include <cstdio>
// Behaviour test for CThingPhysical::ValidateInitialisation (0x006A4D60).
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase { C3DVector Position; };
struct CWorldMap {
    float size; int calls;
    bool IsPosInMap(const C3DVector& p) { ++calls; return p.X >= 0 && p.Y >= 0 && p.X < size && p.Y < size; }
};
struct CThingPhysical {
    bool positioned; CTCPhysicsBase* PhysicsTC; CWorldMap* map;
    bool IsPositionedThing() { return positioned; }
    CWorldMap* GetWorldMap() { return map; }
    bool ValidateInitialisation()
    {
        if (IsPositionedThing() && PhysicsTC != 0) { if (!GetWorldMap()->IsPosInMap(PhysicsTC->Position)) return false; }
        return true;
    }
};
int main()
{
    CWorldMap m; m.size = 100.0f; m.calls = 0;
    CTCPhysicsBase phys; phys.Position.X = 10; phys.Position.Y = 20; phys.Position.Z = 0;
    CThingPhysical t; t.map = &m; t.PhysicsTC = &phys; t.positioned = true;
    int fails = 0;
    if (!t.ValidateInitialisation()) ++fails;
    phys.Position.X = 150; if (t.ValidateInitialisation()) ++fails;
    t.positioned = false; if (!t.ValidateInitialisation()) ++fails;
    t.positioned = true; t.PhysicsTC = 0; if (!t.ValidateInitialisation()) ++fails;
    if (m.calls != 2) ++fails;
    if (fails) { std::printf("006a4d60_TEST FAIL\n"); return 1; }
    std::printf("006a4d60_TEST PASS\n");
    return 0;
}