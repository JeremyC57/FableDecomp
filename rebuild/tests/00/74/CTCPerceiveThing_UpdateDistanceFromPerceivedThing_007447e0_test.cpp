// Behaviour test for CTCPerceiveThing::UpdateDistanceFromPerceivedThing (0x007447E0): a model of the row -- the squared
// distance between the owner's and the perceived thing's positions is stored in DistanceFromPerceivedThingSquared.
#include <stdio.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetSquaredMagnitude() const { return (X * X + Z * Z + Y * Y); }
};
struct CThing { C3DVector Pos; CThing(float x, float y, float z) : Pos(x, y, z) { } const C3DVector& GetPos() const { return Pos; } };
struct CTCPerceiveThing {
    CThing* Owner; CThing* Perceived; float DistanceFromPerceivedThingSquared;
    void UpdateDistanceFromPerceivedThing() {
        CThing* perceived = Perceived;
        CThing* owner = Owner;
        DistanceFromPerceivedThingSquared = (owner->GetPos() - perceived->GetPos()).GetSquaredMagnitude();
    }
};
int main()
{
    CThing a(1.0f, 2.0f, 3.0f), b(4.0f, 6.0f, 15.0f);
    CTCPerceiveThing p; p.Owner = &a; p.Perceived = &b; p.DistanceFromPerceivedThingSquared = -1.0f;
    p.UpdateDistanceFromPerceivedThing();
    float d = p.DistanceFromPerceivedThingSquared - 169.0f;
    int ok = d < 1e-4f && d > -1e-4f;
    printf(ok ? "007447e0_TEST PASS\n" : "007447e0_TEST FAIL\n");
    return ok ? 0 : 1;
}