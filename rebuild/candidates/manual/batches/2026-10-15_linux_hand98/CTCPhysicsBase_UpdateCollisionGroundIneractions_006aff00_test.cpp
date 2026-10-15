#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() { } };
static long g_lastFlags = -1;
struct CWorldMap {
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos, long flags) { g_lastFlags = flags; C3DVector n; n.X = 0.0f; n.Y = 0.0f; n.Z = pos.Z + 1.0f; return n; }
};
static CWorldMap g_map;
struct CWorld { virtual ~CWorld() { } virtual CWorldMap* GetWorldMap() { return &g_map; } };
static CWorld g_world;
static long g_kind = 0; static C3DVector g_pos, g_normal;
struct CThing { virtual ~CThing() { } virtual void OnCollision(long kind, const C3DVector& pos, const C3DVector& normal) { g_kind = kind; g_pos = pos; g_normal = normal; } };
struct CTCPhysicsBase {
    CThing* sub;
    CWorld* GetWorld() { return &g_world; }
    void UpdateCollisionGroundIneractions(const C3DVector& pos)
    {
        CThing* thing = sub;
        thing->OnCollision(3, pos, GetWorld()->GetWorldMap()->GetGroundFaceNormalAt(pos, 0));
    }
};
int main()
{
    CThing t; CTCPhysicsBase b; b.sub = &t;
    C3DVector p; p.X = 1.0f; p.Y = 2.0f; p.Z = 3.0f;
    b.UpdateCollisionGroundIneractions(p);
    if (g_kind != 3 || g_pos.Y != 2.0f || g_normal.Z != 4.0f || g_lastFlags != 0) { std::printf("006aff00_TEST FAIL\n"); return 1; }
    std::printf("006aff00_TEST PASS\n");
    return 0;
}
