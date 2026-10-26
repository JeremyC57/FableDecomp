// Behaviour test for CThingVillage::Create (0x0083B7F0): allocate, construct, place the physics at pos with identity axes, activate.
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
class CThingVillage : public CThing {
public:
    CThingVillage() { PhysicsTC = &g_physics; }
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long player) { g_args[0] = defIndex; g_args[1] = player; g_pos = &pos; return g_construct; }
    static CThingVillage* Create(long defIndex, const C3DVector& pos, long player)
    {
        CThingVillage* p = new (false) CThingVillage;
        CThingVillage* result = 0;
        if (p && p->ConstructFromParams(defIndex, pos, player)) {
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
    CThingVillage* t = CThingVillage::Create(5, v, 2);
    if (t != (void*)g_storage || g_args[0] != 5 || g_args[1] != 2 || g_pos != &v) { std::printf("0083b7f0_TEST FAIL\n"); return 1; }
    if (g_physics.calls != 1 || g_physics.pos.Y != 2 || g_physics.rhs.Up.Z != 1 || g_physics.rhs.Forward.Y != 1 || g_physics.rhs.Up.X != 0 || g_paramsLive != 0) { std::printf("0083b7f0_TEST FAIL\n"); return 1; }
    g_activate = false;
    if (CThingVillage::Create(5, v, 2) != 0 || g_physics.calls != 2) { std::printf("0083b7f0_TEST FAIL\n"); return 1; }
    g_construct = false;
    if (CThingVillage::Create(5, v, 2) != 0 || g_physics.calls != 2) { std::printf("0083b7f0_TEST FAIL\n"); return 1; }
    g_construct = true; g_activate = true; g_alloc = false;
    if (CThingVillage::Create(5, v, 2) != 0) { std::printf("0083b7f0_TEST FAIL\n"); return 1; }
    std::printf("0083b7f0_TEST PASS\n");
    return 0;
}