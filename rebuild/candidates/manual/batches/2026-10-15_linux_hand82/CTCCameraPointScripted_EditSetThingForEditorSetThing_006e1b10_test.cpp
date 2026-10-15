// Behaviour test for 006e1b10 (catalog EditSetThingForEditorSetThing): returns a copy of the stored position.
#include <cstdio>

struct C3DVector
{
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

struct CPositionedObject_006e1b10
{
    C3DVector Position;
    CPositionedObject_006e1b10() : Position(7.0f, 8.0f, 9.0f) {}
    C3DVector GetPosition() const { return C3DVector(Position.X, Position.Y, Position.Z); }
};

int main()
{
    CPositionedObject_006e1b10 o;
    C3DVector p = o.GetPosition();
    if (p.X != 7.0f || p.Y != 8.0f || p.Z != 9.0f) { std::printf("006e1b10_TEST FAIL\n"); return 1; }
    std::printf("006e1b10_TEST PASS\n");
    return 0;
}
