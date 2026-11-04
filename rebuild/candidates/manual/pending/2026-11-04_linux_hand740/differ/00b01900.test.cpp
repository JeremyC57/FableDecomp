// Behaviour test for NParticleEngine::CPSCRenderMesh::AddToBoundingSphere (0x00B01900): a standalone model -- every active particle adds
// a sphere at its interpolated position with radius |size| / 2 (size lerped from the start to the end size over the particle's life),
// a trail adds the segment's midpoint with half its length (at least the trail width); no active particle clears both primitive
// handles and returns false.
#include <cstdio>
#include <cmath>
struct V { float x, y, z; };
struct Sphere { V c; float r; bool any;
    void Add(V p, float rad) {
        if (!any) { c = p; r = rad; any = true; return; }
        float dx = p.x - c.x, dy = p.y - c.y, dz = p.z - c.z, d = std::sqrt(dx * dx + dy * dy + dz * dz);
        if (d + rad <= r) return;
        float nr = (d + rad + r) * 0.5f, k = d > 0 ? (nr - r) / d : 0;
        c.x += dx * k; c.y += dy * k; c.z += dz * k; r = nr;
    }
};
struct Particle { bool active; V pos, trailEnd; float life; };
struct Mesh { float startSize, endSize, trailWidth; bool hasTrail, primHandle, trailHandle;
    bool AddToBoundingSphere(Sphere& s, const Particle* p, int n) {
        bool added = false;
        for (int i = 0; i < n; ++i) {
            if (!p[i].active) continue;
            if (hasTrail) {
                V t = { p[i].trailEnd.x - p[i].pos.x, p[i].trailEnd.y - p[i].pos.y, p[i].trailEnd.z - p[i].pos.z };
                float len = std::sqrt(t.x * t.x + t.y * t.y + t.z * t.z);
                if (trailWidth > len) len = trailWidth;
                V mid = { t.x * 0.5f + p[i].pos.x, t.y * 0.5f + p[i].pos.y, t.z * 0.5f + p[i].pos.z };
                s.Add(mid, len * 0.5f); added = true;
            }
            float size = (endSize - startSize) * p[i].life + startSize;
            float mag = std::sqrt(3.0f * size * size);
            s.Add(p[i].pos, mag * 0.5f); added = true;
        }
        if (!added) { primHandle = false; trailHandle = false; }
        return added;
    }
};
int main()
{
    Mesh m = { 2.0f, 4.0f, 1.0f, false, true, true };
    Particle ps[2] = { { true, { 0, 0, 0 }, { 0, 0, 0 }, 0.0f }, { false, { 100, 0, 0 }, { 0, 0, 0 }, 0.5f } };
    Sphere s = { { 0, 0, 0 }, 0, false };
    bool ok = m.AddToBoundingSphere(s, ps, 2) && std::fabs(s.r - std::sqrt(3.0f)) < 1e-4f && m.primHandle;
    Particle none[1] = { { false, { 0, 0, 0 }, { 0, 0, 0 }, 0 } };
    Sphere s2 = { { 0, 0, 0 }, 0, false };
    ok = ok && !m.AddToBoundingSphere(s2, none, 1) && !m.primHandle && !m.trailHandle;
    Mesh t = { 0.0f, 0.0f, 3.0f, true, true, true };
    Particle tp[1] = { { true, { 0, 0, 0 }, { 2, 0, 0 }, 0.0f } };
    Sphere s3 = { { 0, 0, 0 }, 0, false };
    ok = ok && t.AddToBoundingSphere(s3, tp, 1) && std::fabs(s3.r - 1.5f) < 1e-4f && std::fabs(s3.c.x - 1.0f) < 1e-4f;
    printf(ok ? "00b01900_TEST PASS\n" : "00b01900_TEST FAIL\n");
    return ok ? 0 : 1;
}
