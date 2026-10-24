// Behaviour test for CThingPhysicalSwitch::Create (0x0083DBE0): allocate through Alloc(), untriggered,, construct, place the physics at pos with identity axes, activate.
#include <cstdio>
#include <stddef.h>
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet {
    C3DVector Up, Forward;
    CRightHandedSet() { Up.X = 0; Up.Y = 0; Up.Z = 1; Forward.X = 0; Forward.Y = 1; Forward.Z = 0; }
};
static int g_paramsLive = 0;
struct CTCParamsPhysicsBase {
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
struct CCharString { const char* text; };
class CThingPhysical;
static unsigned long g_flags = 99; static const CCharString* g_name = 0; static bool g_last = false; static void* g_creators = (void*)1;
class CThingGameObject : public CThing {
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long player, unsigned long flags, const CCharString& scriptName,
                             CThingPhysical* creator, CThingPhysical* originalCreator, bool a8)
    { g_args[0] = defIndex; g_args[1] = player; g_pos = &pos; g_flags = flags; g_name = &scriptName; g_creators = (void*)((size_t)creator | (size_t)originalCreator); g_last = a8; return g_construct; }
};
class CClass;
class CThingPhysicalSwitch : public CThingGameObject {
public:
    bool Triggered;
    CThingPhysicalSwitch() : Triggered(true) { PhysicsTC = &g_physics; }
    static CClass* Alloc() { CThingPhysicalSwitch* p = new (false) CThingPhysicalSwitch; return (CClass*)p; }
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long player, const CCharString& scriptName)
    {
        Triggered = false;
        return CThingGameObject::ConstructFromParams(defIndex, pos, player, 1, scriptName, 0, 0, true);
    }
    static CThingPhysicalSwitch* Create(long defIndex, const C3DVector& pos, long player, const CCharString& scriptName)
    {
        CThingPhysicalSwitch* p = (CThingPhysicalSwitch*)Alloc();
        CThingPhysicalSwitch* result = 0;
        if (p && p->ConstructFromParams(defIndex, pos, player, scriptName)) {
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
    CCharString nm = { "switch" };
    g_physics.calls = 0;
    CThingPhysicalSwitch* t = CThingPhysicalSwitch::Create(5, v, 2, nm);
    if (t != (void*)g_storage || g_args[0] != 5 || g_args[1] != 2 || g_pos != &v || g_flags != 1 || g_name != &nm || g_creators != 0 || !g_last || t->Triggered) { std::printf("0083dbe0_TEST FAIL\n"); return 1; }
    if (g_physics.calls != 1 || g_physics.pos.Y != 2 || g_physics.rhs.Up.Z != 1 || g_physics.rhs.Forward.Y != 1 || g_physics.rhs.Up.X != 0 || g_paramsLive != 0) { std::printf("0083dbe0_TEST FAIL\n"); return 1; }
    g_activate = false;
    if (CThingPhysicalSwitch::Create(5, v, 2, nm) != 0 || g_physics.calls != 2) { std::printf("0083dbe0_TEST FAIL\n"); return 1; }
    g_construct = false;
    if (CThingPhysicalSwitch::Create(5, v, 2, nm) != 0 || g_physics.calls != 2) { std::printf("0083dbe0_TEST FAIL\n"); return 1; }
    g_construct = true; g_activate = true; g_alloc = false;
    if (CThingPhysicalSwitch::Create(5, v, 2, nm) != 0) { std::printf("0083dbe0_TEST FAIL\n"); return 1; }
    std::printf("0083dbe0_TEST PASS\n");
    return 0;
}