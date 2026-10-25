// Behaviour test for CPhysicalPrimitiveCylinder::GetCylinderBaseAndTop (0x008158D0): with an empty base name the base
// is the physics position plus up * the def's base offset and the top is base + up * the def's cylinder height (the
// primitive's own Height is ignored); with a named base dummy object the base and up axis come from the dummy object
// and the top is base + up * GetCylinderHeight (the primitive's Height, or the def's height when below the epsilon).
// The def pointer is add-ref'd and released in balance on every path.
#include <cstdio>
#include <cstring>

struct Vec { float X, Y, Z; };
static Vec V(float x, float y, float z) { Vec v; v.X = x; v.Y = y; v.Z = z; return v; }
static Vec Add(const Vec& a, const Vec& b) { return V(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static Vec Mul(const Vec& a, float f) { return V(a.X * f, a.Y * f, a.Z * f); }

static const float kEpsilon = 1e-4f;

struct Def { long refCount; int destroyed; float baseOffset; float cylinderHeight; };
static void AddRef(Def* d) { if (d) ++d->refCount; }
static void Release(Def* d) { if (d && --d->refCount == 0) ++d->destroyed; }

struct Physics { Vec position; Vec up; };
struct Graphic { const char* dummyName; Vec dummyPos; Vec dummyUp; int lookups; };
struct Thing { Physics* physics; Graphic* graphic; Def* def; };
struct Cylinder { const char* baseName; float radius; float height; };

static float GetCylinderHeight(const Cylinder& c, const Thing& thing)
{
    Def* def = thing.def;
    AddRef(def);
    float h = c.height;
    if (h < kEpsilon)
        h = def->cylinderHeight;
    Release(def);
    return h;
}

static void GetCylinderBaseAndTop(const Cylinder& c, const Thing& thing, Vec& base, Vec& top)
{
    Physics* physics = thing.physics;
    Def* def = thing.def;
    AddRef(def);
    if (strcmp(c.baseName, "") == 0) {
        Vec up = physics->up;
        base = Add(Mul(up, def->baseOffset), physics->position);
        top = Add(Mul(up, def->cylinderHeight), base);
    }
    else {
        Vec up = V(0, 0, 0);
        ++thing.graphic->lookups;
        if (strcmp(thing.graphic->dummyName, c.baseName) == 0) {
            base = thing.graphic->dummyPos;
            up = thing.graphic->dummyUp;
        }
        top = Add(Mul(up, GetCylinderHeight(c, thing)), base);
    }
    Release(def);
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
static bool Same(const Vec& a, float x, float y, float z) { return Near(a.X, x) && Near(a.Y, y) && Near(a.Z, z); }

int main()
{
    int fails = 0;
    Def def = { 1, 0, 0.5f, 2.0f };
    Physics physics = { V(10, 20, 30), V(0, 1, 0) };
    Graphic graphic = { "hip", V(1, 2, 3), V(0, 0, 1), 0 };
    Thing thing = { &physics, &graphic, &def };

    // empty base name: physics position + up * offset, top uses the def height (primitive height ignored)
    Cylinder plain = { "", 0.3f, 7.0f };
    Vec base, top;
    GetCylinderBaseAndTop(plain, thing, base, top);
    if (!Same(base, 10, 20.5f, 30) || !Same(top, 10, 22.5f, 30)) ++fails;
    if (graphic.lookups != 0) ++fails;

    // named dummy object, primitive height set: top = dummy pos + dummy up * height
    Cylinder named = { "hip", 0.3f, 4.0f };
    GetCylinderBaseAndTop(named, thing, base, top);
    if (!Same(base, 1, 2, 3) || !Same(top, 1, 2, 7)) ++fails;
    if (graphic.lookups != 1) ++fails;

    // named dummy object, primitive height below epsilon: falls back to the def height
    Cylinder fallback = { "hip", 0.3f, 0.0f };
    GetCylinderBaseAndTop(fallback, thing, base, top);
    if (!Same(top, 1, 2, 5)) ++fails;

    // ref counting balanced, def never destroyed
    if (def.refCount != 1 || def.destroyed != 0) ++fails;

    if (fails) { printf("008158d0_TEST FAIL\n"); return 1; }
    printf("008158d0_TEST PASS\n");
    return 0;
}