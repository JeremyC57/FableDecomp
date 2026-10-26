// Behaviour test for CThingSwitch::Create (0x0083C5D0): by-value name; allocate, construct, place the physics at pos with identity axes, activate.
#include <cstdio>
#include <stddef.h>
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet() { Up.X = 0; Up.Y = 0; Up.Z = 1; Forward.X = 0; Forward.Y = 1; Forward.Z = 0; }
};
static int g_paramsLive = 0;
class CTCParamsPhysicsBase { public:
    C3DVector Position; CRightHandedSet Rhs;
    CTCParamsPhysicsBase(const C3DVector& p, const CRightHandedSet& a) : Position(p), Rhs(a) { ++g_paramsLive; }
    ~CTCParamsPhysicsBase() { --g_paramsLive; }
};
struct CTCPhysics {
    C3DVector pos; CRightHandedSet rhs; int calls;
    virtual void ConstructFromParams(const CTCParamsPhysicsBase& params) { pos = params.Position; rhs = params.Rhs; ++calls; }
};
static bool g_alloc = true, g_construct = true, g_activate = true;
static long g_args[2]; static const C3DVector* g_pos = 0;
static double g_storage[64];
static CTCPhysics g_physics;

class CThing {
public:
    CTCPhysics* PhysicsTC;
    bool DoActivationSequence() { return g_activate; }
    static void* operator new(size_t, bool) throw() { return g_alloc ? g_storage : 0; }
    static void operator delete(void*, bool) throw() {}
};
static int g_strings = 0; static float g_radius = 0; static const char* g_text = 0;
class CCharString { public:
    const char* text;
    CCharString(const char* t) : text(t) { ++g_strings; }
    CCharString(const CCharString& o) : text(o.text) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
class CThingSwitch : public CThing {
public:
    CThingSwitch() { PhysicsTC = &g_physics; }
    bool ConstructFromParams(long defIndex, const C3DVector& pos, float radius, CCharString scriptName) { g_args[0] = defIndex; g_args[1] = 0; g_pos = &pos; g_radius = radius; g_text = scriptName.text; return g_construct; }
    static CThingSwitch* Create(long defIndex, const C3DVector& pos, float radius, CCharString scriptName)
    {
        CThingSwitch* p = new (false) CThingSwitch;
        CThingSwitch* result = 0;
        if (p && p->ConstructFromParams(defIndex, pos, radius, scriptName)) {
            CTCPhysics* physics = p->PhysicsTC;
            physics->ConstructFromParams(CTCParamsPhysicsBase(pos, CRightHandedSet()));
            if (p->DoActivationSequence())
                result = p;
        }
        return result;
    }
};

int main()
{
    C3DVector v = { 1, 2, 3 };
    g_physics.calls = 0;
    CThingSwitch* t = CThingSwitch::Create(5, v, 2.5f, CCharString("sw"));
    if (t != (void*)g_storage || g_args[0] != 5 || g_args[1] != 0 || g_pos != &v || g_radius != 2.5f || g_text[0] != 's' || g_strings != 0) { std::printf("0083c5d0_TEST FAIL\n"); return 1; }
    if (g_physics.calls != 1 || g_physics.pos.Y != 2 || g_physics.rhs.Up.Z != 1 || g_physics.rhs.Forward.Y != 1 || g_physics.rhs.Up.X != 0 || g_paramsLive != 0) { std::printf("0083c5d0_TEST FAIL\n"); return 1; }
    g_activate = false;
    if (CThingSwitch::Create(5, v, 2.5f, CCharString("sw")) != 0 || g_physics.calls != 2) { std::printf("0083c5d0_TEST FAIL\n"); return 1; }
    g_construct = false;
    if (CThingSwitch::Create(5, v, 2.5f, CCharString("sw")) != 0 || g_physics.calls != 2) { std::printf("0083c5d0_TEST FAIL\n"); return 1; }
    g_construct = true; g_activate = true; g_alloc = false;
    if (CThingSwitch::Create(5, v, 2.5f, CCharString("sw")) != 0) { std::printf("0083c5d0_TEST FAIL\n"); return 1; }
    std::printf("0083c5d0_TEST PASS\n");
    return 0;
}