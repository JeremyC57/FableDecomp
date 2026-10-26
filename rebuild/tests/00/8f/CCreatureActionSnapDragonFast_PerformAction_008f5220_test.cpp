// Behaviour test for CCreatureAction_SnapDragonFast::PerformAction (0x008F5220): the mouth dummy is dropped to the
// dragon's height and pushed 1.5 units along the facing; an obstruction is created there (radius 1.5) whose driver
// gets 1.5 and ignores the dragon; the bite explosion is created 2.5 units behind the mouth and owned by the dragon.
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};

struct Thing {
    C3DVector pos, facing, dummy;
    bool hasDriver;
    float driverValue;
    Thing* ignored;
    Thing* owner;
};

static Thing g_obstruction, g_bite;
static C3DVector g_obsPos, g_obsLook, g_bitePos, g_biteLook;
static float g_radius;
static long g_def;
static bool g_makeObstruction = true, g_makeBite = true;

static Thing* CreateObstruction(const C3DVector* a, const C3DVector* b, float r)
{ g_obsPos = *a; g_obsLook = *b; g_radius = r; return g_makeObstruction ? &g_obstruction : 0; }
static Thing* CreateThing(long def, const C3DVector* a, const C3DVector* b)
{ g_def = def; g_bitePos = *a; g_biteLook = *b; return g_makeBite ? &g_bite : 0; }

static void PerformAction(Thing* dragon)
{
    C3DVector mouth = dragon->pos;
    mouth = dragon->dummy;                       // GetDummyObject fills the mouth position
    mouth.Z = dragon->pos.Z;
    C3DVector offset = dragon->facing * 1.5f;
    mouth += offset;
    Thing* obstruction = CreateObstruction(&mouth, &dragon->pos, 1.5f);
    if (obstruction) {
        obstruction->driverValue = 1.5f;
        obstruction->ignored = dragon;
    }
    long explosion = 77;
    C3DVector position = mouth - dragon->facing * 2.5f;
    Thing* bite = CreateThing(explosion, &position, &dragon->pos);
    if (bite)
        bite->owner = dragon;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    Thing dragon;
    dragon.pos = C3DVector(10.0f, 20.0f, 3.0f);
    dragon.facing = C3DVector(1.0f, 0.0f, 0.0f);
    dragon.dummy = C3DVector(11.0f, 21.0f, 7.0f);
    PerformAction(&dragon);
    if (!Near(g_obsPos.X, 12.5f) || !Near(g_obsPos.Y, 21.0f) || !Near(g_obsPos.Z, 3.0f) || !Near(g_radius, 1.5f)
        || !Near(g_obsLook.X, 10.0f) || !Near(g_obstruction.driverValue, 1.5f) || g_obstruction.ignored != &dragon) {
        std::printf("008f5220_TEST FAIL\n");
        return 1;
    }
    if (g_def != 77 || !Near(g_bitePos.X, 10.0f) || !Near(g_bitePos.Y, 21.0f) || !Near(g_bitePos.Z, 3.0f)
        || g_bite.owner != &dragon || !Near(g_biteLook.Y, 20.0f)) {
        std::printf("008f5220_TEST FAIL\n");
        return 1;
    }
    g_makeObstruction = false; g_makeBite = false; g_bite.owner = 0;
    PerformAction(&dragon);
    if (g_bite.owner != 0) {
        std::printf("008f5220_TEST FAIL\n");
        return 1;
    }
    std::printf("008f5220_TEST PASS\n");
    return 0;
}