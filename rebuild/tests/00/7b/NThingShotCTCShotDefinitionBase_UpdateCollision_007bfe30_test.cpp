// Behaviour test for 0x007BFE30 NThingShot::CTCShotDefinitionBase::UpdateCollision: killed_me is cleared; with a weapon-trail component
// and at least two projectile trail points the shot searches a box around the last segment (centre = mid point of last.first and
// prev.second for extended trails, else of prev.first and last.first; half-size (|line| + 0.5) * 1.5 truncated) for solid things that are
// not the live shooter (the parent thing when the shooter is dead), skips flag-0x20 things while the thing manager pauses entities,
// rolling things when the shot cannot hit rolling targets, things without hit locations and things already in HitThingsList; each
// remaining thing is tested against two triangles (extended: last.first/last.second/prev.first, then last.second/prev.second/prev.first
// when the first misses) or the finite line prev.first -> last.first, and the hit nearest prev.first (strictly nearer, start 1e10) is
// reported once through OnCollision(type 1) with its hit location stored in PBestCollisionHitLocation, only when that thing is alive.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
struct V { float X, Y, Z; V() : X(0), Y(0), Z(0) {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static V operator+(const V& a, const V& b) { return V(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static V operator-(const V& a, const V& b) { return V(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static V operator*(const V& a, float s) { return V(a.X * s, a.Y * s, a.Z * s); }
struct Box { long TLX, TLY, BRX, BRY; };
struct HitLoc { int id; };
struct Thing {
    bool dead, solid, rolling, hasHitLocs; unsigned long flags;
    int triHit;                // 0 none, 1 first triangle, 2 second triangle
    bool lineHit; V hitPos; V hitNormal; HitLoc loc;
    Thing() : dead(false), solid(true), rolling(false), hasHitLocs(true), flags(0), triHit(0), lineHit(false) { loc.id = 0; }
};
struct World {
    std::vector<Thing*> all; bool pauseNonScripted, pauseAll; Box lastBox; const Thing* lastIgnored; int triCalls, lineCalls;
    World() : pauseNonScripted(false), pauseAll(false), lastIgnored(0), triCalls(0), lineCalls(0) {}
};
struct Shot {
    bool hasTrail, extended, canHitRolling; std::vector<std::pair<V, V> > points; Thing* parent; Thing* shooter;
    std::vector<Thing*> hitList; HitLoc* best; int collisions; V collPos, collNormal; Thing* collThing; World* world;
    Shot() : hasTrail(true), extended(false), canHitRolling(false), parent(0), shooter(0), best(0), collisions(0), collThing(0), world(0) {}
    void UpdateCollision(bool& killed_me) {
        killed_me = false;
        bool useExtendedTrails = extended;
        if (!hasTrail || points.size() < 2) return;
        const std::pair<V, V>& last = *(points.end() - 1);
        const std::pair<V, V>& prev = *(points.end() - 2);
        V centre, line;
        if (useExtendedTrails) { centre = (last.first + prev.second) * 0.5f; line = last.first - prev.second; }
        else { centre = (prev.first + last.first) * 0.5f; line = last.first - prev.first; }
        long cx = (long)centre.X, cy = (long)centre.Y;
        long r = (long)(((float)std::sqrt(line.X * line.X + line.Y * line.Y + line.Z * line.Z) + 0.5f) * 1.5f);
        Box area = { cx - r, cy - r, cx + r, cy + r };
        world->lastBox = area;
        Thing* ignore = parent;
        if (shooter && !shooter->dead) ignore = shooter;
        world->lastIgnored = ignore;
        std::vector<Thing*> things;
        for (size_t i = 0; i < world->all.size(); ++i)
            if (world->all[i] != ignore && world->all[i]->solid) things.push_back(world->all[i]);
        float closest = 1.0e10f; Thing* closestThing = 0; HitLoc* closestLoc = 0; V closestPos, closestNormal;
        for (size_t i = 0; i < things.size(); ++i) {
            Thing* thing = things[i];
            if ((thing->flags & 0x20) && (world->pauseNonScripted || world->pauseAll)) continue;
            if (thing->rolling && !canHitRolling) continue;
            if (!thing->hasHitLocs || std::find(hitList.begin(), hitList.end(), thing) != hitList.end()) continue;
            HitLoc* loc = 0;
            if (useExtendedTrails) {
                ++world->triCalls; if (thing->triHit == 1) loc = &thing->loc;
                if (!loc) { ++world->triCalls; if (thing->triHit == 2) loc = &thing->loc; }
            } else { ++world->lineCalls; if (thing->lineHit) loc = &thing->loc; }
            if (loc) {
                V offset = thing->hitPos - prev.first;
                float d = offset.X * offset.X + offset.Y * offset.Y + offset.Z * offset.Z;
                if (d < closest) { closest = d; closestPos = thing->hitPos; closestNormal = thing->hitNormal; closestThing = thing; closestLoc = loc; }
            }
        }
        if (closestThing && !closestThing->dead) { best = closestLoc; ++collisions; collPos = closestPos; collNormal = closestNormal; collThing = closestThing; }
    }
};
static int fails = 0;
static void check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++fails; } }
static Shot MakeShot(World& w, Thing& parent) {
    Shot s; s.world = &w; s.parent = &parent;
    s.points.push_back(std::make_pair(V(0, 0, 0), V(0, 0, 0)));
    s.points.push_back(std::make_pair(V(10, 20, 0), V(12, 22, 0)));    // prev
    s.points.push_back(std::make_pair(V(30, 20, 0), V(32, 22, 0)));    // last
    return s;
}
int main() {
    // fewer than two points / no trail: only killed_me is cleared
    { World w; Thing p; Shot s = MakeShot(w, p); s.points.resize(1); bool k = true; s.UpdateCollision(k);
      check(!k && s.collisions == 0 && w.lastIgnored == 0, "one point"); }
    { World w; Thing p; Shot s = MakeShot(w, p); s.hasTrail = false; bool k = true; s.UpdateCollision(k); check(!k && w.lastIgnored == 0, "no trail"); }
    // line mode: box around the prev.first..last.first mid point, nearest hit wins, dead shooter -> parent ignored
    { World w; Thing p; Thing a, b, c; a.lineHit = true; a.hitPos = V(28, 20, 0); a.loc.id = 1; b.lineHit = true; b.hitPos = V(12, 20, 0); b.loc.id = 2;
      c.lineHit = false; Thing sh; sh.dead = true; w.all.push_back(&p); w.all.push_back(&a); w.all.push_back(&b); w.all.push_back(&c);
      Shot s = MakeShot(w, p); s.shooter = &sh; bool k = true; s.UpdateCollision(k);
      check(w.lastBox.TLX == 20 - 30 && w.lastBox.TLY == 20 - 30 && w.lastBox.BRX == 50 && w.lastBox.BRY == 50, "line box (r = (20 + 0.5) * 1.5 -> 30)");
      check(w.lastIgnored == &p && w.lineCalls == 3 && w.triCalls == 0, "dead shooter: parent ignored; one line test per thing");
      check(s.collisions == 1 && s.collThing == &b && s.best == &b.loc && s.collPos.X == 12.0f, "nearest hit to prev.first reported"); }
    // extended mode: centre of last.first and prev.second; second triangle only after the first misses; live shooter ignored
    { World w; Thing p; Thing sh; Thing a, b; a.triHit = 2; a.hitPos = V(14, 22, 0); b.triHit = 1; b.hitPos = V(30, 20, 0);
      w.all.push_back(&p); w.all.push_back(&sh); w.all.push_back(&a); w.all.push_back(&b);
      Shot s = MakeShot(w, p); s.extended = true; s.shooter = &sh; bool k = false; s.UpdateCollision(k);
      check(w.lastIgnored == &sh && w.triCalls == 2 + 2 + 1, "live shooter ignored (parent tested); first-triangle hit stops");
      check(w.lastBox.TLX == 21 - 27 && w.lastBox.BRY == 21 + 27, "extended box: centre (21, 21), r = (18.11 + 0.5) * 1.5 -> 27");
      check(s.collThing == &a, "second-triangle hit nearer prev.first wins"); }
    // filters: paused flag-0x20 thing, rolling thing, already-hit thing, no hit locations, non-solid; dead nearest -> no report
    { World w; Thing p; Thing a, b, c, d, e; a.flags = 0x20; b.rolling = true; d.hasHitLocs = false; e.solid = false;
      Thing* all[] = { &a, &b, &c, &d, &e }; for (int i = 0; i < 5; ++i) { all[i]->lineHit = true; all[i]->hitPos = V(11, 20, 0); w.all.push_back(all[i]); }
      w.pauseAll = true; Shot s = MakeShot(w, p); s.hitList.push_back(&c); bool k = true; s.UpdateCollision(k);
      check(w.lineCalls == 0 && s.collisions == 0, "every candidate filtered");
      w.pauseAll = false; s.canHitRolling = true; s.hitList.clear(); a.dead = true; a.hitPos = V(10, 20, 0); s.UpdateCollision(k);
      check(w.lineCalls == 3 && s.collisions == 0, "nearest thing dead: no collision"); }
    // equal distance keeps the first (strictly nearer)
    { World w; Thing p; Thing a, b; a.lineHit = b.lineHit = true; a.hitPos = b.hitPos = V(15, 20, 0); w.all.push_back(&a); w.all.push_back(&b);
      Shot s = MakeShot(w, p); bool k; s.UpdateCollision(k); check(s.collThing == &a, "tie keeps the first"); }
    if (fails == 0) std::printf("007bfe30_TEST PASS\n");
    return fails;
}