#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CWorldMap { public: virtual ~CWorldMap() { } virtual long GetMapNumberAt(const C3DVector& pos) { return (long)pos.X + 70000; } };
static CWorldMap g_map;
class CThing { public: unsigned short MapNumber; void SetCurrentMapNumber(unsigned short m) { MapNumber = m; } };
static int g_moving = -1;
struct CTCPhysicsLight {
    CThing* sub;
    C3DVector Position;
    CWorldMap* GetWorldMap() { return &g_map; }
    void SetAsMoving(bool moving) { g_moving = moving ? 1 : 0; }
    void SetPosition(const C3DVector& pos)
    {
        Position = pos;
        sub->SetCurrentMapNumber(GetWorldMap()->GetMapNumberAt(pos));
        SetAsMoving(true);
    }
};
int main()
{
    CThing t; t.MapNumber = 0; CTCPhysicsLight l; l.sub = &t;
    C3DVector p; p.X = 5.0f; p.Y = 6.0f; p.Z = 7.0f;
    l.SetPosition(p);
    if (l.Position.Y != 6.0f || t.MapNumber != (unsigned short)70005 || g_moving != 1) { std::printf("007233b0_TEST FAIL\n"); return 1; }
    std::printf("007233b0_TEST PASS\n");
    return 0;
}