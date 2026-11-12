// Behaviour test for 0x009711D0 CAIStateGroup_CreatureUseCoverBase::MoveIntoCover(const C3DVector&): the row's logic over stub
// classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
static std::string trace;
static void check(bool v, int line) { if (!v) { printf("009711d0_TEST FAIL line %d\n", line); exit(1); } }
enum EAIActionReturn { AI_ACTION_NULL = 0, AI_ACTION_TERMINATE_PROCESS = 1, AI_ACTION_FINISHED = 2, AI_ACTION_FAILED = 3, AI_ACTION_IN_PROGRESS = 4 };
enum EAIActionRepeat { ACTION_REPEAT_UNTIL_FINISHED = 1, ACTION_REPEAT_CALLBACK = 2 };
enum EMovementType { AI_MOVEMENT_RUNNING = 5 };
enum { MODE_STRAFE = 25 };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetMagnitude2D() const { return (float)sqrt(X * X + Y * Y); }
};
static float GFGetWorld2DDistance(const C3DVector& a, const C3DVector& b)
{ float dy = a.Y - b.Y; float dx = a.X - b.X; float d = dx * dx + dy * dy; return (float)sqrt(d); }
class CThing { public: bool Dead; C3DVector Pos; bool IsDead() const { return Dead; } const C3DVector& GetPos() const { return Pos; } };
class CTCCreatureModeManager { public: unsigned long Mode; bool IsModeActive(unsigned long m) const { return m == Mode; } };
class CThingAICreature : public CThing { public: CTCCreatureModeManager modes;
    bool PeekTC(const CTCCreatureModeManager** out) const { *out = &modes; return true; } };
class CAIGameCameraBase { public: C3DVector Pos; const C3DVector PeekCurrentPos() const { return Pos; } };
class CWorld { public: CAIGameCameraBase camera; CAIGameCameraBase* GetGameCamera() { return &camera; } };
template <class T> class CIntelligentPointer { public: T* Item; T* operator->() const { return Item; } };
template <class T> static bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) { if (p.Item != 0) { if (!p->IsDead()) return true; } return false; }
class CCreatureAction_DuckAndCover { public:
    C3DVector Facing;
    CCreatureAction_DuckAndCover(CThingAICreature&, const C3DVector& pos) : Facing(pos) { trace += 'C'; }
    ~CCreatureAction_DuckAndCover() { trace += 'c'; }
};
class CAIStateGroup_CreatureUseCoverBase { public:
    CThingAICreature creature; CWorld world;
    CIntelligentPointer<CThing> PTargetThing;
    bool HasDiveAnim, TimingCritical; long ValidPositionsFound;
    const EAIActionReturn* results; bool last_near_camera; float last_facing_x;
    CThingAICreature& GetCreature() const { return const_cast<CThingAICreature&>(creature); }
    CWorld& GetWorld() const { return const_cast<CWorld&>(world); }
    void Terminate() { trace += 'X'; }
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureAction_DuckAndCover& a, bool wait)
    { check(!wait, __LINE__); last_facing_x = a.Facing.X; trace += 'D'; return *results++; }
    EAIActionReturn AIAction_ProjectileAttackAndStrafeToPos(EAIActionRepeat repeat, C3DVector pos, CIntelligentPointer<CThing> target,
        bool& nav, EMovementType movement, bool near_camera)
    { check(repeat == ACTION_REPEAT_CALLBACK && movement == AI_MOVEMENT_RUNNING && target.Item == PTargetThing.Item, __LINE__);
      (void)pos; nav = true; last_near_camera = near_camera; trace += 'S'; return *results++; }
    bool MoveIntoCover(const C3DVector& cover_pos);
};

bool CAIStateGroup_CreatureUseCoverBase::MoveIntoCover(const C3DVector& cover_pos)
{
    TimingCritical = true;
    const CTCCreatureModeManager* modes;
    GetCreature().PeekTC(&modes);
    bool strafing = modes->IsModeActive(MODE_STRAFE);
    EAIActionReturn ret;
    bool nav_route_calculated;
    do {
        if (!IS_THING_ALIVE(PTargetThing)) {
            Terminate();
            TimingCritical = false;
            return false;
        }
        C3DVector offset = cover_pos - GetCreature().GetPos();
        float distance = offset.GetMagnitude2D();
        if (!strafing && HasDiveAnim && distance > 2.4f && distance < 2.8f) {
            ret = AIAction_PerformActionAndWait(CCreatureAction_DuckAndCover(GetCreature(), offset), false);
        } else {
            ret = AIAction_ProjectileAttackAndStrafeToPos(ACTION_REPEAT_CALLBACK, cover_pos, PTargetThing, nav_route_calculated,
                AI_MOVEMENT_RUNNING, GFGetWorld2DDistance(GetCreature().GetPos(), GetWorld().GetGameCamera()->PeekCurrentPos()) < 10.0f);
        }
    } while (ret == AI_ACTION_IN_PROGRESS);
    TimingCritical = false;
    if (ret == AI_ACTION_FINISHED) {
        ++ValidPositionsFound;
        return true;
    }
    return false;
}

int main()
{
    CThing target; target.Dead = false;
    CAIStateGroup_CreatureUseCoverBase g;
    g.PTargetThing.Item = &target; g.HasDiveAnim = true; g.ValidPositionsFound = 0; g.TimingCritical = false;
    g.creature.Dead = false; g.creature.Pos = C3DVector(0.0f, 0.0f, 0.0f); g.creature.modes.Mode = 0;
    g.world.camera.Pos = C3DVector(3.0f, 4.0f, 0.0f);                    // 5 units from the creature
    static const EAIActionReturn r1[] = { AI_ACTION_FINISHED };
    g.results = r1; trace = "";
    check(g.MoveIntoCover(C3DVector(2.5f, 0.0f, 1.0f)) && trace == "CDc" && g.ValidPositionsFound == 1 && !g.TimingCritical, __LINE__);
    check(g.last_facing_x == 2.5f, __LINE__);                              // dive: 2.5 units away
    static const EAIActionReturn r2[] = { AI_ACTION_IN_PROGRESS, AI_ACTION_FAILED };
    g.results = r2; trace = "";
    check(!g.MoveIntoCover(C3DVector(5.0f, 0.0f, 0.0f)) && trace == "SS" && g.ValidPositionsFound == 1, __LINE__);
    check(g.last_near_camera, __LINE__);                                   // the camera is within 10 units
    g.world.camera.Pos = C3DVector(30.0f, 0.0f, 0.0f);
    g.creature.modes.Mode = MODE_STRAFE;                                   // strafing: never dives
    static const EAIActionReturn r3[] = { AI_ACTION_FINISHED };
    g.results = r3; trace = "";
    check(g.MoveIntoCover(C3DVector(2.5f, 0.0f, 0.0f)) && trace == "S" && !g.last_near_camera && g.ValidPositionsFound == 2, __LINE__);
    target.Dead = true; trace = ""; g.TimingCritical = true;
    check(!g.MoveIntoCover(C3DVector(2.5f, 0.0f, 0.0f)) && trace == "X" && !g.TimingCritical, __LINE__);
    g.PTargetThing.Item = 0; trace = "";
    check(!g.MoveIntoCover(C3DVector(2.5f, 0.0f, 0.0f)) && trace == "X", __LINE__);
    printf("009711d0_TEST PASS\n");
    return 0;
}