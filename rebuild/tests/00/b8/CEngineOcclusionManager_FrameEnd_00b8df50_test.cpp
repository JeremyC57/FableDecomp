// Behaviour test for 0x00B8DF50 CEngineOcclusionManager::FrameEnd: a host model of the row's logic (capture block, shader enables,
// depth / colour write states, per-flare sphere clip, world transform, expected pixel count, tri-fan draw, restores).
#include <cstdio>
#include <cmath>

struct Vec3 { float X, Y, Z; };
struct Mat34 { float E[12]; };                                  // E11..E33 rows, then E41 E42 E43
struct FlareEntry { Vec3 Position; float Radius; unsigned int CircleSegments; unsigned int MaxPixelCount; unsigned int OccluderHandle; };

struct RenderState { int CaptureDepth; unsigned int ZWrite; unsigned int ColourWrite; };
struct ShaderManager { bool VSEnabled, PSEnabled; int VSApplied; int Prepared; Mat34 World; };
struct Camera {
    Vec3 Pos, Look; float NearZ, FarZ; Vec3 PlaneN[4]; Mat34 View, InvNoScale; float ScaleX, ScaleY; int Refreshed;
    bool ClipSphere(const Vec3& c, float r) const {
        Vec3 d = { c.X - Pos.X, c.Y - Pos.Y, c.Z - Pos.Z };
        float depth = d.X * Look.X + d.Y * Look.Y + d.Z * Look.Z;
        if (depth + r < NearZ || depth - r > FarZ) return false;
        static const int order[4] = { 2, 3, 0, 1 };
        for (int k = 0; k < 4; ++k) {
            const Vec3& n = PlaneN[order[k]];
            if (d.X * n.X + d.Y * n.Y + d.Z * n.Z > r) return false;
        }
        return true;
    }
    float ViewZ(const Vec3& w) const { return View.E[8] * w.Z + View.E[2] * w.X + View.E[5] * w.Y + View.E[11]; }
};

struct World {
    RenderState rs; ShaderManager sm; Camera cam; long dimX, dimY;
    int draws; unsigned int lastSegments; int selected; int stateBlocks;
};

static long RoundNear(float v) { return (long)floor(v + 0.5f); }   // fistp under round-to-nearest (no ties in the test data)

static void FrameEnd(World& w, FlareEntry* tests, unsigned int count)
{
    ++w.rs.CaptureDepth;                                           // scoped capture block
    if (count != 0) {
        bool vsOld = w.sm.VSEnabled; w.sm.VSEnabled = true;
        ++w.sm.VSApplied;
        bool psOld = w.sm.PSEnabled; w.sm.PSEnabled = false;
        ++w.stateBlocks;
        w.rs.ZWrite = 0; w.rs.ColourWrite = 0;
        for (unsigned int i = 0; i < count; ++i) {
            FlareEntry& e = tests[i];
            if (w.cam.ClipSphere(e.Position, e.Radius)) {
                Mat34 m = w.cam.InvNoScale;
                m.E[9] = e.Position.X; m.E[10] = e.Position.Y; m.E[11] = e.Position.Z;
                for (int k = 0; k < 9; ++k) m.E[k] *= e.Radius;
                w.sm.World = m;
                float z = w.cam.ViewZ(e.Position);
                if (z > 1e-4f) {
                    float scale = e.Radius / z;
                    float rx = (float)w.dimX * w.cam.ScaleX * scale * 0.5f;
                    float ry = (float)w.dimY * w.cam.ScaleY * scale * 0.5f;
                    e.MaxPixelCount = (unsigned int)RoundNear(rx * ry * 3.1415927f);
                }
                ++w.selected; w.lastSegments = e.CircleSegments; ++w.sm.Prepared; ++w.draws;
            } else {
                e.MaxPixelCount = 1;
            }
        }
        w.rs.ZWrite = 1; w.rs.ColourWrite = 15;
        w.sm.VSEnabled = vsOld; w.sm.PSEnabled = psOld;
        ++w.cam.Refreshed;
    }
    --w.rs.CaptureDepth;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    World w;
    w.rs.CaptureDepth = 0; w.rs.ZWrite = 1; w.rs.ColourWrite = 15;
    w.sm.VSEnabled = false; w.sm.PSEnabled = true; w.sm.VSApplied = 0; w.sm.Prepared = 0;
    Camera& c = w.cam;
    c.Pos.X = 0; c.Pos.Y = 0; c.Pos.Z = 0; c.Look.X = 0; c.Look.Y = 0; c.Look.Z = 1; c.NearZ = 1.0f; c.FarZ = 100.0f;
    // side planes of a 90-degree frustum looking down +Z (outward normals)
    c.PlaneN[0].X = 0.7071f; c.PlaneN[0].Y = 0; c.PlaneN[0].Z = -0.7071f;
    c.PlaneN[1].X = -0.7071f; c.PlaneN[1].Y = 0; c.PlaneN[1].Z = -0.7071f;
    c.PlaneN[2].X = 0; c.PlaneN[2].Y = 0.7071f; c.PlaneN[2].Z = -0.7071f;
    c.PlaneN[3].X = 0; c.PlaneN[3].Y = -0.7071f; c.PlaneN[3].Z = -0.7071f;
    for (int k = 0; k < 12; ++k) { c.View.E[k] = 0; c.InvNoScale.E[k] = 0; }
    c.View.E[0] = 1; c.View.E[4] = 1; c.View.E[8] = 1;                    // identity view: view Z = world Z
    c.InvNoScale.E[0] = 1; c.InvNoScale.E[4] = 2; c.InvNoScale.E[8] = 3; c.InvNoScale.E[9] = 7;
    c.ScaleX = 1.0f; c.ScaleY = 2.0f; c.Refreshed = 0;
    w.dimX = 640; w.dimY = 480; w.draws = 0; w.lastSegments = 0; w.selected = 0; w.stateBlocks = 0;

    // empty: only the capture block opens and closes
    FrameEnd(w, 0, 0);
    if (w.rs.CaptureDepth != 0 || w.sm.VSApplied != 0 || c.Refreshed != 0) { std::printf("00b8df50_TEST FAIL\n"); return 1; }

    FlareEntry t[3];
    t[0].Position.X = 0; t[0].Position.Y = 0; t[0].Position.Z = 10; t[0].Radius = 0.5f; t[0].CircleSegments = 12; t[0].MaxPixelCount = 0;
    t[1].Position.X = 0; t[1].Position.Y = 0; t[1].Position.Z = -10; t[1].Radius = 0.5f; t[1].CircleSegments = 8; t[1].MaxPixelCount = 0;
    t[2].Position.X = 50; t[2].Position.Y = 0; t[2].Position.Z = 10; t[2].Radius = 0.5f; t[2].CircleSegments = 6; t[2].MaxPixelCount = 0;
    FrameEnd(w, t, 3);
    // visible: rx = 640 * 1 * 0.05 * 0.5 = 16, ry = 480 * 2 * 0.05 * 0.5 = 24, pi * 16 * 24 = 1206.37 -> 1206
    if (t[0].MaxPixelCount != 1206) { std::printf("00b8df50_TEST FAIL\n"); return 1; }
    if (t[1].MaxPixelCount != 1 || t[2].MaxPixelCount != 1) { std::printf("00b8df50_TEST FAIL\n"); return 1; }
    if (w.draws != 1 || w.lastSegments != 12 || w.sm.Prepared != 1 || w.stateBlocks != 1) { std::printf("00b8df50_TEST FAIL\n"); return 1; }
    if (!Near(w.sm.World.E[0], 0.5f) || !Near(w.sm.World.E[4], 1.0f) || !Near(w.sm.World.E[8], 1.5f) ||
        !Near(w.sm.World.E[9], 0.0f) || !Near(w.sm.World.E[11], 10.0f)) { std::printf("00b8df50_TEST FAIL\n"); return 1; }
    if (w.sm.VSEnabled != false || w.sm.PSEnabled != true || w.rs.ZWrite != 1 || w.rs.ColourWrite != 15 ||
        w.rs.CaptureDepth != 0 || c.Refreshed != 1) { std::printf("00b8df50_TEST FAIL\n"); return 1; }
    std::printf("00b8df50_TEST PASS\n");
    return 0;
}