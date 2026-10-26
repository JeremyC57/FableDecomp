// Behaviour test for 0x00853290 NCameraModes::CScriptModuleBase::GetTransformedPosition: live, dead and
// absent origin things.
#include <cstdio>
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
class CThing { public: bool m_dead; C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct IP { const CThing* item; const CThing* GetPItem() const { return item; } const CThing* operator->() const { return item; } };
struct Module {
    IP PThingOrigin; C3DVector CoordOrigin;
    C3DVector GetTransformedPosition(const C3DVector& offset)
    {
        C3DVector origin = CoordOrigin;
        if (PThingOrigin.GetPItem()) {
            if (!PThingOrigin->m_dead) origin = PThingOrigin->GetPosition();
        }
        return C3DVector(origin.X + offset.X, origin.Y + offset.Y, origin.Z + offset.Z);
    }
};
int main()
{
    CThing t; t.m_dead = false; t.pos = C3DVector(10, 20, 30);
    Module m; m.PThingOrigin.item = &t; m.CoordOrigin = C3DVector(1, 2, 3);
    C3DVector off(1, 1, 1);
    C3DVector a = m.GetTransformedPosition(off);
    t.m_dead = true; C3DVector b = m.GetTransformedPosition(off);
    m.PThingOrigin.item = 0; C3DVector c = m.GetTransformedPosition(off);
    if (a.X != 11 || a.Z != 31 || b.X != 2 || b.Z != 4 || c.Y != 3) { std::printf("00853290_TEST FAIL\n"); return 1; }
    std::printf("00853290_TEST PASS\n");
    return 0;
}