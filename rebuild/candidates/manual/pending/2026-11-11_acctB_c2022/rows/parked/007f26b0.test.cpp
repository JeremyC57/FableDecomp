// Behaviour test for 0x007F26B0 CTCLightningOrb::Initialise (parked row): a host model of the orb setup with the engine callees mocked:
// the predicted target position (ground height), the spline control points (start, raised take-off point, bowed middle, the two Fireheart
// approach points, target), the traversal frame count, the lightning node and the particle effect attachment.
#include <cstdio>
#include <cmath>

struct V3 { float X, Y, Z; };
static V3 Make(float x, float y, float z) { V3 v; v.X = x; v.Y = y; v.Z = z; return v; }
static V3 Add(V3 a, V3 b) { return Make(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static V3 Sub(V3 a, V3 b) { return Make(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static V3 Mul(V3 a, float s) { return Make(a.X * s, a.Y * s, a.Z * s); }
static float Mag(V3 a) { return (float)sqrt(a.X * a.X + a.Z * a.Z + a.Y * a.Y); }
static V3 Cross(V3 a, V3 b) { return Make(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }

struct Orb {
    V3 points[8]; int count; long frames; bool node_added; int node_sources; long node_def; bool gravity; bool moving; bool resend;
    int effects_attached; float rotate_xy, rotate_yz;
    void Rotate(V3& v, float xy, float yz) { rotate_xy = xy; rotate_yz = yz; (void)v; }   // mocked C3DVector::Rotate
    void Initialise(V3 start, V3 target_pos_now, V3 target_last, float ground_z, bool fireheart_alive, float speed, long fps,
                    long lightning_def, bool has_effect, float random)
    {
        count = 0; frames = -1; node_added = false; node_sources = 0; node_def = 0; gravity = true; moving = false; resend = false;
        effects_attached = 0;
        V3 velocity = Mul(Mul(Sub(target_pos_now, target_last), 0.0f), (float)fps);
        V3 target_pos = Add(target_pos_now, velocity);
        target_pos.Z = ground_z;
        V3 difference = Sub(target_pos, start);
        float distance = Mag(difference);
        V3 direction = Mul(difference, 1.0f / distance);
        V3 middle = Add(Mul(Mul(direction, distance), 0.5f), start);
        V3 up = Cross(direction, Make(direction.Y, -direction.X, 0.0f));
        Rotate(up, distance, 0.06f - random * 0.12f);
        middle = Sub(middle, Mul(Mul(up, distance), 0.075f));
        float approach_distance = fireheart_alive ? 5.0f : 2.0f;
        float approach_height = fireheart_alive ? 1.25f : 0.5f;
        V3 flat = Make(direction.X, direction.Y, 0.0f);
        float f = (float)sqrt(flat.X * flat.X + flat.Y * flat.Y);
        flat = Mul(flat, 1.0f / f);
        V3 approach = Add(Add(start, Mul(flat, approach_distance)), Make(0.0f, 0.0f, approach_height));
        points[count++] = start; points[count++] = approach; points[count++] = middle;
        if (fireheart_alive) {
            points[count++] = Sub(target_pos, Mul(flat, 7.0f));
            points[count++] = Add(Sub(target_pos, Mul(Mul(flat, 7.0f), 0.5f)), Make(0.0f, 0.0f, 2.0f));
        }
        points[count++] = target_pos;
        if (speed > 1e-4f) frames = (long)((distance / speed) * fps);
        if (lightning_def > 0) { node_added = true; node_sources = 2; node_def = lightning_def; }
        gravity = false; moving = true; resend = true;
        if (has_effect) ++effects_attached;
    }
};

static bool Near(float a, float b) { return fabs(a - b) < 1e-3f; }

int main()
{
    int bad = 0;
    Orb o;
    o.Initialise(Make(0, 0, 1), Make(10, 0, 3), Make(9, 0, 3), 0.0f, false, 2.0f, 30, 7, true, 0.5f);
    if (o.count != 4 || !Near(o.points[3].X, 10.0f) || !Near(o.points[3].Z, 0.0f)) bad |= 1;          // target on the ground, no Fireheart points
    if (!Near(o.points[1].X, 2.0f) || !Near(o.points[1].Z, 1.5f)) bad |= 2;                          // take-off point 2 along, 0.5 up
    float d = (float)sqrt(100.0f + 1.0f);
    if (o.frames != (long)((d / 2.0f) * 30)) bad |= 4;                                               // frames = distance / speed * fps
    if (!o.node_added || o.node_sources != 2 || o.node_def != 7 || o.gravity || !o.moving || !o.resend || o.effects_attached != 1) bad |= 8;
    if (!Near(o.rotate_xy, d) || !Near(o.rotate_yz, 0.0f)) bad |= 16;                                 // Rotate(distance, 0.06 - 0.12 r)
    o.Initialise(Make(0, 0, 0), Make(0, 20, 0), Make(0, 20, 0), 1.0f, true, 0.0f, 30, 0, false, 0.0f);
    if (o.count != 6 || !Near(o.points[1].Y, 5.0f) || !Near(o.points[1].Z, 1.25f)) bad |= 32;        // Fireheart: 5 along, 1.25 up
    if (!Near(o.points[3].Y, 13.0f) || !Near(o.points[4].Y, 16.5f) || !Near(o.points[4].Z, 3.0f)) bad |= 64;
    if (o.frames != -1 || o.node_added || o.effects_attached != 0) bad |= 128;                       // no speed, no lightning, no effect
    if (!Near(o.rotate_yz, 0.06f)) bad |= 256;
    if (bad) { std::printf("007f26b0_TEST FAIL %d\n", bad); return 1; }
    std::printf("007f26b0_TEST PASS\n");
    return 0;
}
