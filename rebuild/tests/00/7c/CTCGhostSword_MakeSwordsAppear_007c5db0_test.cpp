// Behaviour test for CTCGhostSword::MakeSwordsAppear @ 0x007C5DB0: sword positions per spell level from the facing vector,
// its quarter-turn side vector scaled by the creation distance, and the caster position.
#include <stdio.h>
#include <math.h>
class C3DVector { public: float X, Y, Z;
    C3DVector() { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Rotate(const C3DVector& axis, float angle)   // rotation about +Z by angle turns (axis is (0,0,1) here)
    { double a = angle * 6.283185307179586 * (axis.Z > 0 ? 1 : -1); float x = (float)(X * cos(a) - Y * sin(a)); float y = (float)(X * sin(a) + Y * cos(a)); X = x; Y = y; }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); } };
class CTCPhysicsBase { public: C3DVector Facing; C3DVector GetFacingVector() const { return Facing; } };
class CThing { public: CTCPhysicsBase* PhysicsTC; C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };
struct CDef { float CreationDistance; };
class CTCGhostSword { public: CThing* ParentThing; const CDef* PDef; long SpellLevel; C3DVector Swords[8]; int Count;
    void AddSword(const C3DVector& p) { Swords[Count++] = p; }
    void MakeSwordsAppear() {
        CThing* parent = ParentThing;
        C3DVector facing = parent->PhysicsTC->GetFacingVector();
        C3DVector side = facing;
        side.Rotate(C3DVector(0.0f, 0.0f, 1.0f), 0.25f);
        side *= PDef->CreationDistance;
        C3DVector pos = parent->GetPos();
        switch (SpellLevel) {
        case 0: AddSword(facing + pos); break;
        case 1: AddSword(side + pos); AddSword(pos - side); break;
        case 2: AddSword(facing + pos); AddSword(side + pos); AddSword(pos - side); break;
        case 3: AddSword(facing + pos); AddSword(pos - facing); AddSword(side + pos); AddSword(pos - side); break;
        }
    } };
static bool Near(const C3DVector& v, float x, float y, float z) { return fabs(v.X - x) < 1e-4 && fabs(v.Y - y) < 1e-4 && fabs(v.Z - z) < 1e-4; }
int main()
{
    CTCPhysicsBase phys; phys.Facing = C3DVector(1, 0, 0);
    CThing thing; thing.PhysicsTC = &phys; thing.Pos = C3DVector(10, 20, 30);
    CDef def = { 2.0f };
    CTCGhostSword s; s.ParentThing = &thing; s.PDef = &def;
    int expect[5] = { 1, 2, 3, 4, 0 };
    for (long level = 0; level < 5; ++level) {
        s.SpellLevel = level; s.Count = 0; s.MakeSwordsAppear();
        if (s.Count != expect[level]) { printf("007c5db0_TEST FAIL\n"); return 1; }
    }
    s.SpellLevel = 3; s.Count = 0; s.MakeSwordsAppear();
    if (!Near(s.Swords[0], 11, 20, 30) || !Near(s.Swords[1], 9, 20, 30) || !Near(s.Swords[2], 10, 22, 30) || !Near(s.Swords[3], 10, 18, 30)) {
        printf("007c5db0_TEST FAIL\n"); return 1;
    }
    printf("007c5db0_TEST PASS\n");
    return 0;
}