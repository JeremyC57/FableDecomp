// Behaviour test for CWorldMap::IsLineOfSight(const CThing&, const CThing&, bool) @ 0x0063E6F0: each thing's eye point is its
// head (creatures), else its graphic focal point, else its physics position -- for the SECOND thing only the creature head
// reaches the second point (the retail copy-paste slip writes the other two into the first); both points must be in the same
// nonzero map and the terrain line clear; with checkThings the game objects in the box between the points are gathered
#include <stdio.h>
#include <vector>
struct Vec { float X, Y, Z; };
static Vec V(float x, float y, float z) { Vec v; v.X = x; v.Y = y; v.Z = z; return v; }
struct Thing { bool creature; bool hasGraphic; Vec head, focal, position; };
struct Box { long TLX, TLY, BRX, BRY; };
static std::vector<Box> g_Searches;
static long MapAt(const Vec& p) { return p.X < 0.0f ? 0 : (p.X < 100.0f ? 1 : 2); }
static bool TerrainClear(const Vec& a, const Vec& b) { return a.Z > 0.0f && b.Z > 0.0f; }
static bool IsLineOfSight(const Thing& from, const Thing& to, bool checkThings)
{
    Vec fromPos = V(0, 0, 0), toPos = V(0, 0, 0);
    if (from.creature) fromPos = from.head; else if (from.hasGraphic) fromPos = from.focal; else fromPos = from.position;
    if (to.creature) toPos = to.head; else if (to.hasGraphic) fromPos = to.focal; else fromPos = to.position;
    long fromMap = MapAt(fromPos), toMap = MapAt(toPos);
    if (fromMap == 0 || fromMap != toMap) return false;
    if (!TerrainClear(fromPos, toPos)) return false;
    if (checkThings) {
        Box b;
        b.TLX = (long)(fromPos.X < toPos.X ? fromPos.X : toPos.X);
        b.TLY = (long)(fromPos.Y < toPos.Y ? fromPos.Y : toPos.Y);
        b.BRX = (long)(fromPos.X > toPos.X ? fromPos.X : toPos.X);
        b.BRY = (long)(fromPos.Y > toPos.Y ? fromPos.Y : toPos.Y);
        g_Searches.push_back(b);
    }
    return true;
}
int main()
{
    Thing hero = { true, true, V(10.25f, 20.5f, 2.0f), V(0, 0, 0), V(10, 20, 0) };
    Thing tree = { false, true, V(0, 0, 0), V(50.5f, 5.75f, 3.0f), V(50, 5, 0) };
    Thing guard = { true, false, V(40.5f, 30.25f, 2.0f), V(0, 0, 0), V(40, 30, 0) };
    Thing distant = { true, false, V(150.5f, 30.25f, 2.0f), V(0, 0, 0), V(150, 30, 0) };
    int ok = IsLineOfSight(hero, guard, true) && g_Searches.size() == 1;
    ok = ok && g_Searches[0].TLX == 10 && g_Searches[0].TLY == 20 && g_Searches[0].BRX == 40 && g_Searches[0].BRY == 30;
    ok = ok && !IsLineOfSight(hero, distant, true) && g_Searches.size() == 1;    // different maps
    ok = ok && IsLineOfSight(hero, guard, false) && g_Searches.size() == 1;   // no search without checkThings
    // the slip: a non-creature second thing overwrites the first point and leaves the second at the origin (map 1 here)
    ok = ok && !IsLineOfSight(hero, tree, false);                             // second point has Z 0: terrain test fails
    printf(ok ? "0063e6f0_TEST PASS\n" : "0063e6f0_TEST FAIL\n");
    return ok ? 0 : 1;
}