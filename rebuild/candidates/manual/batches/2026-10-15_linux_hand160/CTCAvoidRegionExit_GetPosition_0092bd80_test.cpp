// Behaviour test for CTCAvoidRegionExit::GetPosition (0x0092BD80): the entrance marker's position while it is set and alive,
// the zero vector otherwise.
#include <cstdio>
struct Vec3 { float X, Y, Z; Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {} Vec3(const Vec3& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct Thing { unsigned char Flags91; Vec3 Pos; Thing() : Flags91(0), Pos(1, 2, 3) {} bool IsDead() const { return (Flags91 & 1) != 0; }
    const Vec3& GetPosition() const { return Pos; } };
struct IPtr { Thing* item; Thing* GetPItem() const { return item; } };
struct Avoid {
    IPtr EntranceMarker;
    bool HasPosition() { return EntranceMarker.GetPItem() && !EntranceMarker.GetPItem()->IsDead(); }
    Vec3 GetPosition() { return HasPosition() ? EntranceMarker.GetPItem()->GetPosition() : Vec3(0.0f, 0.0f, 0.0f); }
};
int main()
{
    Thing t; Avoid a; a.EntranceMarker.item = &t;
    Vec3 p = a.GetPosition();
    if (p.X != 1 || p.Y != 2 || p.Z != 3) { std::printf("0092bd80_TEST FAIL\n"); return 1; }
    t.Flags91 = 1; Vec3 q = a.GetPosition();
    a.EntranceMarker.item = 0; Vec3 r = a.GetPosition();
    if (q.X != 0 || q.Z != 0 || r.Y != 0) { std::printf("0092bd80_TEST FAIL\n"); return 1; }
    std::printf("0092bd80_TEST PASS\n");
    return 0;
}
