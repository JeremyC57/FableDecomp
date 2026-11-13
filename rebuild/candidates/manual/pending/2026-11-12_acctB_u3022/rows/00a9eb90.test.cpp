// Behaviour test for 0x00A9EB90 C3DAnimationManager::BlendAnimations(C3DKeyframe* from, const C3DKeyframe* to, unsigned long count, float t):
// host model. Positions are lerped (from + (to - from) * t); rotations are slerped with the engine's weights (shortest arc: the second
// weight is negated when the dot product is negative; plain lerp weights within 0.05 of parallel). The SSE path (pool weights finished by
// GFFinishPartialSlerp: from = w.X * from + w.Y * to) and the scalar SetSlerp path give the same result; the blend statistics count the
// keyframes. The table-driven fast arc-cosine / sine are modelled with acos / sin (the engine tables approximate them).
#include <cstdio>
#include <cmath>

struct Quat { float X, Y, Z, W; };
struct V4 { float X, Y, Z, W; };
struct Key { Quat Rotation; V4 Position; V4 Scaling; };

static void Weights(Quat& w, const Quat& a, const Quat& b, float t)
{
    float cosom = a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
    float sign = 1.0f;
    if (cosom < 0.0f) { sign = -1.0f; cosom = -cosom; }
    if (1.0f - cosom > 0.05f) {
        float omega = (float)std::acos(cosom);
        float sinom = (float)std::sqrt(1.0f - cosom * cosom);
        w.X = (float)std::sin((1.0f - t) * omega) / sinom;
        w.Y = (float)std::sin(omega * t) / sinom;
    } else { w.X = 1.0f - t; w.Y = t; }
    w.Y = sign * w.Y;
}
static unsigned long g_Keyframes;
static void Blend(Key* from, const Key* to, unsigned long count, float t, bool sse)     // 0x00A9EB90
{
    for (unsigned long i = 0; i < count; ++i) {
        V4& p = from[i].Position; const V4& q = to[i].Position;
        p.X += (q.X - p.X) * t; p.Y += (q.Y - p.Y) * t; p.Z += (q.Z - p.Z) * t; p.W += (q.W - p.W) * t;
    }
    if (sse) {
        Quat weights[8];
        for (unsigned long i = 0; i < count; ++i) Weights(weights[i], from[i].Rotation, to[i].Rotation, t);
        for (unsigned long i = 0; i < count; ++i) {
            Quat& r = from[i].Rotation; const Quat& o = to[i].Rotation; float a = weights[i].X, b = weights[i].Y;
            r.X = r.X * a + o.X * b; r.Y = r.Y * a + o.Y * b; r.Z = r.Z * a + o.Z * b; r.W = r.W * a + o.W * b;
        }
    } else {
        for (unsigned long i = 0; i < count; ++i) {
            Quat w; Weights(w, from[i].Rotation, to[i].Rotation, t);
            Quat& r = from[i].Rotation; const Quat& o = to[i].Rotation; Quat n;
            n.X = w.X * r.X + w.Y * o.X; n.Y = w.X * r.Y + w.Y * o.Y; n.Z = w.X * r.Z + w.Y * o.Z; n.W = w.X * r.W + w.Y * o.W;
            r = n;
        }
    }
    g_Keyframes += count;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    int fails = 0;
    const float h = 0.70710678f;
    for (int pass = 0; pass < 2; ++pass) {
        Key from[3], to[3];
        Quat id = { 0, 0, 0, 1 }, z90 = { 0, 0, h, h }, negid = { 0, 0, 0, -1 };
        V4 p0 = { 0, 0, 0, 1 }, p1 = { 10, 20, -4, 1 };
        from[0].Rotation = id; to[0].Rotation = z90;
        from[1].Rotation = id; to[1].Rotation = negid;            // same rotation, opposite sign: shortest arc keeps it
        from[2].Rotation = z90; to[2].Rotation = z90;
        for (int i = 0; i < 3; ++i) { from[i].Position = p0; to[i].Position = p1; }
        Blend(from, to, 3, 0.5f, pass == 0);
        // 0 -> 90 degrees about Z at t = 0.5 is 45 degrees: (0, 0, sin 22.5, cos 22.5).
        if (!Near(from[0].Rotation.Z, 0.38268343f) || !Near(from[0].Rotation.W, 0.92387953f)) ++fails;
        if (!Near(from[1].Rotation.W, 1.0f) || !Near(from[1].Rotation.Z, 0.0f)) ++fails;
        if (!Near(from[2].Rotation.Z, h) || !Near(from[2].Rotation.W, h)) ++fails;
        if (!Near(from[1].Position.X, 5.0f) || !Near(from[1].Position.Y, 10.0f) || !Near(from[1].Position.Z, -2.0f)) ++fails;
    }
    if (g_Keyframes != 6) ++fails;
    if (fails) { std::printf("00a9eb90_TEST FAIL\n"); return 1; }
    std::printf("00a9eb90_TEST PASS\n");
    return 0;
}
