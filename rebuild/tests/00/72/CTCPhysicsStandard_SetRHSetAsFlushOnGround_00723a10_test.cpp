// Behaviour test for CTCPhysicsStandard::SetRHSetAsFlushOnGround (0x00723A10): the world map's ground face normal at Position is
// handed to the virtual SetRHSetAsFlushOnNormal. Self-contained host model (test-only shells of the real classes).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
class CWorldMap {
public:
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos) const { C3DVector n; n.X = pos.X * 0.0f; n.Y = pos.Y; n.Z = 1.0f; return n; }
};
static CWorldMap g_WorldMap;
class CTCPhysicsStandard {
public:
    CTCPhysicsStandard() : Calls(0) { Position.X = 3.0f; Position.Y = 4.0f; Position.Z = 5.0f; }
    virtual ~CTCPhysicsStandard() { }
    virtual void SetRHSetAsFlushOnNormal(const C3DVector& normal) { Seen = normal; ++Calls; }
    virtual void SetRHSetAsFlushOnGround()
    {
        C3DVector normal = PeekWorldMap().GetGroundFaceNormalAt(Position);
        SetRHSetAsFlushOnNormal(normal);
    }
    const CWorldMap& PeekWorldMap() const { return g_WorldMap; }
    C3DVector Position;
    C3DVector Seen;
    int Calls;
};
int main()
{
    CTCPhysicsStandard physics;
    physics.SetRHSetAsFlushOnGround();
    if (physics.Calls != 1 || physics.Seen.Y != 4.0f || physics.Seen.Z != 1.0f) { std::printf("CTCPhysicsStandard_00723a10_TEST FAIL\n"); return 1; }
    std::printf("CTCPhysicsStandard_00723a10_TEST PASS\n");
    return 0;
}
