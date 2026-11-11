// Behaviour test for 0x0092ACE0 CAIStateGroup_ApprenticeHero::GoToLibrary(): the row's body copied over test doubles
// (marker lookup, physics bounding info, AI actions); checks the no-marker, move-failed, look-failed, animation-failed and
// success paths, the move target (bounding centre), and the look target (forward divided by its XY magnitude, plus the marker
// position) with the 0xb flags and the file-static turn speed.
#include <cstdio>
#include <cmath>
#include <cstring>
namespace library_test {
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetMagnitude2D() const { return (float)std::sqrt(X * X + Y * Y); }
    C3DVector& operator/=(float d) { float r = 1.0f / d; X *= r; Y *= r; Z *= r; return *this; }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
enum EAIActionRepeat { ACTION_REPEAT_UNTIL_FINISHED = 1 };
enum EAIActionReturn { AI_ACTION_RUNNING = 1, AI_ACTION_FINISHED = 2 };
enum EAIMovementType { AI_MOVEMENT_WALKING = 3 };
struct CCharString { const char* s; CCharString(const char* t) : s(t) { } };
struct CRightHandedSet { C3DVector fwd; const C3DVector& GetForward() const { return fwd; } };
struct CTCPhysicsBase {
    C3DVector centre; float radius; CRightHandedSet rhs;
    void GetBoundingInfo(C3DVector& c, float& r) { c = centre; r = radius; }
    const CRightHandedSet& GetRHSet() const { return rhs; }
};
struct CThingGameObject {
    CTCPhysicsBase physics; C3DVector pos;
    CTCPhysicsBase* GetPhysicsTC() { return &physics; }
    const C3DVector& GetPos() const { return pos; }
};
struct CThingCreatureBase { int id; };
static CThingCreatureBase g_creature = { 7 };
struct CCreatureAction_PlayCombatAnimation {
    CThingCreatureBase* c; const char* anim; bool loop;
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* cr, const CCharString& a, bool l) : c(cr), anim(a.s), loop(l) { }
};
static CThingGameObject g_library;
static bool g_haveMarker = true;
static int g_moveResult = 2, g_lookResult = 2, g_animResult = 2;
static C3DVector g_movePos, g_lookPos; static long g_lookFlags = 0; static float g_lookSpeed = 0.0f;
static bool g_animOk = false; static int g_calls = 0;
static float g_LookAtTurnSpeed = 0.01f;
struct CAIStateGroup_ApprenticeHero {
    CThingGameObject* GetObjectWithScriptName(const CCharString& n) { ++g_calls; return (g_haveMarker && std::strcmp(n.s, "LibraryMarker") == 0) ? &g_library : 0; }
    CThingCreatureBase* GetCreature() const { return &g_creature; }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat r, C3DVector p, bool& reached, EAIMovementType m, bool a, bool b, long c, bool d, float e) {
        ++g_calls; reached = true; g_movePos = p;
        return (r == 1 && m == 3 && a && b && c == 0 && d && e == 0.0f) ? (EAIActionReturn)g_moveResult : AI_ACTION_RUNNING;
    }
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat r, C3DVector p, long flags, float speed) {
        ++g_calls; g_lookPos = p; g_lookFlags = flags; g_lookSpeed = speed;
        return r == 1 ? (EAIActionReturn)g_lookResult : AI_ACTION_RUNNING;
    }
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureAction_PlayCombatAnimation& a, bool flag) {
        ++g_calls; g_animOk = a.c == &g_creature && std::strcmp(a.anim, "ST_TRAINEE_PERUSE_LIBRARY") == 0 && a.loop && !flag;
        return (EAIActionReturn)g_animResult;
    }
    bool GoToLibrary();
};
bool CAIStateGroup_ApprenticeHero::GoToLibrary()
{
    CThingGameObject* library = GetObjectWithScriptName("LibraryMarker");
    if (!library)
        return true;
    C3DVector centre;
    float radius;
    library->GetPhysicsTC()->GetBoundingInfo(centre, radius);
    bool reached;
    if (AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, centre, reached, AI_MOVEMENT_WALKING, true, true, 0, true, 0.0f) != AI_ACTION_FINISHED)
        return false;
    else {
        C3DVector direction = library->GetPhysicsTC()->GetRHSet().GetForward();
        direction /= direction.GetMagnitude2D();
        const C3DVector& position = library->GetPos();
        C3DVector target = direction + position;
        if (AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, target, 0xb, g_LookAtTurnSpeed) != AI_ACTION_FINISHED)
            return false;
        bool failed = AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "ST_TRAINEE_PERUSE_LIBRARY", true),
            false) != AI_ACTION_FINISHED;
        return failed ? false : true;
    }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static void Reset(bool marker, int mv, int lk, int an)
{
    g_haveMarker = marker; g_moveResult = mv; g_lookResult = lk; g_animResult = an; g_calls = 0; g_animOk = false;
    g_lookFlags = 0; g_lookSpeed = 0.0f;
}
} // namespace library_test
using namespace library_test;
int main()
{
    bool ok = true;
    CAIStateGroup_ApprenticeHero g;
    g_library.physics.centre = C3DVector(10.0f, 20.0f, 1.0f); g_library.physics.radius = 2.0f;
    g_library.physics.rhs.fwd = C3DVector(3.0f, 4.0f, 2.0f); g_library.pos = C3DVector(100.0f, 200.0f, 5.0f);
    Reset(false, 2, 2, 2); ok = ok && g.GoToLibrary() == true && g_calls == 1;
    Reset(true, 1, 2, 2); ok = ok && g.GoToLibrary() == false && g_calls == 2 && g_movePos.X == 10.0f && g_movePos.Y == 20.0f;
    Reset(true, 2, 1, 2); ok = ok && g.GoToLibrary() == false && g_calls == 3;
    ok = ok && Near(g_lookPos.X, 100.6f) && Near(g_lookPos.Y, 200.8f) && Near(g_lookPos.Z, 5.4f) && g_lookFlags == 0xb && g_lookSpeed == 0.01f;
    Reset(true, 2, 2, 1); ok = ok && g.GoToLibrary() == false && g_calls == 4 && g_animOk;
    Reset(true, 2, 2, 2); ok = ok && g.GoToLibrary() == true && g_calls == 4 && g_animOk;
    std::printf(ok ? "0092ace0_TEST PASS\n" : "0092ace0_TEST FAIL\n");
    return ok ? 0 : 1;
}