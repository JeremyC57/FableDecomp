#include <cstdio>
#include <cmath>
// Behaviour model of NCameraModes::CBase::UpdateCameraStretchAroundHero 0x0084D910.
struct V { float X, Y, Z; };
struct Hero { V pos; float nomR, curR, nomH, curH; bool mainPlayer; };
static float lerp(float o, float n, float t) { return (n - o) * t + o; }
static void Update(bool stretch, const Hero& h, V& position, V& lookAt)
{
    if (!stretch || !h.mainPlayer) return;
    V off = { lookAt.X - position.X, lookAt.Y - position.Y, lookAt.Z - position.Z };
    float dx = position.X - h.pos.X, dy = position.Y - h.pos.Y, dz = position.Z - h.pos.Z;
    float d = (float)sqrt(dx * dx + dy * dy) - h.curR;
    if (d < 0.0f) d = 0.0f;
    float blend = (float)exp((double)((d * d) * -0.01f));
    float s = lerp(1.0f, h.curR / h.nomR, blend);
    float hz = lerp(1.0f, h.curH / h.nomH, blend) * dz;
    position.X = dx * s + h.pos.X; position.Y = dy * s + h.pos.Y; position.Z = (0.0f * s + h.pos.Z) + hz;
    lookAt.X = off.X + position.X; lookAt.Y = off.Y + position.Y; lookAt.Z = off.Z + position.Z;
}
static bool near(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    Hero h = { {0, 0, 0}, 1.0f, 2.0f, 2.0f, 4.0f, true };
    // inside the radius: d clamps to 0, blend = 1 -> full scale 2 on xy, height scale 2 on z.
    V p = {1, 0, 1}, l = {2, 0, 1};
    Update(true, h, p, l);
    if (!near(p.X, 2) || !near(p.Y, 0) || !near(p.Z, 2) || !near(l.X, 3) || !near(l.Z, 2)) { std::printf("0084d910_TEST FAIL\n"); return 1; }
    // far away: blend ~ 0 -> unchanged.
    V p2 = {100, 0, 3}, l2 = {101, 1, 3};
    Update(true, h, p2, l2);
    if (!near(p2.X, 100) || !near(p2.Z, 3) || !near(l2.Y, 1)) { std::printf("0084d910_TEST FAIL\n"); return 1; }
    // disabled: no change.
    V p3 = {1, 0, 1}, l3 = {2, 0, 1};
    Update(false, h, p3, l3);
    if (!near(p3.X, 1)) { std::printf("0084d910_TEST FAIL\n"); return 1; }
    std::printf("0084d910_TEST PASS\n");
    return 0;
}