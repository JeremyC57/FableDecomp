// Behaviour test: CThingAICreature::Create -- new + construct, InitialPos set, construct from the parameters, physics placed with the default facing, activation; null on any failure
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

struct CThingAICreatureInit { int dummy; };
struct CThingAICreature {
    CTCPhysicsBase  physics;
    CTCPhysicsBase* PhysicsTC;
    C3DVector InitialPos;
    long  defIndex, creationType;
    const CThingAICreatureInit* init;
    bool  activated;
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    static CThingAICreature* NewThing()                 // CThing::operator new(size, false) 0x004C7380 + the ctor 0x00831F80
    {
        if (g_allocRefuses)
            return 0;
        CThingAICreature* c = new CThingAICreature;
        c->PhysicsTC = &c->physics;
        c->physics.constructs = 0;
        c->activated = false;
        c->InitialPos.X = c->InitialPos.Y = c->InitialPos.Z = -1.0f;
        return c;
    }
    bool ConstructFromParams(long d, const C3DVector&, long type, const CThingAICreatureInit& i)   // 0x00830360
    {
        defIndex = d; creationType = type; init = &i;
        return !g_failConstruct;
    }
    bool DoActivationSequence() { activated = !g_failActivation; return activated; }
    static CThingAICreature* Create(long defIndex, const C3DVector& position, long creationType, const CThingAICreatureInit& init)
    {
        CThingAICreature* creature = NewThing();
        CThingAICreature* result = 0;
        if (creature) {
            creature->InitialPos = position;
            if (creature->ConstructFromParams(defIndex, position, creationType, init)) {
                creature->GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet()));
                if (creature->DoActivationSequence())
                    result = creature;
            }
        }
        return result;
    }
};

int main()
{
    C3DVector pos = { 1.0f, 2.0f, 3.0f };
    CThingAICreatureInit init = { 0 };
    CThingAICreature* c = CThingAICreature::Create(55, pos, 5, init);
    if (!c || c->InitialPos.X != 1.0f || c->InitialPos.Z != 3.0f || c->defIndex != 55 || c->creationType != 5 || c->init != &init
        || c->physics.constructs != 1 || c->physics.placedAt.Y != 2.0f || c->physics.upZ != 1.0f || !c->activated || g_paramsAlive != 0)
        { std::printf("00833800_TEST FAIL\n"); return 1; }
    delete c;
    g_failConstruct = true;
    if (CThingAICreature::Create(55, pos, 5, init) != 0)
        { std::printf("00833800_TEST FAIL\n"); return 1; }
    g_failConstruct = false;
    g_failActivation = true;
    if (CThingAICreature::Create(55, pos, 5, init) != 0)
        { std::printf("00833800_TEST FAIL\n"); return 1; }
    g_failActivation = false;
    g_allocRefuses = true;
    if (CThingAICreature::Create(55, pos, 5, init) != 0)
        { std::printf("00833800_TEST FAIL\n"); return 1; }
    std::printf("00833800_TEST PASS\n");
    return 0;
}