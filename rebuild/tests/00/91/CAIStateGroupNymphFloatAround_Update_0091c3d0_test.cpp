// 0091c3d0 behaviour test: CAIStateGroup_NymphFloatAround::Update (match.cpp body, host-modelled engine stand-ins)
#include <stdio.h>
#include <string.h>
#include <math.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

static unsigned int g_InvSqrtMantissaTable[128];
static void BuildTable()
{
    for (int i = 0; i < 128; ++i) {
        unsigned int bits = ((unsigned int)(126 + (i >> 6)) << 23) | ((unsigned int)(i & 63) << 17) | (1u << 16);
        float f; memcpy(&f, &bits, 4);
        int e = (int)((0x17c - ((bits >> 23) & 0xff)) >> 1);
        double m = (1.0 / sqrt((double)f)) / ldexp(1.0, e - 127) - 1.0;
        int t = (int)(m * 256.0 + 0.5); if (t < 0) t = 0; if (t > 255) t = 255;
        g_InvSqrtMantissaTable[i] = (unsigned int)t;
    }
}
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared2D() const { return X * X + Y * Y; }
    void Normalise()
    {
        float x = X, y = Y, z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned int bits; memcpy(&bits, &lengthSquared, 4);
        unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r; memcpy(&r, &estimate, 4);
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r; Y *= r; Z *= r;
    }
};
class CRightHandedSet { public: C3DVector Up, Forward; CRightHandedSet(const C3DVector& u, const C3DVector& f) : Up(u), Forward(f) {} };

static int g_setOrientCalls = 0; static C3DVector g_lastForward, g_lastUp;
struct CPhysicsBody { void SetOrientation(const CRightHandedSet& o) { ++g_setOrientCalls; g_lastForward = o.Forward; g_lastUp = o.Up; } };
class CThing { public: C3DVector Pos; CPhysicsBody* PPhysics; const C3DVector& GetPosition() const { return Pos; } };
class CThingCreatureBase : public CThing { public: };
struct CIntelligentPointerCreature {
    CThing* m_pItem;
    CIntelligentPointerCreature(CThingCreatureBase* item) : m_pItem(item) {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(const CThing* item) { m_pItem = (CThing*)item; }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
class CTCNymph { public:
    bool FlyingAround, CanAttack, ReadyToAttack, HasBeenInLimbo;
    CIntelligentPointerCreature PTarget;
    int startCalls, stopMode, readyAfter, checks; CThingCreatureBase* targetOnReady;
    CTCNymph() : FlyingAround(false), CanAttack(true), ReadyToAttack(true), HasBeenInLimbo(false), PTarget(0), startCalls(0), stopMode(0), readyAfter(0), checks(0), targetOnReady(0) {}
    void StartFlyingAround(long) { ++startCalls; FlyingAround = true; }
    void CheckIfCanAttack(long frame) { CHECK(frame == 77); if (++checks >= readyAfter) { ReadyToAttack = true; PTarget.SetPItem(targetOnReady); } }
    void StopFlyingAround(long mode) { stopMode = (int)mode; FlyingAround = false; }
};
class CCreatureActionBase { public: };
class CCreatureAction_NymphTeleportIntoLimbo : public CCreatureActionBase { public: CCreatureAction_NymphTeleportIntoLimbo(CThingCreatureBase*) {} };
class CWorld { public: static long __fastcall GetFrame() { return 77; } };
static CWorld g_world;
class CAIBrain { public: int yields; CThingCreatureBase* PCreature; void Yield() { ++yields; } };
class CAIStateGroupBase { public:
    CAIBrain* PBrain; CTCNymph* nymphComponent; bool stopping; int terminated; long actionResult; int actions;
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld() { return &g_world; }
    bool IsStopping() { return stopping; }
    void Terminate() { ++terminated; }
    long AIAction_PerformActionAndWait(CCreatureActionBase*, bool wait) { CHECK(!wait); ++actions; return actionResult; }
};
struct CThingWithIface { };
struct CAIStateGroup_NymphFloatAround : CAIStateGroupBase { void Update(); };

// ---- match.cpp body (GetInterface modelled as a direct component fetch; the action temporary named) ----
void CAIStateGroup_NymphFloatAround::Update()
{
    CTCNymph* nymph = nymphComponent;
    nymph->CanAttack = false;
    nymph->ReadyToAttack = false;
    nymph->PTarget = CIntelligentPointerCreature(0);
    if (!nymph->FlyingAround) {
        if (nymph->HasBeenInLimbo) {
            CCreatureAction_NymphTeleportIntoLimbo action(GetCreature());
            if (AIAction_PerformActionAndWait(&action, false) != 2) {
                Terminate();
                return;
            }
        } else
            nymph->StartFlyingAround(0);
    }
    long frame = GetPWorld()->GetFrame();
    while (!nymph->ReadyToAttack) {
        nymph->CheckIfCanAttack(frame);
        if (IsStopping() || !nymph->FlyingAround) {
            Terminate();
            return;
        }
        PBrain->Yield();
    }
    nymph->StopFlyingAround(-1);
    if (nymph->PTarget.GetPItem()) {
        const CThing* creature = GetCreature();
        const CThing* target = nymph->PTarget.GetPItem();
        const C3DVector& from = creature->GetPosition();
        const C3DVector& to = target->GetPosition();
        C3DVector direction(to.X - from.X, to.Y - from.Y, 0.0f);
        if (direction.GetLengthSquared2D() > 1.0f) {
            direction.Normalise();
            GetCreature()->PPhysics->SetOrientation(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), direction));
        }
    }
    Terminate();
}

int main()
{
    BuildTable();
    CPhysicsBody body;
    CThingCreatureBase me; me.Pos = C3DVector(1, 1, 5); me.PPhysics = &body;
    CThingCreatureBase far_; far_.Pos = C3DVector(4, 5, 9); far_.PPhysics = 0;
    CThingCreatureBase near_; near_.Pos = C3DVector(1.5f, 1.5f, 0); near_.PPhysics = 0;
    CAIBrain brain = { 0, &me };
    CTCNymph n; n.readyAfter = 3; n.targetOnReady = &far_; n.PTarget.SetPItem(&near_);
    CAIStateGroup_NymphFloatAround g; g.PBrain = &brain; g.nymphComponent = &n; g.stopping = false; g.terminated = 0; g.actionResult = 2; g.actions = 0;
    g.Update();
    CHECK(!n.CanAttack && n.ReadyToAttack && n.startCalls == 1 && n.checks == 3 && brain.yields == 3 && n.stopMode == -1);
    CHECK(g.terminated == 1 && g_setOrientCalls == 1);
    CHECK(fabs(g_lastForward.X - 0.6f) < 0.01f && fabs(g_lastForward.Y - 0.8f) < 0.01f && g_lastForward.Z == 0.0f);
    CHECK(g_lastUp.X == 0.0f && g_lastUp.Y == 0.0f && g_lastUp.Z == 1.0f);
    // target within 1 unit in the plane: no orientation change
    CTCNymph n2; n2.readyAfter = 1; n2.targetOnReady = &near_; n2.FlyingAround = true; g.nymphComponent = &n2; g.terminated = 0;
    g.Update();
    CHECK(n2.startCalls == 0 && g_setOrientCalls == 1 && g.terminated == 1);
    // back from limbo, the teleport action fails: terminate before the loop
    CTCNymph n3; n3.HasBeenInLimbo = true; n3.PTarget.SetPItem(&far_); g.nymphComponent = &n3; g.terminated = 0; g.actionResult = 1;
    g.Update();
    CHECK(g.actions == 1 && g.terminated == 1 && n3.checks == 0 && n3.PTarget.GetPItem() == 0 && n3.stopMode == 0);
    // stopping inside the loop
    CTCNymph n4; n4.readyAfter = 5; g.nymphComponent = &n4; g.terminated = 0; g.stopping = true;
    g.Update();
    CHECK(n4.checks == 1 && g.terminated == 1 && n4.stopMode == 0 && n4.startCalls == 1);
    if (!g_fail) printf("0091c3d0_TEST PASS\n");
    return g_fail;
}