// Behaviour test for 0x0083D460: CThingMarker::Create (host model: the row's body over a stub thing hierarchy; the allocator, the
// game-object construction, the physics component and the activation are scripted stubs)
#include <cstdio>
#include <cstddef>

class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet() { Up.X = 0.0f; Up.Y = 0.0f; Up.Z = 1.0f; Forward.X = 0.0f; Forward.Y = 1.0f; Forward.Z = 0.0f; }
};
class CCharString { public: const char* Text; };
class CTCParamsPhysicsBase { public:
    C3DVector Position; CRightHandedSet Rhs;
    CTCParamsPhysicsBase(const C3DVector& p, const CRightHandedSet& r) : Position(p), Rhs(r) { }
};
class CTCPhysicsBase { public:
    int Calls; C3DVector Position; CRightHandedSet Rhs;
    void ConstructFromParams(const CTCParamsPhysicsBase& params) { ++Calls; Position = params.Position; Rhs = params.Rhs; }
};

static bool g_alloc_ok = true, g_construct_ok = true, g_activate_ok = true;
static CTCPhysicsBase g_physics;
static long g_def = 0; static const char* g_name = 0;
static unsigned char g_storage[256];

class CThingMarker {
public:
    unsigned long Type; float MaxHealth, Health; bool Activated;
    CThingMarker() : Type(11), MaxHealth(0), Health(0), Activated(false) { }
    static void* operator new(size_t, bool) throw() { return g_alloc_ok ? (void*)g_storage : 0; }
    static void operator delete(void*, bool) throw() { }
    CTCPhysicsBase* GetPhysicsTC() { return &g_physics; }
    bool DoActivationSequence() { Activated = true; return g_activate_ok; }
    static CThingMarker* Create(long defIndex, const C3DVector& position, const CCharString& scriptName);
protected:
    bool ConstructFromParams(long defIndex, const C3DVector&, const CCharString& scriptName)
    {
        g_def = defIndex; g_name = scriptName.Text;
        if (!g_construct_ok)
            return false;
        Health = 1.0f;
        MaxHealth = 1.0f;
        return true;
    }
};

// the row's body
CThingMarker* CThingMarker::Create(long defIndex, const C3DVector& position, const CCharString& scriptName)
{
    CThingMarker* p_marker = new (false) CThingMarker;
    CThingMarker* p_result = 0;
    if (p_marker && p_marker->ConstructFromParams(defIndex, position, scriptName)) {
        p_marker->GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet()));
        if (p_marker->DoActivationSequence())
            p_result = p_marker;
    }
    return p_result;
}

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }

int main()
{
    C3DVector pos = { 1.0f, 2.0f, 3.0f };
    CCharString name = { "MARKER_1" };
    g_physics.Calls = 0;
    CThingMarker* m = CThingMarker::Create(42, pos, name);
    Check(m != 0 && m->Type == 11 && m->Health == 1.0f && m->MaxHealth == 1.0f && m->Activated);
    Check(g_def == 42 && g_name == name.Text && g_physics.Calls == 1);
    Check(g_physics.Position.X == 1.0f && g_physics.Position.Z == 3.0f && g_physics.Rhs.Up.Z == 1.0f && g_physics.Rhs.Forward.Y == 1.0f);
    g_activate_ok = false;                       // activation fails: null, after the physics setup
    Check(CThingMarker::Create(1, pos, name) == 0 && g_physics.Calls == 2);
    g_activate_ok = true; g_construct_ok = false;   // construction fails: null, no physics setup
    Check(CThingMarker::Create(1, pos, name) == 0 && g_physics.Calls == 2);
    g_construct_ok = true; g_alloc_ok = false;      // allocation fails: null
    Check(CThingMarker::Create(1, pos, name) == 0 && g_physics.Calls == 2);
    if (g_fail) { std::printf("0083d460_TEST FAIL\n"); return 1; }
    std::printf("0083d460_TEST PASS\n");
    return 0;
}