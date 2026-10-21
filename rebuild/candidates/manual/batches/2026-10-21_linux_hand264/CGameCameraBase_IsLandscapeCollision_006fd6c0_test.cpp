// Behaviour test for CGameCameraBase::IsLandscapeCollision @ 0x006FD6C0: march from the camera towards its look-at
// point in quarter steps of the unit direction while more than 1 unit away; if any ground sample is above the camera,
// move the camera to the highest such sample's x/y at 2 units above it and report true.
#include <cstdio>
#include <cmath>
struct V3 { float x, y, z; };
static float Ground(const V3& p) { return (p.x > 2.0f && p.x < 3.0f) ? 5.0f : 0.0f; }
static bool Collide(V3& cam, const V3& target)
{
    V3 start = cam, d = { target.x - start.x, target.y - start.y, target.z - start.z };
    float len = (float)std::sqrt(d.x * d.x + (d.y * d.y + d.z * d.z));
    V3 dir = { d.x / len, d.y / len, d.z / len }, step = { dir.x * 0.25f, dir.y * 0.25f, dir.z * 0.25f };
    V3 pos = start, best = start;
    if (len > 1.0f) {
        for (;;) {
            float h = Ground(pos);
            if (h > start.z) { best = pos; best.z = h; }
            pos.x += step.x; pos.y += step.y; pos.z += step.z;
            V3 r = { target.x - pos.x, target.y - pos.y, target.z - pos.z };
            if (!((float)std::sqrt(r.x * r.x + (r.y * r.y + r.z * r.z)) > 1.0f)) break;
        }
    }
    if (best.z > start.z) { cam.x = best.x; cam.y = best.y; cam.z = best.z + 2.0f; return true; }
    return false;
}
int main()
{
    V3 cam = { 0, 0, 1 }, target = { 10, 0, 1 };
    if (!Collide(cam, target) || cam.z != 7.0f || !(cam.x > 2.0f && cam.x < 3.0f)) { std::printf("006fd6c0_TEST FAIL\n"); return 1; }
    V3 cam2 = { 0, 0, 6 };
    if (Collide(cam2, target) || cam2.z != 6.0f) { std::printf("006fd6c0_TEST FAIL\n"); return 1; }
    V3 cam3 = { 0, 0, 1 }, closeTarget = { 0.5f, 0, 1 };
    if (Collide(cam3, closeTarget)) { std::printf("006fd6c0_TEST FAIL\n"); return 1; }
    std::printf("006fd6c0_TEST PASS\n");
    return 0;
}
