// Behaviour test for CTCPhysicsBase::SetPosition (0x006B0C10): a body in the map-who refuses a move into another map that is
// not loaded (collision kind 4, position unchanged); otherwise the map-who entry moves, the appearance updates, the old
// position is saved, the position is stored and the thing's map number follows the new position.
#include <cstdio>

struct V3 { float X, Y, Z; };
struct World {
    float mapWidth; bool loaded[4];
    long MapNumber(const V3& p) const { return (long)(p.X / mapWidth); }
    bool IsMapLoaded(long m) const { return m >= 0 && m < 4 && loaded[m]; }
};
struct Body {
    V3 position, oldPosition, offset, mapwhoPos; bool inMapwho;
    int collisions, collisionKind, appearanceUpdates; long mapNumber;
};
static V3 Offset(const V3& p, const V3& o) { V3 r; r.X = p.X + o.X; r.Y = p.Y + o.Y; r.Z = p.Z; return r; }
static void SetPosition(Body& b, const World& w, const V3& pos)
{
    if (b.inMapwho) {
        long oldMap = w.MapNumber(Offset(b.position, b.offset));
        long newMap = w.MapNumber(Offset(pos, b.offset));
        if (oldMap != newMap) {
            if (newMap < 1 || !w.IsMapLoaded(newMap)) {
                ++b.collisions; b.collisionKind = 4;
                return;
            }
        }
        b.mapwhoPos = Offset(pos, b.offset);
        ++b.appearanceUpdates;
    }
    b.oldPosition = b.position;
    b.position = pos;
    b.mapNumber = w.MapNumber(b.position);
}
static bool Fail() { std::printf("006b0c10_TEST FAIL\n"); return true; }
int main()
{
    World w; w.mapWidth = 100.0f; w.loaded[0] = true; w.loaded[1] = true; w.loaded[2] = false; w.loaded[3] = true;
    Body b; b.position.X = 150.0f; b.position.Y = 0.0f; b.position.Z = 0.0f; b.oldPosition = b.position;
    b.offset.X = 1.0f; b.offset.Y = 2.0f; b.offset.Z = 0.0f; b.mapwhoPos = b.position; b.inMapwho = true;
    b.collisions = 0; b.collisionKind = 0; b.appearanceUpdates = 0; b.mapNumber = 1;
    V3 same; same.X = 160.0f; same.Y = 5.0f; same.Z = 3.0f;
    SetPosition(b, w, same);
    if (b.position.X != 160.0f || b.oldPosition.X != 150.0f || b.appearanceUpdates != 1 || b.mapwhoPos.Y != 7.0f || b.mapNumber != 1)
        if (Fail()) return 1;
    V3 unloaded; unloaded.X = 250.0f; unloaded.Y = 0.0f; unloaded.Z = 0.0f;
    SetPosition(b, w, unloaded);
    if (b.collisions != 1 || b.collisionKind != 4 || b.position.X != 160.0f || b.appearanceUpdates != 1)
        if (Fail()) return 1;
    V3 zero; zero.X = 10.0f; zero.Y = 0.0f; zero.Z = 0.0f;
    SetPosition(b, w, zero);
    if (b.collisions != 2 || b.position.X != 160.0f)
        if (Fail()) return 1;
    V3 loaded; loaded.X = 350.0f; loaded.Y = 0.0f; loaded.Z = 0.0f;
    SetPosition(b, w, loaded);
    if (b.position.X != 350.0f || b.mapNumber != 3 || b.appearanceUpdates != 2)
        if (Fail()) return 1;
    b.inMapwho = false;
    SetPosition(b, w, unloaded);
    if (b.position.X != 250.0f || b.mapNumber != 2 || b.collisions != 2 || b.appearanceUpdates != 2)
        if (Fail()) return 1;
    std::printf("006b0c10_TEST PASS\n");
    return 0;
}