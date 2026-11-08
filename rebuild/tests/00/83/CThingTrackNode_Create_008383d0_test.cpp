// Behaviour test for CThingTrackNode::Create (0x008383D0): allocate, construct from the def index / position / by-value track name,
// add CTCVillageMember then CTCPhysicsStandard, place the physics component at the position with the identity axes, activate;
// null (and no activation) on a refused allocation, a failed construction or a failed activation; the by-value name is destroyed once.
#include <cstdio>
#include <cstring>
#include <stddef.h>
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up, Forward; };
class CIdentityRightHandedSet : public CRightHandedSet {
public:
    CIdentityRightHandedSet() { Up.X = 0.0f; Up.Y = 0.0f; Up.Z = 1.0f; Forward.X = 0.0f; Forward.Y = 1.0f; Forward.Z = 0.0f; }
};
static int g_paramsLive = 0;
class CTCParamsBase { public: virtual ~CTCParamsBase() {} };
class CTCParamsPhysicsBase : public CTCParamsBase {
public:
    C3DVector Position; CRightHandedSet Rhs;
    CTCParamsPhysicsBase(const C3DVector& p, const CRightHandedSet& a) : Position(p), Rhs(a) { ++g_paramsLive; }
    ~CTCParamsPhysicsBase() { --g_paramsLive; }
};
struct CTCPhysicsBase {
    C3DVector pos; CRightHandedSet rhs; int calls;
    virtual void ConstructFromParams(const CTCParamsBase& params)
    { const CTCParamsPhysicsBase& p = static_cast<const CTCParamsPhysicsBase&>(params); pos = p.Position; rhs = p.Rhs; ++calls; }
};
static bool g_alloc = true, g_construct = true, g_activate = true;
static long g_defIndex = -1; static const C3DVector* g_pos = 0;
static double g_storage[64];
static CTCPhysicsBase g_physics;
static int g_strings = 0; static const char* g_text = 0;
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public:
    const char* text;
    CCharString(const char* t) : text(t) { ++g_strings; }
    CCharString(const CCharString& o) : text(o.text) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
class CThing {
public:
    CTCPhysicsBase* PhysicsTC;
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    bool DoActivationSequence() { Log("activate"); return g_activate; }
    void AddTC(const CCharString& name, bool, const CTCParamsBase*) { Log(name.text); }
    static void* operator new(size_t, bool) throw() { return g_alloc ? g_storage : 0; }
    static void operator delete(void*, bool) throw() {}
};
class CThingTrackNode : public CThing {
public:
    CThingTrackNode() { PhysicsTC = &g_physics; }
    bool ConstructFromParams(long defIndex, const C3DVector& pos, CCharString trackName)
    { g_defIndex = defIndex; g_pos = &pos; g_text = trackName.text; Log("construct"); return g_construct; }
    static CThingTrackNode* Create(long defIndex, const C3DVector& position, CCharString trackName)
    {
        CThingTrackNode* p = new (false) CThingTrackNode;
        CThingTrackNode* result = 0;
        if (p && p->ConstructFromParams(defIndex, position, trackName)) {
            p->AddTC("CTCVillageMember", false, 0);
            p->AddTC("CTCPhysicsStandard", false, 0);
            CTCPhysicsBase* physics = p->GetPhysicsTC();
            physics->ConstructFromParams(CTCParamsPhysicsBase(position, CIdentityRightHandedSet()));
            if (p->DoActivationSequence())
                result = p;
        }
        return result;
    }
};

int main()
{
    C3DVector v = { 1, 2, 3 };
    g_physics.calls = 0; g_log[0] = 0;
    CThingTrackNode* t = CThingTrackNode::Create(5, v, CCharString("Track1"));
    if (t != (void*)g_storage || g_defIndex != 5 || g_pos != &v || std::strcmp(g_text, "Track1") != 0 || g_strings != 0) { std::printf("008383d0_TEST FAIL\n"); return 1; }
    if (std::strcmp(g_log, "construct;CTCVillageMember;CTCPhysicsStandard;activate;") != 0) { std::printf("008383d0_TEST FAIL %s\n", g_log); return 1; }
    if (g_physics.calls != 1 || g_physics.pos.X != 1 || g_physics.pos.Y != 2 || g_physics.pos.Z != 3 || g_physics.rhs.Up.Z != 1 || g_physics.rhs.Forward.Y != 1
        || g_physics.rhs.Up.X != 0 || g_physics.rhs.Up.Y != 0 || g_physics.rhs.Forward.X != 0 || g_physics.rhs.Forward.Z != 0 || g_paramsLive != 0) { std::printf("008383d0_TEST FAIL\n"); return 1; }
    g_activate = false; g_log[0] = 0;
    t = CThingTrackNode::Create(5, v, CCharString("Track1"));       // (the name temporary dies with the statement)
    if (t != 0 || g_physics.calls != 2 || g_strings != 0) { std::printf("008383d0_TEST FAIL\n"); return 1; }
    g_construct = false; g_log[0] = 0;
    t = CThingTrackNode::Create(5, v, CCharString("Track1"));
    if (t != 0 || g_physics.calls != 2 || std::strcmp(g_log, "construct;") != 0 || g_strings != 0) { std::printf("008383d0_TEST FAIL\n"); return 1; }
    g_construct = true; g_activate = true; g_alloc = false; g_log[0] = 0;
    t = CThingTrackNode::Create(5, v, CCharString("Track1"));
    if (t != 0 || g_log[0] != 0 || g_strings != 0) { std::printf("008383d0_TEST FAIL\n"); return 1; }
    std::printf("008383d0_TEST PASS\n");
    return 0;
}