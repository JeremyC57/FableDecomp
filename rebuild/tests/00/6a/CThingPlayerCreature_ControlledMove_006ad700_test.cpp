// Behaviour test for CThingPlayerCreature::ControlledMove (0x006AD700): the walking forces are updated first; when the game camera
// gives its manager, the hero has hit locations and the camera mode is not scripted control (8), the camera inside the hero
// (projectile-weapon mode 25 inside the bounding sphere, else inside a combat hit location) pushes the hero by 0.1 x the flattened
// centre-minus-camera direction (normalised only when its 2D length exceeds 1e-4) and removes that direction's component from the
// movement acceleration; the acceleration then goes to the physics as a controlled impulse (or into the impulse velocity) unless the
// current action (not finished) prevents normal movement, and an action that clears the movement vector zeroes it.
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; };
float DELTA_NOTIONAL_ZERO = 1e-4f;      // retail global 0x0129BA3C, read and squared at run time (not folded: keeps the boundary case exact on x87)
struct World {
    bool has_manager; int camera_mode; V3 cam_pos;
    bool has_hit_locations; V3 centre; float radius; bool pos_inside; int pos_inside_calls;
    int player_mode;
    bool has_physics; int impulses; V3 impulse[2];
    bool has_action; bool finished; bool prevents; bool clears;
    int walking_updates;
};
struct Hero {
    World* w; V3 MovementAcceleration; V3 ImpulseVelocity;
    void ApplyImpulseVelocity(const V3& impulse)
    {
        if (w->has_physics) w->impulse[w->impulses++] = impulse;
        else { ImpulseVelocity.X += impulse.X; ImpulseVelocity.Y += impulse.Y; ImpulseVelocity.Z += impulse.Z; }
    }
    void ControlledMove()
    {
        ++w->walking_updates;
        if (w->has_manager && w->has_hit_locations) {
            const V3& cam_pos = w->cam_pos;
            V3 centre_pos = w->centre; float radius = w->radius;
            if (w->camera_mode != 8) {
                bool inside_sphere = false;
                if (w->player_mode == 25) {
                    float dx = cam_pos.X - centre_pos.X, dy = cam_pos.Y - centre_pos.Y, dz = cam_pos.Z - centre_pos.Z;
                    inside_sphere = dx * dx + dy * dy + dz * dz < radius * radius;
                }
                bool push = inside_sphere;
                if (!push) { ++w->pos_inside_calls; push = w->pos_inside; }
                if (push) {
                    V3 line_out2d = { centre_pos.X - cam_pos.X, centre_pos.Y - cam_pos.Y, 0.0f };
                    if (line_out2d.X * line_out2d.X + line_out2d.Y * line_out2d.Y > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
                        float m = std::sqrt(line_out2d.X * line_out2d.X + line_out2d.Y * line_out2d.Y);
                        line_out2d.X /= m; line_out2d.Y /= m;
                    }
                    const float IMPULSE_MAG = 0.1f;
                    V3 push_impulse = { line_out2d.X * IMPULSE_MAG, line_out2d.Y * IMPULSE_MAG, line_out2d.Z * IMPULSE_MAG };
                    w->impulse[w->impulses++] = push_impulse;
                    float along = line_out2d.X * MovementAcceleration.X + line_out2d.Y * MovementAcceleration.Y
                        + line_out2d.Z * MovementAcceleration.Z;
                    MovementAcceleration.X -= line_out2d.X * along;
                    MovementAcceleration.Y -= line_out2d.Y * along;
                    MovementAcceleration.Z -= line_out2d.Z * along;
                }
            }
        }
        bool have = w->has_action;
        if (!have || w->finished || !w->prevents)
            ApplyImpulseVelocity(MovementAcceleration);
        if (have && w->clears)
            MovementAcceleration.X = MovementAcceleration.Y = MovementAcceleration.Z = 0.0f;
    }
};
static int fail(int n) { std::printf("006ad700_TEST FAIL %d\n", n); return 1; }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
static World base()
{
    World w = { true, 3, { 0, 0, 0 }, true, { 4, 0, 1 }, 1.0f, false, 0, 1, true, 0, { { 0, 0, 0 }, { 0, 0, 0 } },
                false, false, false, false, 0 };
    return w;
}
static Hero hero(World* w) { Hero h = { w, { 2, 3, 5 }, { 0, 0, 0 } }; return h; }
int main()
{
    // camera inside a combat hit location: push away along +X (centre - camera, flattened, normalised)
    World w = base(); w.pos_inside = true; Hero h = hero(&w); h.ControlledMove();
    if (w.walking_updates != 1 || w.impulses != 2 || w.pos_inside_calls != 1) return fail(1);
    if (!near(w.impulse[0].X, 0.1f) || !near(w.impulse[0].Y, 0) || !near(w.impulse[0].Z, 0)) return fail(2);
    if (!near(h.MovementAcceleration.X, 0) || !near(h.MovementAcceleration.Y, 3) || !near(h.MovementAcceleration.Z, 5)) return fail(3);
    if (!near(w.impulse[1].Y, 3) || !near(w.impulse[1].X, 0)) return fail(4);       // then the acceleration itself
    // not inside anything: no push, the acceleration goes to the physics unchanged
    w = base(); h = hero(&w); h.ControlledMove();
    if (w.impulses != 1 || !near(w.impulse[0].X, 2) || !near(h.MovementAcceleration.X, 2)) return fail(5);
    // no manager / no hit locations / scripted camera: no push and no PosInside query
    w = base(); w.pos_inside = true; w.has_manager = false; h = hero(&w); h.ControlledMove();
    if (w.impulses != 1 || w.pos_inside_calls != 0) return fail(6);
    w = base(); w.pos_inside = true; w.has_hit_locations = false; h = hero(&w); h.ControlledMove();
    if (w.impulses != 1 || w.pos_inside_calls != 0) return fail(7);
    w = base(); w.pos_inside = true; w.camera_mode = 8; h = hero(&w); h.ControlledMove();
    if (w.impulses != 1 || w.pos_inside_calls != 0) return fail(8);
    // projectile-weapon mode inside the sphere: push without the PosInside query; outside the sphere: the query decides
    w = base(); w.player_mode = 25; w.radius = 5.0f; h = hero(&w); h.ControlledMove();
    if (w.impulses != 2 || w.pos_inside_calls != 0) return fail(9);
    w = base(); w.player_mode = 25; w.radius = std::sqrt(17.0f); h = hero(&w); h.ControlledMove();   // on the sphere: not inside
    if (w.impulses != 1 || w.pos_inside_calls != 1) return fail(10);
    w = base(); w.player_mode = 24; w.radius = 5.0f; h = hero(&w); h.ControlledMove();             // other mode: sphere ignored
    if (w.impulses != 1 || w.pos_inside_calls != 1) return fail(11);
    // a tiny flattened offset is not normalised
    w = base(); w.pos_inside = true; w.centre.X = 0.00005f; w.centre.Z = 3.0f; h = hero(&w); h.ControlledMove();
    if (!near(w.impulse[0].X, 0.000005f) || !near(h.MovementAcceleration.X, 2.0f - 0.00005f * 0.00005f * 2.0f)) return fail(12);
    // actions: preventing (not finished) blocks the impulse; finished or not preventing lets it through; clearing zeroes it
    w = base(); w.has_action = true; w.prevents = true; h = hero(&w); h.ControlledMove();
    if (w.impulses != 0 || !near(h.MovementAcceleration.Y, 3)) return fail(13);
    w = base(); w.has_action = true; w.prevents = true; w.finished = true; h = hero(&w); h.ControlledMove();
    if (w.impulses != 1) return fail(14);
    w = base(); w.has_action = true; w.clears = true; h = hero(&w); h.ControlledMove();
    if (w.impulses != 1 || h.MovementAcceleration.X != 0 || h.MovementAcceleration.Y != 0 || h.MovementAcceleration.Z != 0) return fail(15);
    w = base(); w.clears = true; h = hero(&w); h.ControlledMove();                                 // no action: nothing cleared
    if (!near(h.MovementAcceleration.Z, 5)) return fail(16);
    // no physics: the acceleration is added to the impulse velocity
    w = base(); w.has_physics = false; h = hero(&w); h.ImpulseVelocity.X = 1; h.ControlledMove();
    if (w.impulses != 0 || !near(h.ImpulseVelocity.X, 3) || !near(h.ImpulseVelocity.Z, 5)) return fail(17);
    // audit additions: exact boundaries and the null-action guard
    // projectile-weapon mode exactly ON the sphere (3-4-5 offset, squared distance 25 == radius squared 25): not inside, so the
    // PosInside query decides (strict <)
    w = base(); w.player_mode = 25; w.centre.X = 3; w.centre.Y = 4; w.centre.Z = 0; w.radius = 5.0f; h = hero(&w); h.ControlledMove();
    if (w.impulses != 1 || w.pos_inside_calls != 1) return fail(18);
    // flattened offset exactly DELTA_NOTIONAL_ZERO long (squared length == the squared epsilon): not normalised (strict >)
    w = base(); w.pos_inside = true; w.centre.X = 1e-4f; w.centre.Z = 0; h = hero(&w); h.ControlledMove();
    if (!near(w.impulse[0].X, 1e-5f) || w.impulse[0].X > 1e-4f) return fail(19);
    // no current action: the acceleration is applied whatever a stale "prevents" answer would say (the null test comes first)
    w = base(); w.prevents = true; h = hero(&w); h.ControlledMove();
    if (w.impulses != 1 || !near(w.impulse[0].Y, 3)) return fail(20);
    std::printf("006ad700_TEST PASS\n");
    return 0;
}