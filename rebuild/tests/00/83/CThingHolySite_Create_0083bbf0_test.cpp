// Behaviour test for CThingHolySite::Create (0x0083BBF0): allocate through Alloc(), construct, place the physics at pos with identity axes, activate.
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
static bool g_playerStart = false;
class CClass;
class CThingHolySite : public CThing {
public:
    CThingHolySite() { PhysicsTC = &g_physics; }
    static CClass* Alloc() { CThingHolySite* p = new (false) CThingHolySite; return (CClass*)p; }
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long player, bool playerStart) { g_args[0] = defIndex; g_args[1] = player; g_pos = &pos; g_playerStart = playerStart; return g_construct; }
    static CThingHolySite* Create(long defIndex, const C3DVector& pos, long player, bool playerStart)
    {
        CThingHolySite* p = (CThingHolySite*)Alloc();
        CThingHolySite* result = 0;
        if (p && p->ConstructFromParams(defIndex, pos, player, playerStart)) {
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
    CThingHolySite* t = CThingHolySite::Create(5, v, 2, true);
    if (t != (void*)g_storage || g_args[0] != 5 || g_args[1] != 2 || g_pos != &v || !g_playerStart) { std::printf("0083bbf0_TEST FAIL\n"); return 1; }
    if (g_physics.calls != 1 || g_physics.pos.Y != 2 || g_physics.rhs.Up.Z != 1 || g_physics.rhs.Forward.Y != 1 || g_physics.rhs.Up.X != 0 || g_paramsLive != 0) { std::printf("0083bbf0_TEST FAIL\n"); return 1; }
    g_activate = false;
    if (CThingHolySite::Create(5, v, 2, true) != 0 || g_physics.calls != 2) { std::printf("0083bbf0_TEST FAIL\n"); return 1; }
    g_construct = false;
    if (CThingHolySite::Create(5, v, 2, true) != 0 || g_physics.calls != 2) { std::printf("0083bbf0_TEST FAIL\n"); return 1; }
    g_construct = true; g_activate = true; g_alloc = false;
    if (CThingHolySite::Create(5, v, 2, true) != 0) { std::printf("0083bbf0_TEST FAIL\n"); return 1; }
    std::printf("0083bbf0_TEST PASS\n");
    return 0;
}