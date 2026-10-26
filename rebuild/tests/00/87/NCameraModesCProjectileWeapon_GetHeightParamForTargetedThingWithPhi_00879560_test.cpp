// Behaviour test for CProjectileWeapon::GetHeightParamForTargetedThingWithPhi (0x00879560): the targeting position
// is taken at full height (1.0) in the mode's world and handed with phi to GetHeightParamForPostionWithPhi.
#include <cstdio>
class CWorld { public: float offset; };
class CThing { public: float x, y, z; };
class C3DVector { public: float X, Y, Z; C3DVector() { } };
static float g_height = 0.0f; static const CWorld* g_world = 0;
static C3DVector GetTargetingPositionOnThing(const CWorld& w, const CThing& t, float h)
{
    g_world = &w; g_height = h; C3DVector v; v.X = t.x + w.offset; v.Y = t.y; v.Z = t.z * h; return v;
}
struct CProjectileWeapon {
    CWorld world;
    const CWorld& GetWorld() const { return world; }
    float GetHeightParamForPostionWithPhi(const C3DVector& p, float phi) const { return p.X + p.Y + p.Z + phi; }
    float GetHeightParamForTargetedThingWithPhi(const CThing& thing, float phi) const
    {
        C3DVector position = GetTargetingPositionOnThing(GetWorld(), thing, 1.0f);
        return GetHeightParamForPostionWithPhi(position, phi);
    }
};
int main()
{
    CProjectileWeapon w; w.world.offset = 10.0f;
    CThing t; t.x = 1.0f; t.y = 2.0f; t.z = 3.0f;
    float r = w.GetHeightParamForTargetedThingWithPhi(t, 0.5f);
    if (r != 16.5f || g_height != 1.0f || g_world != &w.world) { std::printf("00879560_TEST FAIL\n"); return 1; }
    std::printf("00879560_TEST PASS\n");
    return 0;
}