#include <cstdio>
#include <cmath>
// Behaviour test for CTCPhysicsStandard::GetOnSurfaceParameters (0x008E5480): the body of src/ over stand-ins -- the
// surface probe over colliding things (accepting hits within 0.15 above and above the lowest limit) and the on-ground test.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CSurfaceHit {
    C3DVector Pos;
    C3DVector Normal;
    float     Fraction;
    CSurfaceHit() : Fraction(1.0f) {}
};
class CTCPhysicsBase;
class CThing { public: CTCPhysicsBase* Physics; };
struct CThingVector { CThing** _M_start; CThing** _M_finish; };
static float g_lastStartZ;
class CTCPhysicsBase {
public:
    bool collides, oneWay;
    float surfaceZ;         // the Z a probe against this body reports (NaN-free); probe fails when < -100
    float Radius;
    float ground;
    bool CollidesWithThings() { return collides; }
    bool CollidesWithThingsOneWay() { return oneWay; }
    bool ProbeSurface(CTCPhysicsBase* other, const C3DVector& pos, const C3DVector& start, CSurfaceHit& hit)
    {
        g_lastStartZ = start.Z;
        if (other->surfaceZ < -100.0f) return false;
        hit.Pos = C3DVector(pos.X, pos.Y, other->surfaceZ);
        return true;
    }
    float GetGroundHeight(const C3DVector& pos) { return ground; }
};
class CTCPhysicsStandard : public CTCPhysicsBase {
public:
    void GetOnSurfaceParameters(const CThingVector& things, bool& onSurface, bool& onGround, const C3DVector& pos, CTCPhysicsBase*& surface);
};

void CTCPhysicsStandard::GetOnSurfaceParameters(const CThingVector& things, bool& onSurface, bool& onGround, const C3DVector& pos, CTCPhysicsBase*& surface)
{
    surface = 0;
    float groundHeight = GetGroundHeight(pos);
    if (CollidesWithThings() || CollidesWithThingsOneWay()) {
        onSurface = false;
        float radius = Radius;
        if (radius < 0.5f)
            radius = 0.5f;
        C3DVector down(0.0f, 0.0f, -radius);
        CSurfaceHit hit;
        float lowest = down.Z * 2.0f + pos.Z;
        for (CThing** it = things._M_start; it != things._M_finish; ++it) {
            CTCPhysicsBase* physics = (*it)->Physics;
            if (physics->CollidesWithThings()) {
                C3DVector start(pos.X, pos.Y, down.Z + pos.Z);
                if (ProbeSurface(physics, pos, start, hit)
                    && hit.Pos.Z - pos.Z < 0.15f && hit.Pos.Z > lowest) {
                    onSurface = true;
                    surface = physics;
                }
            }
        }
    }
    onGround = fabsf(pos.Z - groundHeight) < 0.04f;
}

static int fail(int line) { printf("008e5480_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CTCPhysicsBase a, b, c, d;
    a.collides = true;  a.surfaceZ = 10.5f;    // too high (0.5 above)
    b.collides = false; b.surfaceZ = 9.9f;     // does not collide
    c.collides = true;  c.surfaceZ = 9.95f;    // accepted
    d.collides = true;  d.surfaceZ = 8.0f;     // below the lowest limit (10 - 2*0.5 = 9)
    CThing ta = { &a }, tb = { &b }, tc = { &c }, td = { &d };
    CThing* list[4] = { &ta, &tb, &tc, &td };
    CThingVector things = { list, list + 4 };
    CTCPhysicsStandard self;
    self.collides = true; self.oneWay = false; self.Radius = 0.2f; self.ground = 9.98f;
    C3DVector pos(1, 2, 10);
    bool onSurface = false, onGround = false;
    CTCPhysicsBase* surface = &a;
    self.GetOnSurfaceParameters(things, onSurface, onGround, pos, surface);
    CHECK(onSurface && surface == &c && onGround && g_lastStartZ == 9.5f);
    // larger radius widens the lowest limit: d accepted (last wins)
    self.Radius = 1.5f; self.ground = 0.0f;
    self.GetOnSurfaceParameters(things, onSurface, onGround, pos, surface);
    CHECK(onSurface && surface == &d && !onGround && g_lastStartZ == 8.5f);
    // not colliding at all: onSurface untouched, surface reset
    self.collides = false; onSurface = true;
    self.GetOnSurfaceParameters(things, onSurface, onGround, pos, surface);
    CHECK(onSurface && surface == 0);
    // one-way collider probes too; nothing acceptable
    self.oneWay = true; self.Radius = 0.5f; c.surfaceZ = -1000.0f; d.surfaceZ = -1000.0f;
    self.GetOnSurfaceParameters(things, onSurface, onGround, pos, surface);
    CHECK(!onSurface && surface == 0);
    printf("008e5480_TEST PASS\n");
    return 0;
}