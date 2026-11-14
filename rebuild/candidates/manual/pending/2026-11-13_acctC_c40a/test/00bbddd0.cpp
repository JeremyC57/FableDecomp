// Behaviour test for 0x00BBDDD0 CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderPrimitive: nothing is rendered when the manager
// is disabled, when the primitive has no world bounding sphere, when the sphere's near edge is beyond the primitive's draw distance or
// when the sphere is outside the frustum (near / far along the look vector, then the side planes); otherwise ONE sub-primitive is
// queued with the camera distance of the sphere centre, flags 1 and the primitive's alpha faded linearly across its fade range
// (0 beyond the draw distance, full alpha before the fade start), and the result is 2.
#include <cstdio>
#include <cmath>
struct V { float x, y, z; };
static float dot(const V& a, const V& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
struct Camera { V pos, look; float nearZ, farZ; V planes[4]; };
struct Prim { bool hasSphere; V centre; float radius; float fadeBegin, maxDraw; unsigned char alpha; };
struct Queue { int count; float distance; unsigned char alpha; unsigned long flags; };
static bool Clip(const Camera& c, const V& centre, float r) {
    V d = { centre.x - c.pos.x, centre.y - c.pos.y, centre.z - c.pos.z };
    float depth = dot(d, c.look);
    if (depth + r < c.nearZ || depth - r > c.farZ || dot(d, c.planes[2]) > r || dot(d, c.planes[3]) > r || dot(d, c.planes[0]) > r || dot(d, c.planes[1]) > r)
        return false;
    return true;
}
static unsigned char Alpha(const Prim& p, float distance) {
    if (p.alpha == 0) return 0;
    if (distance > p.maxDraw) return 0;
    if (distance < p.fadeBegin) return p.alpha;
    int fade = (int)std::floor((distance - p.fadeBegin) * 255.0f / (p.maxDraw - p.fadeBegin) + 0.5f);
    return (unsigned char)(((255 - fade) * p.alpha) >> 8);
}
static int Render(bool enable, const Camera& c, const Prim& p, Queue& q) {
    if (!enable) return 0;
    if (p.hasSphere) {
        V d = { p.centre.x - c.pos.x, p.centre.y - c.pos.y, p.centre.z - c.pos.z };
        float distance = (float)std::sqrt(dot(d, d));
        if (distance - p.radius > p.maxDraw) return 0;
        if (Clip(c, p.centre, p.radius)) { ++q.count; q.distance = distance; q.alpha = Alpha(p, distance); q.flags = 1; return 2; }
    }
    return 0;
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main() {
    Camera c = { { 0, 0, 0 }, { 0, 1, 0 }, 1.0f, 100.0f, { { 1, -1, 0 }, { -1, -1, 0 }, { 0, -1, 1 }, { 0, -1, -1 } } };
    Prim p = { true, { 0, 10, 0 }, 1.0f, 20.0f, 60.0f, 200 };
    Queue q = { 0, 0, 0, 0 };
    check(Render(false, c, p, q) == 0 && q.count == 0, "disabled manager renders nothing");
    check(Render(true, c, p, q) == 2 && q.count == 1 && q.distance == 10.0f && q.alpha == 200 && q.flags == 1, "visible near primitive: full alpha, flags 1");
    p.centre.y = 40.0f; check(Render(true, c, p, q) == 2 && q.count == 2 && q.distance == 40.0f && q.alpha == 99, "half-way through the fade range");
    p.centre.y = 60.5f; check(Render(true, c, p, q) == 2 && q.count == 3 && q.alpha == 0, "centre beyond the draw distance but the edge inside: alpha 0");
    p.centre.y = 62.0f; check(Render(true, c, p, q) == 0 && q.count == 3, "edge beyond the draw distance: nothing");
    p.centre.y = 10.0f; p.hasSphere = false; check(Render(true, c, p, q) == 0 && q.count == 3, "no bounding sphere: nothing");
    p.hasSphere = true; p.centre.y = -5.0f; check(Render(true, c, p, q) == 0 && q.count == 3, "behind the near plane: nothing");
    p.centre.y = 10.0f; p.centre.x = 30.0f; check(Render(true, c, p, q) == 0 && q.count == 3, "outside a side plane: nothing");
    p.centre.x = 0.0f; p.alpha = 0; check(Render(true, c, p, q) == 2 && q.count == 4 && q.alpha == 0, "zero alpha still queues with alpha 0");
    if (!fails) std::printf("00bbddd0_TEST PASS\n");
    return fails;
}
