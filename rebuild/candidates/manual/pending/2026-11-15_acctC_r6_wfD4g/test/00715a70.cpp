// Behaviour test for 0x00715A70 NTCScriptedControl::CActionTalkBase::SetUpCamera: a model of the row's decisions over plain vectors.
// Checked: the far-enough flag (> 2.0), the look direction (things differ: at - from normalised; same thing talking to itself: minus
// the facing vector, NOT normalised; same thing otherwise: other + me normalised; no things: from - at normalised), the zoom clamp
// ((|from - at| - 0.4) / 2.6 in [0, 1]) and height = -zoom^2, the focus cases (1..4 scale offset Z and move at towards from by
// 0.1 / 0.25 / 0.33 / 0.5; 5 / 7 raise / lower from by 0.75; 5..7 zero offset), the at.Z pull (0.75 of the way) when the offset is
// non-zero and from is above at, at += offset, the demon-door override (position 11), the 13 camera positions (side = dir x up),
// the creature head lift inside radius + 0.2, and the zoom-type choice of module and frame count (15 s slow, 0.8 s fast).
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
static V mk(float x, float y, float z) { V v = { x, y, z }; return v; }
static V add(V a, V b) { return mk(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static V sub(V a, V b) { return mk(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static V mul(V a, float k) { return mk(a.X * k, a.Y * k, a.Z * k); }
static float sq(V a) { return a.X * a.X + a.Y * a.Y + a.Z * a.Z; }
static V norm(V a) { return mul(a, 1.0f / std::sqrt(sq(a))); }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
static bool nearv(V a, V b) { return near(a.X, b.X) && near(a.Y, b.Y) && near(a.Z, b.Z); }

struct Creature { V pos; float radius; float headZ; };
struct Out { bool far; V dir; V at; V camera; bool moveBetween; long frames; };

// thingsKind: 0 = no things, 1 = different things, 2 = same thing + talking to itself, 3 = same thing otherwise
static Out SetUpCamera(V me, V other, int thingsKind, V facing, V from, V at, V fromThingPos, V atThingPos, long focus, long position,
                       bool demonDoor, const Creature* cs, int n, long zoomType, long fps)
{
    Out o;
    o.far = std::sqrt(sq(sub(me, other))) > 2.0f;
    V dir;
    if (thingsKind == 1) dir = norm(sub(atThingPos, fromThingPos));
    else if (thingsKind == 2) dir = mul(facing, -1.0f);
    else if (thingsKind == 3) dir = norm(add(other, me));
    else dir = norm(sub(from, at));
    float zoom = (std::sqrt(sq(sub(from, at))) - 0.4f) * (1.0f / 2.6f);
    if (zoom < 0.0f) zoom = 0.0f; else if (zoom > 1.0f) zoom = 1.0f;
    float height = -(zoom * zoom);
    V offset = mk(0, 0, height), start = at, delta = sub(at, from);
    static const float k[5] = { 0, 0.1f, 0.25f, 0.33f, 0.5f };
    if (focus >= 1 && focus <= 4) { offset = mk(0, 0, height * k[focus]); at = add(from, mul(delta, k[focus])); }
    else if (focus == 5) { from.Z += 0.75f; offset = mk(0, 0, 0); }
    else if (focus == 6) offset = mk(0, 0, 0);
    else if (focus == 7) { from.Z -= 0.75f; offset = mk(0, 0, 0); }
    if (sq(offset) > 1e-4f * 1e-4f && from.Z > at.Z) at.Z = (at.Z - from.Z) * 0.75f + from.Z;
    at = add(at, offset);
    if (demonDoor) position = 11;
    V side = mk(dir.Y, -dir.X, 0), torso = mk(0, 0, 1.0f), c = from;
    switch (position) {
    case 0: c = add(from, dir); break;
    case 1: c = sub(from, mul(side, 0.4f)); break;
    case 2: c = add(from, mul(side, 0.4f)); break;
    case 3: c = sub(sub(from, mul(side, 0.4f)), mul(dir, 0.5f)); break;
    case 4: c = sub(sub(from, mul(side, 0.4f)), mul(dir, 2.0f)); break;      // behind left shoulder, far
    case 5: c = sub(add(from, mul(side, 0.4f)), mul(dir, 0.5f)); break;      // behind right shoulder, near
    case 6: c = sub(add(from, mul(side, 0.4f)), mul(dir, 2.0f)); break;
    case 7: c = add(sub(from, side), torso); break;
    case 8: c = add(sub(from, mul(side, 2.6f)), torso); break;               // left of torso, far
    case 9: c = add(add(from, side), torso); break;                          // right of torso, near
    case 10: c = add(add(from, mul(side, 2.6f)), torso); break;
    default: break;
    }
    for (int i = 0; i < n; ++i) {
        float dx = cs[i].pos.X - c.X, dy = cs[i].pos.Y - c.Y, r = cs[i].radius + 0.2f;
        if (dx * dx + dy * dy < r * r && cs[i].headZ + 0.5f > c.Z) c.Z = cs[i].headZ + 0.5f;
    }
    o.dir = dir; o.at = at; o.camera = c;
    o.moveBetween = zoomType != 0;
    o.frames = zoomType == 0 ? 0 : (long)std::floor((float)fps * (zoomType == 1 ? 15.0f : 0.8f) + 0.5f);
    (void)start;
    return o;
}

int main()
{
    int fails = 0;
    V z = mk(0, 0, 0);
    Out a = SetUpCamera(mk(0, 0, 0), mk(3, 0, 0), 1, z, mk(0, 0, 2), mk(4, 0, 2), mk(0, 0, 0), mk(0, 4, 0), 0, 0, false, 0, 0, 0, 30);
    if (!a.far || !nearv(a.dir, mk(0, 1, 0)) || !nearv(a.camera, mk(0, 1, 2)) || a.moveBetween) ++fails;
    Out b = SetUpCamera(mk(0, 0, 0), mk(1, 0, 0), 2, mk(0, 2, 0), mk(0, 0, 2), mk(4, 0, 2), z, z, 0, 0, false, 0, 0, 1, 30);
    if (b.far || !nearv(b.dir, mk(0, -2, 0)) || !b.moveBetween || b.frames != 450) ++fails;      // not normalised
    Out c = SetUpCamera(mk(1, 0, 0), mk(1, 2, 0), 3, z, z, z, z, z, 0, 0, false, 0, 0, 2, 30);
    if (!nearv(c.dir, mk(2 / std::sqrt(8.0f), 2 / std::sqrt(8.0f), 0)) || c.frames != 24) ++fails;
    Out d = SetUpCamera(z, z, 0, z, mk(3, 0, 1), mk(0, 0, 0), z, z, 4, 2, false, 0, 0, 0, 30);
    float zm = (3.1623f - 0.4f) / 2.6f; if (zm > 1.0f) zm = 1.0f;
    // focus 4: at = from + delta * 0.5 = (1.5, 0, 0.5); offset Z = -zm^2 * 0.5; from above at -> at.Z pulled, then += offset
    float atZ = (0.5f - 1.0f) * 0.75f + 1.0f - zm * zm * 0.5f;
    if (!near(d.at.X, 1.5f) || !near(d.at.Z, atZ)) ++fails;
    V dd = norm(mk(3, 0, 1));                                                                   // dir = from - at, normalised
    if (!nearv(d.camera, add(mk(3, 0, 1), mul(mk(dd.Y, -dd.X, 0), 0.4f)))) ++fails;           // position 2: from + side * 0.4
    Out e = SetUpCamera(z, z, 0, z, mk(3, 0, 1), mk(0, 0, 0), z, z, 5, 9, true, 0, 0, 0, 30);   // demon door: camera = from
    if (!nearv(e.camera, mk(3, 0, 1.75f)) || !nearv(e.at, mk(0, 0, 0))) ++fails;
    Creature cr[2] = { { mk(3.1f, 0, 0), 0.5f, 4.0f }, { mk(9, 9, 0), 0.5f, 9.0f } };
    Out f = SetUpCamera(z, z, 0, z, mk(3, 0, 1), mk(0, 0, 0), z, z, 6, 12, false, cr, 2, 0, 30);
    if (!near(f.camera.Z, 4.5f)) ++fails;                                                      // lifted by the near creature only
    V fr = mk(3, 0, 1), sd = mk(dd.Y, -dd.X, 0);
    Out g4 = SetUpCamera(z, z, 0, z, fr, z, z, z, 0, 4, false, 0, 0, 0, 30), g5 = SetUpCamera(z, z, 0, z, fr, z, z, z, 0, 5, false, 0, 0, 0, 30);
    if (!nearv(g4.camera, sub(sub(fr, mul(sd, 0.4f)), mul(dd, 2.0f))) || !nearv(g5.camera, sub(add(fr, mul(sd, 0.4f)), mul(dd, 0.5f)))) ++fails;
    Out g8 = SetUpCamera(z, z, 0, z, fr, z, z, z, 0, 8, false, 0, 0, 0, 30), g9 = SetUpCamera(z, z, 0, z, fr, z, z, z, 0, 9, false, 0, 0, 0, 30);
    if (!nearv(g8.camera, add(sub(fr, mul(sd, 2.6f)), mk(0, 0, 1))) || !nearv(g9.camera, add(add(fr, sd), mk(0, 0, 1)))) ++fails;
    if (fails) { std::printf("00715a70_TEST FAIL %d\n", fails); return 1; }
    std::printf("00715a70_TEST PASS\n");
    return 0;
}
