// Behaviour test for 0097f690: src/0097f690.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAIStateGroupBase::AIAction_MoveToPreCalcRoute(const CThing& routeThing, bool& speed, EMovementType anim, const CAIFilterBase& condition, bool extra)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroupBase::AIAction_MoveToPreCalcRoute(const CThing& routeThing, bool& speed, EMovementType anim, const CAIFilterBase& condition, bool extra) -- walk to the route thing, then (on arrival) along its precalculated route to the destination, polling the condition each step
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097F690, length 226 bytes (`ret 14h`).
// Callees: CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (mode 2, C3DVector by
// value, then 1,0,1,0), the condition's virtual slot 0, CAIStateGroupBase::SetUpPreCalcNav @ 0x0097F5B0.
// Shape: two `do { r = MoveToPos(...); if (!condition) return 3; } while (r == 4);` loops; the first returns any
// result other than 2; the POD vector argument is copied into the argument area dword by dword.
// Signature: Ego_r.pdb `?AIAction_MoveToPreCalcRoute@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCThing@@AA_NW4EMovementType@@
// ABVCAIFilterBase@1@_N@Z` and AIAction_MoveToPos(EAIActionRepeat, C3DVector, bool&, EMovementType, bool, bool, const CThing*,
// bool, float). These parameter types (a reference, an enum, a bool) give retail's register assignment (speed ebx,
// anim edi, extra ebp) and the loop result's spill into the dead condition slot. All-`long` parameters permute them.
struct C3DVector
{
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // user copy ctor: the by-value argument is built in place
};

struct CThing
{
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

enum EMovementType { MOVEMENT_DEFAULT };
enum { AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };

struct CAIStateGroupBase
{
    struct CAIFilterBase                            // 4 bytes (Ego_r.pdb): the caller's "keep going" test, virtual slot 0
    {
        virtual bool IsStillValid() const;
    };
    virtual void v00();
    unsigned char _pad04[0x10];
    long AIAction_MoveToPos(long mode, C3DVector pos, bool& speed, EMovementType anim, bool extra,
                            bool b1, const CThing* target, bool b3, float f);          // 0x0097EC00
    C3DVector SetUpPreCalcNav(const CThing& routeThing);                                // 0x0097F5B0
    long AIAction_MoveToPreCalcRoute(const CThing& routeThing, bool& speed, EMovementType anim,
                                     const CAIFilterBase& condition, bool extra);
};

long CAIStateGroupBase::AIAction_MoveToPreCalcRoute(const CThing& routeThing, bool& speed, EMovementType anim,
                                                    const CAIFilterBase& condition, bool extra)
{
    long result;
    do
    {
        result = AIAction_MoveToPos(2, routeThing.GetPosition(), speed, anim, extra, true, 0, true, 0.0f);
        if (!condition.IsStillValid())
            return AI_ACTION_ABORTED;
    } while (result == AI_ACTION_RUNNING);
    if (result != AI_ACTION_DONE)
        return result;
    C3DVector destination = SetUpPreCalcNav(routeThing);
    do
    {
        result = AIAction_MoveToPos(2, destination, speed, anim, extra, true, 0, true, 0.0f);
        if (!condition.IsStillValid())
            return AI_ACTION_ABORTED;
    } while (result == AI_ACTION_RUNNING);
    return result;
}

// ---- stubs and driver ----
static int g_moves = 0, g_proxMoves = 0, g_navs = 0, g_checks = 0;
static int g_script[16]; static int g_scriptPos = 0;
static int g_validUntil = 1000;
struct V3 { float X, Y, Z; };
static V3 g_posRaw = { 1.0f, 2.0f, 3.0f };
static V3 g_destRaw = { 7.0f, 8.0f, 9.0f };
static bool* g_lastSpeed = 0; static long g_lastAnim = -1; static bool g_lastExtra = false;
static float g_lastProx = 0.0f; static V3 g_lastPos = { 0, 0, 0 };
const C3DVector& CThing::GetPosition() const { return *(const C3DVector*)&g_posRaw; }
bool CAIStateGroupBase::CAIFilterBase::IsStillValid() const { return ++g_checks <= g_validUntil; }
void CAIStateGroupBase::v00() { }
long CAIStateGroupBase::AIAction_MoveToPos(long mode, C3DVector pos, bool& speed, EMovementType anim, bool extra,
                                           bool b1, const CThing* target, bool b3, float f)
{
    ++g_moves; CHECK(mode == 2 && b1 && target == 0 && b3 && f == 0.0f);
    g_lastSpeed = &speed; g_lastAnim = anim; g_lastExtra = extra; g_lastPos.X = pos.X; g_lastPos.Y = pos.Y; g_lastPos.Z = pos.Z;
    return g_script[g_scriptPos++];
}
C3DVector CAIStateGroupBase::SetUpPreCalcNav(const CThing& routeThing) { ++g_navs; return *(const C3DVector*)&g_destRaw; }
#define CALL(r) s.AIAction_MoveToPreCalcRoute(route, flag, (EMovementType)5, cond, true)
#define SECOND g_moves
int main()
{
    CAIStateGroupBase s; CThing route; CAIStateGroupBase::CAIFilterBase cond; bool flag = true;
    // first leg running twice then done; second leg running once then done
    int sc[] = { 4, 4, 2, 4, 2 }; for (int i = 0; i < 5; ++i) g_script[i] = sc[i];
    g_scriptPos = 0; g_checks = 0; g_validUntil = 1000;
    long r = CALL(0);
    CHECK(r == 2 && g_navs == 1 && g_scriptPos == 5 && g_checks == 5);
    CHECK(g_lastSpeed == &flag && g_lastAnim == 5 && g_lastExtra && g_lastPos.X == 7.0f && g_lastPos.Z == 9.0f);
    // first leg fails with 1: returned at once, no navigation
    g_script[0] = 1; g_scriptPos = 0; g_checks = 0; g_navs = 0;
    r = CALL(0);
    CHECK(r == 1 && g_navs == 0 && g_scriptPos == 1);
    // the condition fails during the second leg: aborted (3)
    g_script[0] = 2; g_script[1] = 4; g_scriptPos = 0; g_checks = 0; g_validUntil = 1;
    r = CALL(0);
    CHECK(r == 3 && g_navs == 1 && g_scriptPos == 2);
    // the condition fails on the first step
    g_script[0] = 2; g_scriptPos = 0; g_checks = 0; g_validUntil = 0;
    r = CALL(0);
    CHECK(r == 3 && g_scriptPos == 1);
    if (fails == 0) printf("0097f690_TEST PASS\n");
    return fails;
}
