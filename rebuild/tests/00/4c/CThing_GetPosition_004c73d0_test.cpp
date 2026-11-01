// Test for 0x004C73D0 CThing::GetPosition() const: links the source object; the result is the address of the
// physics component's Position (+0x0C), so it tracks later writes and follows a changed PhysicsTC.
#include <stdio.h>
class C3DVector { public: float X, Y, Z; };
class CTCPhysicsBase { public: unsigned char _base[0x0c]; C3DVector Position; };
class CThing {
public:
    unsigned char   _pad[0x60];
    CTCPhysicsBase* PhysicsTC;
    const C3DVector& GetPos() const;
};
int main()
{
    CTCPhysicsBase a, b; CThing t;
    a.Position.X = 1.0f; a.Position.Y = 2.0f; a.Position.Z = 3.0f;
    b.Position.X = -4.0f; b.Position.Y = 0.5f; b.Position.Z = 8.0f;
    t.PhysicsTC = &a;
    const C3DVector& p = t.GetPos();
    bool ok = &p == &a.Position && (const char*)&p == (const char*)&a + 0x0c && p.Y == 2.0f;
    a.Position.Y = 7.0f;
    ok = ok && t.GetPos().Y == 7.0f;
    t.PhysicsTC = &b;
    ok = ok && &t.GetPos() == &b.Position && t.GetPos().Z == 8.0f;
    if (!ok) { puts("004c73d0 FAIL"); return 1; }
    puts("OK_0x004c73d0");
    return 0;
}
