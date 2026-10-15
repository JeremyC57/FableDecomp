// Behaviour test for CProjectileWeapon::GetPhiForTargetedThing (0x008798C0): the targeting position at full height
// (1.0) in the mode's world is handed to GetPhiForPosition.
#include <cstdio>
struct CWorld { float offset; };
struct CThing { float x, y, z; };
struct C3DVector { float X, Y, Z; C3DVector() { } };
static float g_height = 0.0f; static const CWorld* g_world = 0;
static C3DVector GetTargetingPositionOnThing(const CWorld& w, const CThing& t, float h)
{
    g_world = &w; g_height = h; C3DVector v; v.X = t.x + w.offset; v.Y = t.y; v.Z = t.z * h; return v;
}
struct CProjectileWeapon {
    CWorld world;
    const CWorld& GetWorld() const { return world; }
    float GetPhiForPosition(const C3DVector& p) const { return p.X * 2.0f + p.Y + p.Z; }
    float GetPhiForTargetedThing(const CThing& thing) const
    {
        C3DVector position = GetTargetingPositionOnThing(GetWorld(), thing, 1.0f);
        return GetPhiForPosition(position);
    }
};
int main()
{
    CProjectileWeapon w; w.world.offset = 4.0f;
    CThing t; t.x = 1.0f; t.y = 2.0f; t.z = 3.0f;
    float r = w.GetPhiForTargetedThing(t);
    if (r != 15.0f || g_height != 1.0f || g_world != &w.world) { std::printf("008798c0_TEST FAIL\n"); return 1; }
    std::printf("008798c0_TEST PASS\n");
    return 0;
}