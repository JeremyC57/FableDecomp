// Behaviour test for CEngineCamera::ClipWorldSpaceBoundingBox (0x00B308E0): a box wholly outside a side plane or wholly
// beyond the far distance is rejected (fullyInside cleared); a box straddling a plane or the far distance is accepted with
// fullyInside cleared; a box well inside is accepted with fullyInside set.
#include <cstdio>

struct V { float X, Y, Z; };
struct Box { V MinCoord, MaxCoord; };
struct Plane { V Normal; float Distance; };
static float Dot(const V& a, const V& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct Camera {
    V WorldPosition; V LookVector; Plane FrustrumPlanes[4];
    bool Clip(const Box& box, float farDistance, bool& fullyInside) const
    {
        fullyInside = true;
        for (int i = 0; i < 4; ++i) {
            const V& n = FrustrumPlanes[i].Normal;
            V p; p.X = n.X > 0 ? box.MinCoord.X : box.MaxCoord.X; p.Y = n.Y > 0 ? box.MinCoord.Y : box.MaxCoord.Y; p.Z = n.Z > 0 ? box.MinCoord.Z : box.MaxCoord.Z;
            if (Dot(p, n) > FrustrumPlanes[i].Distance) { fullyInside = false; return false; }
            if (fullyInside) {
                V q; q.X = n.X > 0 ? box.MaxCoord.X : box.MinCoord.X; q.Y = n.Y > 0 ? box.MaxCoord.Y : box.MinCoord.Y; q.Z = n.Z > 0 ? box.MaxCoord.Z : box.MinCoord.Z;
                if (Dot(q, n) > FrustrumPlanes[i].Distance) fullyInside = false;
            }
        }
        const V& l = LookVector;
        V p; p.X = l.X > 0 ? box.MinCoord.X : box.MaxCoord.X; p.Y = l.Y > 0 ? box.MinCoord.Y : box.MaxCoord.Y; p.Z = l.Z > 0 ? box.MinCoord.Z : box.MaxCoord.Z;
        V d; d.X = p.X - WorldPosition.X; d.Y = p.Y - WorldPosition.Y; d.Z = p.Z - WorldPosition.Z;
        if (Dot(d, l) > farDistance) { fullyInside = false; return false; }
        if (fullyInside) {
            V q; q.X = l.X > 0 ? box.MaxCoord.X : box.MinCoord.X; q.Y = l.Y > 0 ? box.MaxCoord.Y : box.MinCoord.Y; q.Z = l.Z > 0 ? box.MaxCoord.Z : box.MinCoord.Z;
            V e; e.X = q.X - WorldPosition.X; e.Y = q.Y - WorldPosition.Y; e.Z = q.Z - WorldPosition.Z;
            if (Dot(e, l) > farDistance) fullyInside = false;
        }
        return true;
    }
};
static Box MakeBox(float x0, float y0, float z0, float x1, float y1, float z1)
{
    Box b; b.MinCoord.X = x0; b.MinCoord.Y = y0; b.MinCoord.Z = z0; b.MaxCoord.X = x1; b.MaxCoord.Y = y1; b.MaxCoord.Z = z1; return b;
}
int main()
{
    // Planes "outside when dot > d": x <= 10, -x <= 10, y <= 10, -y <= 10; look +z from the origin.
    Camera c;
    c.WorldPosition.X = c.WorldPosition.Y = c.WorldPosition.Z = 0;
    c.LookVector.X = 0; c.LookVector.Y = 0; c.LookVector.Z = 1;
    float nx[4] = { 1, -1, 0, 0 }, ny[4] = { 0, 0, 1, -1 };
    for (int i = 0; i < 4; ++i) { c.FrustrumPlanes[i].Normal.X = nx[i]; c.FrustrumPlanes[i].Normal.Y = ny[i]; c.FrustrumPlanes[i].Normal.Z = 0; c.FrustrumPlanes[i].Distance = 10; }
    int bad = 0; bool in = false;
    if (!c.Clip(MakeBox(-1, -1, 1, 1, 1, 2), 100, in) || !in) ++bad;          // well inside
    if (!c.Clip(MakeBox(5, -1, 1, 15, 1, 2), 100, in) || in) ++bad;           // straddles x = 10
    if (c.Clip(MakeBox(11, -1, 1, 15, 1, 2), 100, in) || in) ++bad;           // wholly beyond x = 10
    if (c.Clip(MakeBox(-1, -15, 1, 1, -11, 2), 100, in) || in) ++bad;         // wholly beyond y = -10
    if (!c.Clip(MakeBox(-1, -1, 90, 1, 1, 110), 100, in) || in) ++bad;        // straddles the far distance
    if (c.Clip(MakeBox(-1, -1, 101, 1, 1, 110), 100, in) || in) ++bad;        // wholly beyond the far distance
    if (bad == 0) std::printf("00b308e0_TEST PASS\n"); else std::printf("00b308e0_TEST FAIL %d\n", bad);
    return bad;
}