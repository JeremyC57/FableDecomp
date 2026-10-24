// Behaviour test for 0x00864170: IsThingRunningAwayFromThing = more than 4 units (2D) from the position AND the
// normalised 2D facing . normalised 2D direction-to-other below the global threshold.
#include <cstdio>
#include <math.h>

float g_RunningAwayDotThreshold = -0.5f;

class C2DVector;

class C3DVector
{
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& r) const { return C3DVector(X - r.X, Y - r.Y, Z - r.Z); }
    C2DVector Get2D() const;
};

class C2DVector
{
public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector& Normalise()
    {
        float l = (float)sqrt(X * X + Y * Y);
        if (l > 0.0f) { X /= l; Y /= l; }
        return *this;
    }
    float DotProduct(const C2DVector& o) const { return Y * o.Y + X * o.X; }
};

inline C2DVector C3DVector::Get2D() const { return C2DVector(X, Y); }

class CTCPhysicsBase
{
public:
    C3DVector m_position;
    C3DVector m_facing;
    CTCPhysicsBase() : m_position(0, 0, 0), m_facing(1, 0, 0) { }
    virtual ~CTCPhysicsBase() { }
    virtual C3DVector GetFacingVector() const { return m_facing; }
};

class CThing
{
public:
    CTCPhysicsBase* PhysicsTC;
    const C3DVector& GetPosition() const { return PhysicsTC->m_position; }
};

bool IsThingRunningAwayFromThing(const CThing& thing, const CThing& other, const C3DVector& pos)
{
    C2DVector facing(thing.PhysicsTC->GetFacingVector().Get2D());
    C2DVector toOther(other.GetPosition() - thing.GetPosition());
    facing.Normalise();
    toOther.Normalise();
    const C3DVector& now = thing.GetPosition();
    float dx = now.X - pos.X;
    float dy = now.Y - pos.Y;
    bool farEnough = (float)sqrt(dx * dx + dy * dy) > 4.0f;
    bool facingAway = facing.DotProduct(toOther) < g_RunningAwayDotThreshold;
    if (farEnough && facingAway)
        return true;
    return false;
}

int main()
{
    CTCPhysicsBase pa, pb;
    CThing a; a.PhysicsTC = &pa;
    CThing b; b.PhysicsTC = &pb;
    pa.m_position = C3DVector(0, 0, 0);
    pb.m_position = C3DVector(10, 0, 0);       // other is along +X
    pa.m_facing = C3DVector(-2, 0, 5);         // facing -X (Z ignored): away from b
    C3DVector farPos(10, 10, 0);
    C3DVector nearPos(1, 1, 0);
    int fails = 0;
    if (!IsThingRunningAwayFromThing(a, b, farPos)) ++fails;
    if (IsThingRunningAwayFromThing(a, b, nearPos)) ++fails;    // within 4 units
    pa.m_facing = C3DVector(3, 0, 0);          // facing the other thing
    if (IsThingRunningAwayFromThing(a, b, farPos)) ++fails;
    pa.m_facing = C3DVector(0, 1, 0);          // perpendicular: dot 0 is not below -0.5
    if (IsThingRunningAwayFromThing(a, b, farPos)) ++fails;
    if (fails) { std::printf("00864170_TEST FAIL\n"); return 1; }
    std::printf("00864170_TEST PASS\n");
    return 0;
}