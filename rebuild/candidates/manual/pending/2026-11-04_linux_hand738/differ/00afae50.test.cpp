// Behaviour test for NParticleEngine::CPSCRenderSprite::AddToBoundingSphere (0x00AFAE50): a standalone model -- with neither bank
// nothing is added (false); each active particle adds its trail as the sphere through the midpoint of pos..tail with radius
// max(|tail - pos|, trailWidth) / 2, and its sprite as a sphere at pos with radius |(w, w * aspect)| / 2 where w lerps the start / end
// render sizes by the animation fraction; a particle past its MaxLife is cleared instead.
#include <cstdio>
#include <cmath>
#include <vector>
struct V { float x, y, z; };
struct Sphere { std::vector<V> c; std::vector<float> r; void Add(V p, float rad) { c.push_back(p); r.push_back(rad); } };
struct Particle { bool active; V pos, tail; float life, maxLife; bool cleared; };
static bool Bound(long sprite, long trail, float startSize, float endSize, float anim, float aspect, float trailWidth,
                  std::vector<Particle>& ps, Sphere& s)
{
    if (!sprite && !trail) return false;
    bool added = false;
    for (size_t i = 0; i < ps.size(); ++i) {
        Particle& p = ps[i];
        if (p.active) {
            if (trail) {
                V d = { p.tail.x - p.pos.x, p.tail.y - p.pos.y, p.tail.z - p.pos.z };
                float len = std::sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
                if (trailWidth > len) len = trailWidth;
                V mid = { p.pos.x + d.x * 0.5f, p.pos.y + d.y * 0.5f, p.pos.z + d.z * 0.5f };
                s.Add(mid, len * 0.5f); added = true;
            }
            if (sprite) {
                float w = (endSize - startSize) * anim + startSize, h = w * aspect;
                s.Add(p.pos, std::sqrt(h * h + w * w) * 0.5f); added = true;
            }
        } else if (p.life > p.maxLife)
            p.cleared = true;
    }
    return added;
}
int main()
{
    std::vector<Particle> ps(2);
    Particle a = { true, { 0, 0, 0 }, { 0, 4, 0 }, 1, 10, false }; Particle b = { false, { 0, 0, 0 }, { 0, 0, 0 }, 12, 10, false };
    ps[0] = a; ps[1] = b;
    Sphere s;
    bool ok = !Bound(0, 0, 1, 2, 0.5f, 1, 0.5f, ps, s) && s.r.empty();
    ok = ok && Bound(5, 6, 1.0f, 3.0f, 0.5f, 0.75f, 0.5f, ps, s) && s.r.size() == 2;
    ok = ok && std::fabs(s.c[0].y - 2.0f) < 1e-6f && std::fabs(s.r[0] - 2.0f) < 1e-6f;          // trail: midpoint, |d| / 2
    ok = ok && std::fabs(s.r[1] - 1.25f) < 1e-6f && ps[1].cleared;                             // sprite: w 2, h 1.5 -> 2.5 / 2
    printf(ok ? "00afae50_TEST PASS\n" : "00afae50_TEST FAIL\n");
    return ok ? 0 : 1;
}
