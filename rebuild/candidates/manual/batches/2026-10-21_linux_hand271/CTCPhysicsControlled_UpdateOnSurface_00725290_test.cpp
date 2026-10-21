// Behaviour test for CTCPhysicsControlled::UpdateOnSurface (0x00725290): the highest mesh hit (normal not flat-zero) lifts
// pos.Z to the hit minus 0.199 and sets OnMesh / the thing stood on; then the ground can only raise pos.Z, OnGround is
// |pos.Z - ground| < 0.05, and the render offset is -0.055 on a mesh else 0.
#include <cstdio>
#include <cmath>

struct Hit { bool solid, hasMesh, hits; float hitZ, normalZ; };
struct Phys { bool solid, follower; float radius; bool onMesh, onGround; float renderZ, lastRenderZ; int standingOn; };
static void UpdateOnSurface(Phys& p, const Hit* things, int count, float& posZ, float groundZ)
{
    p.lastRenderZ = p.renderZ;
    int standingOn = -1;
    if (p.solid || p.follower) {
        float radius = p.radius;
        p.onMesh = false;
        if (radius < 0.5f) radius = 0.5f;
        for (int i = 0; i < count; ++i) {
            const Hit& h = things[i];
            if (h.solid && h.hasMesh && h.hits && h.hitZ <= posZ + radius && h.hitZ >= posZ - radius) {
                if ((float)std::fabs(h.normalZ) > 1e-4f) {
                    if (h.hitZ > posZ) posZ = h.hitZ - 0.199f;
                    p.onMesh = true;
                    standingOn = i;
                }
            }
        }
    }
    if (groundZ > posZ) posZ = groundZ;
    p.onGround = (float)std::fabs(posZ - groundZ) < 0.05f;
    p.standingOn = standingOn;
    p.renderZ = p.onMesh ? -0.055f : 0.0f;
}
int main()
{
    Hit things[2] = { { true, true, true, 10.3f, 1.0f }, { true, false, true, 11.0f, 1.0f } };
    Phys p; p.solid = true; p.follower = false; p.radius = 0.2f; p.onMesh = false; p.onGround = false; p.renderZ = 0.5f;
    p.lastRenderZ = 0.0f; p.standingOn = -1;
    float z = 10.0f;
    UpdateOnSurface(p, things, 2, z, 0.0f);
    if (std::fabs(z - (10.3f - 0.199f)) > 1e-5f || !p.onMesh || p.standingOn != 0 || p.onGround || p.renderZ != -0.055f ||
        p.lastRenderZ != 0.5f) { std::printf("00725290_TEST FAIL\n"); return 1; }
    p.solid = false; z = 1.0f;
    UpdateOnSurface(p, things, 2, z, 1.02f);
    if (z != 1.02f || !p.onGround || p.standingOn != -1 || !p.onMesh) { std::printf("00725290_TEST FAIL\n"); return 1; }
    std::printf("00725290_TEST PASS\n");
    return 0;
}
