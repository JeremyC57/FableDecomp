// Behaviour test for CCreatureAction_SnapDragon::PerformAction (0x008F5060): the mouth dummy position (at the dragon's
// height) gets an obstruction (1.5) whose 0xED driver gets 1.5 and ignores the dragon; the bite explosion is created 2.5
// behind the mouth along the facing, looking at the dragon, and its 0x41 driver gets the dragon as owner.
#include <cstdio>
#include <cstring>
#include <cmath>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CCharString { public: const char* s; CCharString(const char* t) : s(t) {} };
class CThing;
static char g_dummy[32]; static C3DVector g_obsA, g_obsB; static float g_obsR = 0, g_driverValue = 0; static int g_ignored = 0;
static CThing* g_ignoredThing = 0; static char g_defName[48]; static long g_createIndex = -1; static C3DVector g_createPos, g_createLook;
static CThing* g_owner = 0; static CThing* g_obstruction = 0; static CThing* g_bite = 0;
struct CPhysicsBody { C3DVector Facing; C3DVector GetFacing() const { return Facing; } };
struct CTCGraphicAppearance {
    C3DVector Dummy;
    bool GetDummyObject(const CCharString& n, long, unsigned long, C3DVector* p, CRightHandedSet*, float) { std::strcpy(g_dummy, n.s); *p = Dummy; return true; }
};
class CTCBase;
class CThing { public:
    unsigned long Bits; CTCBase* PTCDriver; CPhysicsBody* PhysicsBody; CTCGraphicAppearance* GraphicAppearance; C3DVector Pos;
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    const C3DVector& GetPosition() const { return Pos; }
    bool GetDummyObject(const CCharString& name, C3DVector* position, CRightHandedSet* orientation)
    { return GraphicAppearance->GetDummyObject(name, 0, 0, position, orientation, 1.0f); }
    bool HasTC(long type) const { return (Bits >> (type == 0xed ? 1 : 0)) & 1; }
};
struct CIntelligentPointerThing { CThing* m_pItem; CIntelligentPointerThing(CThing* t) : m_pItem(t) {} };
struct CIntelligentPointerVector { void push_back(const CIntelligentPointerThing& v) { g_ignoredThing = v.m_pItem; ++g_ignored; } };
struct CTCDriverED { CIntelligentPointerVector IgnoredThings; void SetValue(float v) { g_driverValue = v; } };
struct CTCDriver41 { void SetOwner(CThing* o) { g_owner = o; } };
CThing* CreateObstruction(const C3DVector* a, const C3DVector* b, float r) { g_obsA = *a; g_obsB = *b; g_obsR = r; return g_obstruction; }
CThing* CreateThing(long index, const C3DVector* p, const C3DVector* look) { g_createIndex = index; g_createPos = *p; g_createLook = *look; return g_bite; }
class CDefinitionManager { public: long GetDefGlobalIndexFromName(const CCharString& n) { std::strcpy(g_defName, n.s); return 77; } };
class CGameDefinitionManager : public CDefinitionManager { public: static CGameDefinitionManager& __fastcall Get() { static CGameDefinitionManager m; return m; } };
struct CPtr { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
class CCreatureActionBase { public: CPtr PCreature; CThing* GetCreature() const { return PCreature.GetPItem(); } };
class CCreatureAction_SnapDragon : public CCreatureActionBase { public: void PerformAction(); };

void CCreatureAction_SnapDragon::PerformAction()
{
    C3DVector mouth = GetCreature()->GetPosition();
    CRightHandedSet orientation;
    GetCreature()->GetDummyObject("DRAGON_MOUTH_DUMMY", &mouth, &orientation);
    mouth.Z = GetCreature()->GetPosition().Z;
    CThing* obstruction = CreateObstruction(&mouth, &GetCreature()->GetPosition(), 1.5f);
    if (obstruction) {
        CTCDriverED* driver = 0;
        if (obstruction->HasTC(0xed))
            driver = (CTCDriverED*)obstruction->PTCDriver;
        driver->SetValue(1.5f);
        driver->IgnoredThings.push_back(CIntelligentPointerThing(GetCreature()));
    }
    long explosion = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("THING_EXPLOSION_JACK_DRAGON_BITE");
    C3DVector position = mouth - GetCreature()->GetPhysics()->GetFacing() * 2.5f;
    CThing* bite = CreateThing(explosion, &position, &GetCreature()->GetPosition());
    if (bite) {
        CTCDriver41* driver;
        if (bite->HasTC(0x41))
            driver = (CTCDriver41*)bite->PTCDriver;
        driver->SetOwner(GetCreature());
    }
}

static int near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    int ok = 1;
    CTCGraphicAppearance ga; ga.Dummy = C3DVector(10, 20, 99);
    CPhysicsBody body; body.Facing = C3DVector(0, 1, 0);
    CThing dragon; dragon.PhysicsBody = &body; dragon.GraphicAppearance = &ga; dragon.Pos = C3DVector(1, 2, 3);
    CTCDriverED edDriver; CTCDriver41 driver41;
    CThing obstruction; obstruction.Bits = 2; obstruction.PTCDriver = (CTCBase*)&edDriver;
    CThing bite; bite.Bits = 1; bite.PTCDriver = (CTCBase*)&driver41;
    g_obstruction = &obstruction; g_bite = &bite;
    CCreatureAction_SnapDragon a; a.PCreature.m_pItem = &dragon;
    a.PerformAction();
    if (std::strcmp(g_dummy, "DRAGON_MOUTH_DUMMY") != 0) ok = 0;
    if (!near(g_obsA.X, 10) || !near(g_obsA.Y, 20) || !near(g_obsA.Z, 3) || !near(g_obsB.X, 1) || !near(g_obsR, 1.5f)) ok = 0;
    if (!near(g_driverValue, 1.5f) || g_ignored != 1 || g_ignoredThing != &dragon) ok = 0;
    if (std::strcmp(g_defName, "THING_EXPLOSION_JACK_DRAGON_BITE") != 0 || g_createIndex != 77) ok = 0;
    if (!near(g_createPos.X, 10) || !near(g_createPos.Y, 17.5f) || !near(g_createPos.Z, 3) || !near(g_createLook.Z, 3) || g_owner != &dragon) ok = 0;
    // no obstruction and no bite: nothing else happens
    g_obstruction = 0; g_bite = 0; g_owner = 0;
    a.PerformAction();
    if (g_ignored != 1 || g_owner != 0) ok = 0;
    std::printf("008f5060_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}