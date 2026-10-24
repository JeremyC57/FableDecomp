// Behaviour test: CThingBuilding::Create -- alloc, construct from the parameters, physics placed with the default facing, activation; null on any failure
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() { Up.X = 0.0f; Up.Y = 0.0f; Up.Z = 1.0f; Forward.X = 0.0f; Forward.Y = 1.0f; Forward.Z = 0.0f; }
};
static int g_paramsAlive = 0;
struct CTCParamsPhysicsBase {
    C3DVector       Position;
    CRightHandedSet Rhs;
    CTCParamsPhysicsBase(const C3DVector& p, const CRightHandedSet& r) : Position(p), Rhs(r) { ++g_paramsAlive; }
    ~CTCParamsPhysicsBase() { --g_paramsAlive; }
};
struct CTCPhysicsBase {
    C3DVector placedAt;
    float     upZ, forwardY;
    int       constructs;
    void ConstructFromParams(const CTCParamsPhysicsBase& params)
    {
        placedAt = params.Position;
        upZ = params.Rhs.Up.Z;
        forwardY = params.Rhs.Forward.Y;
        ++constructs;
    }
};
struct CCharString { const char* text; };
static bool g_failConstruct = false, g_failActivation = false, g_allocRefuses = false;

struct CThingBuilding {
    CTCPhysicsBase  physics;
    CTCPhysicsBase* PhysicsTC;
    long  defIndex, a3, a4;
    const CCharString* name;
    bool  activated;
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    static CThingBuilding* Alloc()                      // 0x0052AC10 (null when the thing manager refuses)
    {
        if (g_allocRefuses)
            return 0;
        CThingBuilding* b = new CThingBuilding;
        b->PhysicsTC = &b->physics;
        b->physics.constructs = 0;
        b->activated = false;
        return b;
    }
    bool BaseConstructFromParams(long d, const C3DVector*, long x3, unsigned long x4, const CCharString& n, void*, void*, bool)
    {                                                   // CThingGameObject::ConstructFromParams 0x008388D0
        defIndex = d; a3 = x3; a4 = (long)x4; name = &n;
        return !g_failConstruct;
    }
    bool ConstructFromParams(long d, const C3DVector& p, long x3, const CCharString& n)
    {
        return BaseConstructFromParams(d, &p, x3, 1, n, 0, 0, true);
    }
    bool DoActivationSequence() { activated = !g_failActivation; return activated; }   // 0x004C9CA0
    static CThingBuilding* Create(long defIndex, const C3DVector& position, long a3, const CCharString& scriptName)
    {
        CThingBuilding* building = Alloc();
        CThingBuilding* result = 0;
        if (building && building->ConstructFromParams(defIndex, position, a3, scriptName)) {
            building->GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet()));
            if (building->DoActivationSequence())
                result = building;
        }
        return result;
    }
};

int main()
{
    C3DVector pos = { 10.0f, 20.0f, 3.0f };
    CCharString name = { "BowerstoneHouse" };
    CThingBuilding* b = CThingBuilding::Create(17, pos, 4, name);
    if (!b || b->defIndex != 17 || b->a3 != 4 || b->a4 != 1 || b->name != &name || b->physics.constructs != 1
        || b->physics.placedAt.Y != 20.0f || b->physics.upZ != 1.0f || b->physics.forwardY != 1.0f || !b->activated || g_paramsAlive != 0)
        { std::printf("0082d920_TEST FAIL\n"); return 1; }
    delete b;
    g_failConstruct = true;
    if (CThingBuilding::Create(17, pos, 4, name) != 0)
        { std::printf("0082d920_TEST FAIL\n"); return 1; }
    g_failConstruct = false;
    g_failActivation = true;
    if (CThingBuilding::Create(17, pos, 4, name) != 0)
        { std::printf("0082d920_TEST FAIL\n"); return 1; }
    g_failActivation = false;
    g_allocRefuses = true;
    if (CThingBuilding::Create(17, pos, 4, name) != 0)
        { std::printf("0082d920_TEST FAIL\n"); return 1; }
    std::printf("0082d920_TEST PASS\n");
    return 0;
}