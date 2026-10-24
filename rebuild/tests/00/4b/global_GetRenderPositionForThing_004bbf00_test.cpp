// Behaviour test for 0x004BBF00 GetRenderPositionForThing(thing, t): the physics position interpolated from the old
// position by t (exactly Position when t == 1), plus the physics render Z offset when the thing is a creature (type 2).
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct CTCPhysicsBase {
    C3DVector Position, OldPosition;
    float ZOffset;
    int calls;
    virtual float GetRenderZOffset(float t) const { const_cast<CTCPhysicsBase*>(this)->calls++; return ZOffset * t; }
    C3DVector GetPosition(float t) const { return t == 1.0f ? Position : (Position - OldPosition) * t + OldPosition; }
    virtual ~CTCPhysicsBase() {}
};
struct CThing { unsigned char ThingType; CTCPhysicsBase* PhysicsTC; };

static C3DVector GetRenderPositionForThing(const CThing& thing, float t)
{
    const CTCPhysicsBase* physics = thing.PhysicsTC;
    C3DVector pos = physics->GetPosition(t);
    if (thing.ThingType == 2)
        pos.Z += physics->GetRenderZOffset(t);
    return pos;
}

int main()
{
    CTCPhysicsBase p; p.Position = C3DVector(10.0f, 20.0f, 30.0f); p.OldPosition = C3DVector(0.0f, 4.0f, 6.0f);
    p.ZOffset = 8.0f; p.calls = 0;
    CThing obj = { 1, &p };
    C3DVector a = GetRenderPositionForThing(obj, 1.0f);
    C3DVector b = GetRenderPositionForThing(obj, 0.5f);
    if (a.X != 10.0f || a.Y != 20.0f || a.Z != 30.0f || b.X != 5.0f || b.Y != 12.0f || b.Z != 18.0f || p.calls != 0) {
        std::printf("004bbf00_TEST FAIL\n"); return 1;
    }
    CThing creature = { 2, &p };
    C3DVector c = GetRenderPositionForThing(creature, 0.5f);
    if (c.X != 5.0f || c.Y != 12.0f || c.Z != 22.0f || p.calls != 1) { std::printf("004bbf00_TEST FAIL\n"); return 1; }
    std::printf("004bbf00_TEST PASS\n");
    return 0;
}