// Behaviour test for 0x00745120 CTCPerceiveThing::UpdateLineOfSight: a self-contained model of the row's logic (same decisions,
// same order of GUI notifications), exercised over the out-of-range / unviewable / blocked / mesh-hit / sight-range / extended-range
// branches. Plain C++98; no row object is linked.
#include <cstdio>

struct Vec { float X, Y, Z; Vec() : X(0), Y(0), Z(0) {} Vec(float x, float y, float z) : X(x), Y(y), Z(z) {} };

struct World {
    // scenario inputs
    bool has_stealth, viewable, in_melee_tc, in_melee, in_cone, map_los, mesh_blocks;
    float visibility, ext_radius, sight_radius;
    long frame;
    // observed outputs
    int adds, removes, aware_info_calls, searches;
    long box[4];
};

struct Perceiver {
    World* w;
    bool has_target;
    float AwarenessTimer, DistSq;
    long FramesSinceDirectlyPerceivedThing, FrameHitByPerceivedThing;
    bool LineOfSight, ExtendedLineOfSight, IgnoreFOVLineOfSight, DirectAwareness;
    Vec my_pos, target_pos;

    void Add() { ++w->adds; }
    void Remove() { ++w->removes; }
    void DrawTail(bool on) { if (on) { if (has_target) Add(); else Remove(); } }
    void SetLOS(bool v) { if (LineOfSight != v) { if (LineOfSight) Remove(); LineOfSight = v; DrawTail(v); } if (LineOfSight) FramesSinceDirectlyPerceivedThing = 0; }
    void SetExt(bool v) { if (ExtendedLineOfSight != v) { if (ExtendedLineOfSight) Remove(); ExtendedLineOfSight = v; DrawTail(v); } }
    void SetIgn(bool v) { if (IgnoreFOVLineOfSight != v) { if (IgnoreFOVLineOfSight) Remove(); IgnoreFOVLineOfSight = v; DrawTail(v); } }
    void ClearAll() { SetLOS(false); SetExt(false); SetIgn(false); }
    static float Min(float a, float b) { return a < b ? a : b; }
    static float Max(float a, float b) { return a > b ? a : b; }

    void UpdateLineOfSight()
    {
        if (AwarenessTimer == 0.0f) DirectAwareness = false;
        float visibility = 1.0f;
        if (w->has_stealth) visibility = w->visibility;
        float ext = w->ext_radius * visibility;
        float sight = w->sight_radius * visibility;
        float sight_sq = sight * sight;
        if (ext * ext < DistSq) { ClearAll(); return; }
        if (w->has_stealth && !w->viewable) { ClearAll(); return; }
        Vec eye(my_pos.X, my_pos.Y, my_pos.Z + 1.5f);
        bool in_fov = true;
        if (!(w->in_melee_tc && w->in_melee)) {
            if (!w->in_cone) in_fov = false;
        }
        Vec target(target_pos.X, target_pos.Y, target_pos.Z + 1.5f);
        if (!w->map_los) { ClearAll(); return; }
        w->box[0] = (long)(Min(eye.X, target.X) - 1.0f);
        w->box[2] = (long)(Max(eye.X, target.X) + 1.0f);
        w->box[1] = (long)(Min(eye.Y, target.Y) - 1.0f);
        w->box[3] = (long)(Max(eye.Y, target.Y) + 1.0f);
        ++w->searches;
        if (w->mesh_blocks) { ClearAll(); return; }
        if (DistSq < sight_sq) {
            SetLOS(in_fov);
            SetExt(false);
            SetIgn(true);
            if (!in_fov && FrameHitByPerceivedThing > 0)
                in_fov = w->frame - FrameHitByPerceivedThing < 30;
            DirectAwareness = in_fov;
        } else {
            SetLOS(false);
            SetExt(in_fov);
            SetIgn(false);
            ++w->aware_info_calls;
        }
    }
};

static World MakeWorld()
{
    World w;
    w.has_stealth = false; w.viewable = true; w.in_melee_tc = false; w.in_melee = false; w.in_cone = true;
    w.map_los = true; w.mesh_blocks = false; w.visibility = 1.0f; w.ext_radius = 20.0f; w.sight_radius = 10.0f; w.frame = 100;
    w.adds = w.removes = w.aware_info_calls = w.searches = 0;
    for (int i = 0; i < 4; ++i) w.box[i] = 0;
    return w;
}

static Perceiver MakePerceiver(World* w, float dist_sq)
{
    Perceiver p;
    p.w = w; p.has_target = true; p.AwarenessTimer = 1.0f; p.DistSq = dist_sq;
    p.FramesSinceDirectlyPerceivedThing = 7; p.FrameHitByPerceivedThing = 0;
    p.LineOfSight = true; p.ExtendedLineOfSight = true; p.IgnoreFOVLineOfSight = true; p.DirectAwareness = true;
    p.my_pos = Vec(2.0f, 3.0f, 0.0f); p.target_pos = Vec(6.5f, -4.0f, 1.0f);
    return p;
}

static int fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); ++fails; } }

int main()
{
    // 1. beyond extended range: all three cleared, three GUI removes, no search
    { World w = MakeWorld(); Perceiver p = MakePerceiver(&w, 500.0f); p.UpdateLineOfSight();
      Check(!p.LineOfSight && !p.ExtendedLineOfSight && !p.IgnoreFOVLineOfSight && w.removes == 3 && w.searches == 0, "range"); }
    // 2. stealthy, unviewable target in range: cleared
    { World w = MakeWorld(); w.has_stealth = true; w.viewable = false; Perceiver p = MakePerceiver(&w, 4.0f); p.UpdateLineOfSight();
      Check(!p.LineOfSight && !p.ExtendedLineOfSight && !p.IgnoreFOVLineOfSight && w.searches == 0, "unviewable"); }
    // 3. stealth visibility shrinks the extended range below the distance
    { World w = MakeWorld(); w.has_stealth = true; w.visibility = 0.5f; Perceiver p = MakePerceiver(&w, 150.0f); p.UpdateLineOfSight();
      Check(!p.ExtendedLineOfSight && w.searches == 0, "visibility"); }
    // 4. world map blocks the line: cleared before the search
    { World w = MakeWorld(); w.map_los = false; Perceiver p = MakePerceiver(&w, 4.0f); p.UpdateLineOfSight();
      Check(!p.LineOfSight && w.searches == 0, "map"); }
    // 5. a physics mesh blocks: cleared after the search; the box spans both eye points with a one-unit border
    { World w = MakeWorld(); w.mesh_blocks = true; Perceiver p = MakePerceiver(&w, 4.0f); p.UpdateLineOfSight();
      Check(!p.LineOfSight && w.searches == 1, "mesh");
      Check(w.box[0] == 1 && w.box[1] == -5 && w.box[2] == 7 && w.box[3] == 4, "box"); }
    // 6. clear line, sight range, in the cone: line of sight on, extended off, ignore-FOV on, directly aware
    { World w = MakeWorld(); Perceiver p = MakePerceiver(&w, 4.0f); p.LineOfSight = false; p.IgnoreFOVLineOfSight = false; p.UpdateLineOfSight();
      Check(p.LineOfSight && !p.ExtendedLineOfSight && p.IgnoreFOVLineOfSight && p.DirectAwareness && p.FramesSinceDirectlyPerceivedThing == 0, "sight");
      Check(w.adds == 2 && w.removes == 1, "sight gui"); }
    // 7. sight range, outside the cone, hit 10 frames ago: no line of sight but directly aware
    { World w = MakeWorld(); w.in_cone = false; Perceiver p = MakePerceiver(&w, 4.0f); p.FrameHitByPerceivedThing = 90; p.UpdateLineOfSight();
      Check(!p.LineOfSight && p.IgnoreFOVLineOfSight && p.DirectAwareness, "recent hit"); }
    // 8. outside the cone, hit long ago: not directly aware
    { World w = MakeWorld(); w.in_cone = false; Perceiver p = MakePerceiver(&w, 4.0f); p.FrameHitByPerceivedThing = 50; p.UpdateLineOfSight();
      Check(!p.DirectAwareness, "old hit"); }
    // 9. in melee the cone is not tested
    { World w = MakeWorld(); w.in_cone = false; w.in_melee_tc = true; w.in_melee = true; Perceiver p = MakePerceiver(&w, 4.0f); p.UpdateLineOfSight();
      Check(p.LineOfSight && p.DirectAwareness, "melee"); }
    // 10. extended range only: extended follows the cone, the others are cleared, last aware info refreshed
    { World w = MakeWorld(); Perceiver p = MakePerceiver(&w, 200.0f); p.ExtendedLineOfSight = false; p.UpdateLineOfSight();
      Check(!p.LineOfSight && p.ExtendedLineOfSight && !p.IgnoreFOVLineOfSight && w.aware_info_calls == 1, "extended"); }
    // 11. no awareness timer: direct awareness dropped even when the line fails
    { World w = MakeWorld(); w.map_los = false; Perceiver p = MakePerceiver(&w, 4.0f); p.AwarenessTimer = 0.0f; p.UpdateLineOfSight();
      Check(!p.DirectAwareness, "timer"); }
    if (fails) { std::printf("00745120_TEST FAIL\n"); return 1; }
    std::printf("00745120_TEST PASS\n");
    return 0;
}