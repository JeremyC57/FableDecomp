// 009175e0 behaviour test: CAIStateGroup_Clean::Update (r5.cpp body: implicit CCharString literals,
// named `unsigned long r = Random() % 4;` passes count), host stand-ins. C++03, g++ -w.
#include <stdio.h>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static void logs(const char* s) { g_log += s; g_log += ";"; }

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
    void RotateAboutZ(float angle, bool flag) { (void)flag; float t = X; X = Y; Y = t + angle; }
};
static float g_radius = 10.0f;
struct CTCPhysicsBase {
    void GetBoundingInfo(C3DVector* centre, float* radius) { *centre = C3DVector(1.0f, 2.0f, 3.0f); *radius = g_radius; }
};
static int g_insideLimit = 100, g_insideCalls = 0;
struct CThingBuilding {
    CTCPhysicsBase* Physics;
    unsigned char m_flags91;
    bool IsPointInside(const C3DVector& point) { (void)point; return ++g_insideCalls <= g_insideLimit; }
};
struct CThing { int Id; };
struct CCharString {
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
};
struct CSoundPair {
    long m_first, m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
static int g_actionIds = 100;
struct CCreatureActionBase {
    std::string Anim; long Id;
    long GetActionID() { return Id; }
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    CCreatureAction_PlayAnimation(CThing* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d)
    { (void)owner; (void)sound; (void)useAnimationMovement; (void)usePhysics; (void)cameraUpdate; (void)c; (void)d;
      Anim = animationName.S; Id = ++g_actionIds; }
};
template <class T> static T* tmpaddr(const T& t) { return const_cast<T*>(&t); }
struct CAIBrain { CThing* PCreature; };
static unsigned long GFROR13(unsigned long v) { v &= 0xFFFFFFFFUL; return ((v >> 13) | (v << 19)) & 0xFFFFFFFFUL; }
static float GFFloatRandom(float range, unsigned long* seed) { (void)seed; return range * 0.5f; }

static int g_moveFails = 0, g_moveFourCount = 0, g_animFailAt = -1, g_anims = 0;
struct CAIStateGroup_Clean {
    CAIBrain* PBrain;
    bool Cleaning, StartingToClean;
    long CleaningActionID;
    float FloorHeight;
    CThingBuilding* Building;
    unsigned long Seed;
    bool Terminated;
    CThingBuilding* GetBuilding() { return Building; }
    CThing* GetCreature() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed() { return &Seed; }
    void Terminate() { Terminated = true; logs("T"); }
    long AIAction_DoNothing() { logs("idle"); return 2; }
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
        bool a, bool b, bool c, CThing* leader, bool d, float e)
    { (void)repeat; (void)pos; (void)radius; (void)reached; (void)movementType; (void)a; (void)b; (void)c; (void)leader; (void)d; (void)e;
      if (g_moveFourCount > 0) { --g_moveFourCount; logs("wait"); return 4; }
      logs("move"); return g_moveFails ? 3 : 2; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait)
    { (void)wait; logs(action->Anim.c_str()); return g_anims++ == g_animFailAt ? 3 : 2; }
    unsigned long Random()
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = (*seed * 0x24A1U + 0x24DFU) & 0xFFFFFFFFUL;
        return *seed = GFROR13(*seed);
    }
    void Update();
};
static bool IsAlive(const CThingBuilding* thing) { return thing && !(thing->m_flags91 & 1); }

void CAIStateGroup_Clean::Update()
{
    Cleaning = false;
    unsigned long r = Random() % 4;
    long passes = r + 5;
    if (!IsAlive(GetBuilding())) {
        Terminate();
        return;
    }
    C3DVector centre;
    float radius;
    CTCPhysicsBase* physics = GetBuilding()->Physics;
    physics->GetBoundingInfo(&centre, &radius);
    centre.Z = FloorHeight;
    float range = 4.0f < radius ? 4.0f : radius;
    C3DVector offset(GFFloatRandom(range, GetPWorldSeed()) + 0.0001f, 0.0f, 0.0f);
    offset.RotateAboutZ(GFFloatRandom(1.0f, GetPWorldSeed()), false);
    C3DVector position = centre + offset;
    for (long i = 0; i < passes; ++i) {
        if (!GetBuilding())
            break;
        float step = 1.4f < radius ? 1.4f : radius;
        offset = C3DVector(GFFloatRandom(step, GetPWorldSeed()) + 0.0001f, 0.0f, 0.0f);
        offset.RotateAboutZ(GFFloatRandom(1.0f, GetPWorldSeed()), false);
        position += offset;
        if (!GetBuilding()->IsPointInside(position)) {
            AIAction_DoNothing();
            Terminate();
            return;
        }
        bool reached;
        long result;
        do {
            if (!GetBuilding()) {
                Terminate();
                return;
            }
            result = AIAction_MoveToPosProximity(2, position, 0.5f, &reached, 3, true, false, true, 0, true, 0.0f);
        } while (result == 4);
        if (result != 2)
            break;
        CleaningActionID = -1;
        StartingToClean = true;
        result = AIAction_PerformActionAndWait(tmpaddr(CCreatureAction_PlayAnimation(GetCreature(), "ST_WASH_FLOOR_INTO",
            CSoundPair(), false, true, false, 0, 0)), false);
        if (result != 2)
            break;
        StartingToClean = false;
        Cleaning = true;
        long loops = (Random() & 1) + 1;
        for (long j = 0; j < loops; ++j) {
            CCreatureAction_PlayAnimation wash(GetCreature(), "ST_WASH_FLOOR_LOOP", CSoundPair(), false, true, false, 0, 0);
            CleaningActionID = wash.GetActionID();
            if (AIAction_PerformActionAndWait(&wash, false) != 2) {
                Terminate();
                return;
            }
        }
        Cleaning = false;
        result = AIAction_PerformActionAndWait(tmpaddr(CCreatureAction_PlayAnimation(GetCreature(), "ST_WASH_FLOOR_OUTOF",
            CSoundPair(), false, true, false, 0, 0)), false);
        if (result != 2)
            break;
    }
    Terminate();
}

static int count(const std::string& s, const char* sub)
{
    int n = 0; size_t p = 0; std::string t(sub);
    while ((p = s.find(t, p)) != std::string::npos) { ++n; p += t.size(); }
    return n;
}
static void reset() { g_log = ""; g_insideLimit = 100; g_insideCalls = 0; g_moveFails = 0; g_moveFourCount = 0; g_animFailAt = -1; g_anims = 0; }

int main()
{
    CThing creature = { 7 };
    CAIBrain brain = { &creature };
    CTCPhysicsBase phys;
    CThingBuilding building = { &phys, 0 };
    CAIStateGroup_Clean g;
    g.PBrain = &brain; g.FloorHeight = 0.0f; g.Building = &building;

    // full run: passes = (first Random) % 4 + 5, each pass does INTO, 1..2 LOOP, OUTOF
    reset(); g.Seed = 1; g.Terminated = false;
    unsigned long s = (1UL * 0x24A1U + 0x24DFU) & 0xFFFFFFFFUL; s = GFROR13(s);
    long expect = (long)(s % 4) + 5;
    g.Update();
    CHECK(g.Terminated);
    CHECK(count(g_log, "ST_WASH_FLOOR_INTO") == expect);
    CHECK(count(g_log, "ST_WASH_FLOOR_OUTOF") == expect);
    CHECK(count(g_log, "ST_WASH_FLOOR_LOOP") >= expect && count(g_log, "ST_WASH_FLOOR_LOOP") <= 2 * expect);
    CHECK(count(g_log, "T;") == 1);
    CHECK(!g.Cleaning && !g.StartingToClean);
    CHECK(g.CleaningActionID > 100);

    // dead building: terminate at once
    reset(); building.m_flags91 = 1; g.Terminated = false; g.Seed = 5;
    g.Update();
    CHECK(g_log == "T;");
    building.m_flags91 = 0;

    // point outside on the 2nd pass: idle then terminate
    reset(); g_insideLimit = 1; g.Terminated = false; g.Seed = 9;
    g.Update();
    CHECK(count(g_log, "ST_WASH_FLOOR_INTO") == 1);
    CHECK(g_log.find("idle;T;") != std::string::npos);
    CHECK(count(g_log, "T;") == 1);

    // move retries (result 4) then failure -> break -> terminate
    reset(); g_moveFourCount = 2; g_moveFails = 1; g.Terminated = false; g.Seed = 3;
    g.Update();
    CHECK(g_log == "wait;wait;move;T;");

    // INTO fails -> break; StartingToClean stays set
    reset(); g_animFailAt = 0; g.Terminated = false; g.Seed = 3;
    g.Update();
    CHECK(g_log == "move;ST_WASH_FLOOR_INTO;T;");
    CHECK(g.StartingToClean && g.CleaningActionID == -1);

    // a LOOP failure terminates from inside the inner loop (Cleaning stays true)
    reset(); g_animFailAt = 1; g.Terminated = false; g.Seed = 3;
    g.Update();
    CHECK(g_log == "move;ST_WASH_FLOOR_INTO;ST_WASH_FLOOR_LOOP;T;");
    CHECK(g.Cleaning);

    if (!g_fail) printf("009175e0_TEST PASS\n");
    return g_fail;
}