// Behaviour test for CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise (0x008C12D0): a copy of the row's logic over a
// small model -- the rush decision (counter + 1 == NumGetHitsBeforeRush), the plain strike-response fallback, the direction from
// the creature to the point of hit or to the living hitter (a dead hitter is ignored: cases h / i), the fast normalise, the 100-degree facing test choosing the back/front
// rush animation, the facing angle of the away vector wrapped into [0,1), and the def release.
#include <cstdio>
#include <cstring>
#include <math.h>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static V Sub(const V& a, const V& b) { return V(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static float Dot(const V& a, const V& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static unsigned long g_table[128];
static void BuildTable() { for (int i = 0; i < 128; ++i) { double m = 1.0 + (i + 0.5) / 128.0; double r = 1.0 / sqrt(m); float f = (float)(r * 2.0); unsigned long b; memcpy(&b, &f, 4); g_table[i] = (b >> 15) & 0xff; g_table[i] = ((b & 0x7fffff) >> 15); } }
static void Normalise(V& v)
{
    float x = v.X, y = v.Y, z = v.Z;
    float l2 = x * x + y * y + z * z;
    unsigned long bits; memcpy(&bits, &l2, 4);
    unsigned long est = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (g_table[(bits >> 17) & 0x7f] << 15);
    float r; memcpy(&r, &est, 4);
    r = (3.0f - r * r * l2) * r * 0.5f;
    v.X *= r; v.Y *= r; v.Z *= r;
}
static float AngleXY(const V& v)
{
    float a = (float)atan2(v.X, v.Y) * 0.15915494f;
    if (a < 0.0f || a >= 1.0f) { a = (float)fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; }
    return a;
}
struct Def { long RefCount; long NumGetHitsBeforeRush; int deleted; };
struct World {
    long counter; Def def; V creaturePos, facing, pointOfHit, hitterPos; bool hitterPresent, hitterDead;
    bool causeRush; int baseStrikeInit, combatInit; char anim[64]; float angle;
};
static void Initialise(World& w)
{
    Def* def = &w.def; ++def->RefCount;                        // GetSubDef hands back a counted def
    long hits = w.counter + 1;
    if (hits > def->NumGetHitsBeforeRush) w.causeRush = false;
    else if (hits == def->NumGetHitsBeforeRush) w.causeRush = true;
    else w.causeRush = false;
    if (!w.causeRush) {
        ++w.baseStrikeInit;
    } else {
        V facing = w.facing;
        V direction = Sub(w.pointOfHit, w.creaturePos);
        if (w.hitterPresent && !w.hitterDead) direction = Sub(w.hitterPos, w.creaturePos);
        V away(0.0f, 0.0f, 0.0f);
        Normalise(direction);
        if (Dot(facing, direction) < (float)cos(1.745329383786327)) {
            away = V(-direction.X, -direction.Y, -direction.Z);
            strcpy(w.anim, "RESPONSE_GENERIC_HIT_BACK_RUSH");
        } else {
            strcpy(w.anim, "RESPONSE_GENERIC_HIT_FRONT_RUSH");
        }
        w.angle = AngleXY(away);
        ++w.combatInit;
    }
    if (--def->RefCount == 0) def->deleted = 1;
}
static World Make(long counter, long needed)
{
    World w; memset(&w, 0, sizeof w);
    w.counter = counter; w.def.RefCount = 1; w.def.NumGetHitsBeforeRush = needed;
    w.creaturePos = V(0, 0, 0); w.facing = V(0, 1, 0); w.pointOfHit = V(0, -5, 0); w.hitterPos = V(0, 5, 0);
    return w;
}
int main()
{
    BuildTable();
    bool ok = true;
    World a = Make(2, 4); Initialise(a);                      // 3 != 4: no rush
    ok = ok && !a.causeRush && a.baseStrikeInit == 1 && a.combatInit == 0 && a.def.RefCount == 1;
    World b = Make(5, 4); Initialise(b);                      // 6 > 4: no rush
    ok = ok && !b.causeRush && b.baseStrikeInit == 1;
    World c = Make(3, 4); Initialise(c);                      // hit from behind, no hitter: back rush, face away (+Y => angle 0)
    ok = ok && c.causeRush && c.combatInit == 1 && strcmp(c.anim, "RESPONSE_GENERIC_HIT_BACK_RUSH") == 0 && c.angle < 0.01f;
    World d = Make(3, 4); d.hitterPresent = true; Initialise(d);   // living hitter in front: front rush, zero vector angle 0
    ok = ok && strcmp(d.anim, "RESPONSE_GENERIC_HIT_FRONT_RUSH") == 0 && d.angle == 0.0f;
    World e = Make(3, 4); e.hitterPresent = true; e.hitterDead = true; e.pointOfHit = V(5, 0, 0); Initialise(e);   // dead hitter ignored; side hit (90 deg) is front
    ok = ok && strcmp(e.anim, "RESPONSE_GENERIC_HIT_FRONT_RUSH") == 0;
    World f = Make(3, 4); f.pointOfHit = V(-5, 0, 0); f.facing = V(1, 0, 0); Initialise(f);   // hit from -X while facing +X: back, away = +X => 0.25
    ok = ok && strcmp(f.anim, "RESPONSE_GENERIC_HIT_BACK_RUSH") == 0 && f.angle > 0.24f && f.angle < 0.26f;
    World h = Make(3, 4); h.hitterPresent = true; h.hitterDead = true; h.hitterPos = V(0, -5, 0); h.pointOfHit = V(0, 5, 0);
    Initialise(h);   // dead hitter BEHIND, point of hit in front: the IS_THING_ALIVE test must skip the hitter -> front rush
    ok = ok && strcmp(h.anim, "RESPONSE_GENERIC_HIT_FRONT_RUSH") == 0 && h.angle == 0.0f;
    World i = Make(3, 4); i.hitterPresent = true; i.hitterPos = V(0, -5, 0); i.pointOfHit = V(0, 5, 0);
    Initialise(i);   // same geometry, hitter alive: the hitter wins over the point of hit -> back rush, away = +Y => angle 0
    ok = ok && strcmp(i.anim, "RESPONSE_GENERIC_HIT_BACK_RUSH") == 0 && i.angle < 0.01f;
    World g = Make(3, 4); g.def.RefCount = 0; Initialise(g);   // last reference released -> Delete
    ok = ok && g.def.deleted == 1;
    if (!ok) { std::printf("008c12d0_TEST FAIL\n"); return 1; }
    std::printf("008c12d0_TEST PASS\n");
    return 0;
}