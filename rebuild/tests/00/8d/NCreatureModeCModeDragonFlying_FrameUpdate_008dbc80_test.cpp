// Behaviour test for 0x008DBC80 NCreatureMode::CModeDragonFlying::FrameUpdate (source-level, like the landed creature-mode test
// 0x008E07F0). The exact production body below is compiled against controlled stand-ins that have the row's interfaces. A double
// precision model written separately checks the results: the spline progress and its clamp, the move, the turn delta and its clamp,
// the banked up vector (skipped near the end or with UsePreviousPos), the flattened and normalised forward, the interpolated
// orientation, the leg-anim alpha, the look target (facing x 50 + position on the last loop, else ahead on the spline), the loop
// counter, and the end of flight (level orientation, mode removal, end action). Run time: well under a second.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define CHECK(c) do { if (!(c)) { printf("check failed at line %d (case %d)\n", __LINE__, g_case); exit(1); } } while (0)
static int g_case = 0;
static int g_base_updates = 0, g_rotate_calls = 0, g_rotatexy_calls = 0, g_set_rhs_calls = 0, g_look_calls = 0;
static int g_remove_mode = -1, g_actions = 0, g_alpha_calls = 0;
static float g_rotate_angle = 0.0f, g_rotatexy_angle = 0.0f, g_alpha_value = 0.0f, g_interp_t = 0.0f;
static long g_look_priority = 0;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    const C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    const C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    C3DVector& Normalise() { float m = (float)sqrt(X * X + Y * Y + Z * Z); X /= m; Y /= m; Z /= m; return *this; }
    float GetAngleXY() const { return (float)atan2(X, Y); }
    // Stand-in rotations: deterministic, distinct per axis, recorded.
    void Rotate(const C3DVector& axis, float angle) { ++g_rotate_calls; g_rotate_angle = angle; X += axis.Y * angle; Y += axis.X * angle; }
    void RotateXY(float angle) { ++g_rotatexy_calls; g_rotatexy_angle = angle; float c = (float)cos(angle), s = (float)sin(angle);
        float x = X * c - Y * s; Y = X * s + Y * c; X = x; }
};
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet() {}
    const C3DVector& GetForward() const { return Forward; }
    void SetForward(const C3DVector& f) { Forward = f; }
    void SetUp(const C3DVector& u) { Up = u; }
    CRightHandedSet Interpolate(const CRightHandedSet& o, float t) const { g_interp_t = t; CRightHandedSet r;
        r.Up = Up + (o.Up - Up) * t; r.Forward = Forward + (o.Forward - Forward) * t; return r; }
};
class CThing;
class CTCPhysicsBase { public:
    CThing* Owner; C3DVector Facing; CRightHandedSet RHS;
    void SetPosition(const C3DVector& p);
    C3DVector GetFacingVector() const { return Facing; }
    const CRightHandedSet& GetRHSet() const { return RHS; }
    void SetRHSet(const CRightHandedSet& r) { ++g_set_rhs_calls; RHS = r; }
};
enum ETCInterfaceType { TCI_ANIMATION = 1, TCI_LOOK = 2, TCI_ENTITY_MODE_MANAGER = 3 };
enum ECreatureModeType { MODE_DRAGON_FLYING = 0x2b };
class CThing { public:
    C3DVector Pos; CTCPhysicsBase* Physics; void* TCs[4];
    const C3DVector& GetPos() const { return Pos; }
    CTCPhysicsBase* GetPhysicsTC() { return Physics; }
    template <class T> bool GetTC(ETCInterfaceType i, T** p) { *p = (T*)TCs[i]; return *p != 0; }
};
void CTCPhysicsBase::SetPosition(const C3DVector& p) { Owner->Pos = p; }
class CCreatureActionBase { public: int Kind; };
template <class T> class CCountedPointer { public:
    T* P;
    CCountedPointer() : P(0) {}
    CCountedPointer(const CCountedPointer& o) : P(o.P) {}
    bool IsNull() const { return P == 0; }
};
class CThingCreatureBase : public CThing { public:
    void SetCurrentAction(const CCountedPointer<CCreatureActionBase>& a) { g_actions += a.P->Kind; }
};
class CAnimSet { public:
    void SetInternalAlpha(int index, float alpha, bool b) { ++g_alpha_calls; g_alpha_value = alpha; CHECK(index == 0 && !b); }
};
class CMainAnim { public: float Percent; float GetPercentageComplete() const { return Percent; } };
class CAnimateThing { public:
    CMainAnim Main; CAnimSet* Leg;
    CMainAnim& GetMainAnim() { return Main; }
    CAnimSet* GetFirstCombinationAnimFromType(long type) { CHECK(type == 0x22); return Leg; }
};
class CTCAnimationComplex { public:
    CAnimateThing Anim; long Index; float Durations[4];
    CAnimateThing& GetAnimation() { return Anim; }
    long GetAnimIndex() const { return Index; }
    float GetAnimDuration(long i) const { return Durations[i]; }
};
class CTCLook { public:
    C3DVector Target;
    void LookAtPos(const C3DVector& p, long priority) { ++g_look_calls; Target = p; g_look_priority = priority; }
};
class CCardinalSpline { public:
    long Count;
    long GetControlPointCount() const { return Count; }
    void GetSplinePosition(C3DVector& out, float t) const { out = C3DVector(t * 100.0f, t * t * 40.0f + 5.0f, 20.0f - t * 8.0f); }
};
static float FABLE_FASTCALL GFGetSignedAngleDifference(float a, float b) { return a - b; }
class CTCCreatureModeManager { public:
    class CModeBase { public:
        CThing& ParentThing;
        CModeBase(CThing& t) : ParentThing(t) {}
        CThing& GetParentThing() { return ParentThing; }
        void FrameUpdate() { ++g_base_updates; }
    };
    void RemoveMode(ECreatureModeType m) { g_remove_mode = m; }
};

namespace NCreatureMode {
class CModeDragonFlyingBase : public CTCCreatureModeManager::CModeBase { public:
    CModeDragonFlyingBase(CThing& t) : CTCCreatureModeManager::CModeBase(t) {}
    CCardinalSpline FlightSpline;
    float           SplinePercentageComplete;
    float           LegAnimAlpha;
};
class CModeDragonFlying : public CModeDragonFlyingBase { public:
    CModeDragonFlying(CThing& t) : CModeDragonFlyingBase(t) {}
    void FrameUpdate();
    C3DVector       Destination;
    C3DVector       StartPos;
    C3DVector       PreviousPos;
    CCountedPointer<CCreatureActionBase> PEndAction;
    long            NumAnimLoops;
    long            NumAnimLoopsToDo;
    bool            UsePreviousPos;
    bool            FirstFrame;
};

void CModeDragonFlying::FrameUpdate()
{
    CTCCreatureModeManager::CModeBase::FrameUpdate();
    CThingCreatureBase* creature = (CThingCreatureBase*)&GetParentThing();
    C3DVector vector_travelled = GetParentThing().GetPos() - PreviousPos;
    C3DVector vector_to_dest = Destination - GetParentThing().GetPos();
    float dist_to_target = vector_to_dest.GetMagnitude();
    CTCAnimationComplex* ptc_animation = 0;
    GetParentThing().GetTC(TCI_ANIMATION, &ptc_animation);
    CAnimateThing* anim = &ptc_animation->GetAnimation();
    float anim_length_in_frames = ptc_animation->GetAnimDuration(ptc_animation->GetAnimIndex());
    float anim_percent_done = anim->GetMainAnim().GetPercentageComplete();
    float percentage_complete_per_frame = 1.0f / anim_length_in_frames;
    float percent_of_spline_travelled = ((float)NumAnimLoops + anim_percent_done) / (float)NumAnimLoopsToDo;
    if (FirstFrame) {
        percent_of_spline_travelled *= 1.0f - 1.0f / ((float)FlightSpline.GetControlPointCount() - 1.0f);
        percent_of_spline_travelled += 1.0f / ((float)FlightSpline.GetControlPointCount() - 1.0f);
    }
    if (percent_of_spline_travelled > 1.0f)
        percent_of_spline_travelled = 1.0f;
    SplinePercentageComplete = percent_of_spline_travelled;
    C3DVector new_pos;
    FlightSpline.GetSplinePosition(new_pos, percent_of_spline_travelled);
    PreviousPos = GetParentThing().GetPos();
    GetParentThing().GetPhysicsTC()->SetPosition(new_pos);
    C3DVector movement_direction = new_pos - PreviousPos;
    movement_direction = movement_direction.Normalise();
    C3DVector previous_facing_direction = GetParentThing().GetPhysicsTC()->GetFacingVector();
    float angle_delta = GFGetSignedAngleDifference(movement_direction.GetAngleXY(), previous_facing_direction.GetAngleXY());
    angle_delta *= 3.0f;
    if (angle_delta > 0.125f)
        angle_delta = 0.125f;
    else if (angle_delta < -0.125f)
        angle_delta = -0.125f;
    C3DVector movement_direction_2d = movement_direction;
    movement_direction_2d.Z = 0.0f;
    movement_direction_2d = movement_direction_2d.Normalise();
    C3DVector up(0.0f, 0.0f, 1.0f);
    float roll_angle = angle_delta;
    C3DVector ideal_movement_direction = movement_direction_2d;
    if (!(anim_percent_done > 0.75f && !PEndAction.IsNull() && NumAnimLoops == NumAnimLoopsToDo - 1) && !UsePreviousPos) {
        up.Rotate(C3DVector(0.0f, 1.0f, 0.0f), roll_angle);
        up.RotateXY(ideal_movement_direction.GetAngleXY());
        up = up.Normalise();
    }
    CRightHandedSet rhs;
    rhs.SetForward(movement_direction_2d);
    rhs.SetUp(up);
    rhs = GetParentThing().GetPhysicsTC()->GetRHSet().Interpolate(rhs, 0.25f);
    GetParentThing().GetPhysicsTC()->SetRHSet(rhs);
    CAnimSet* pleg_anim_set = anim->GetFirstCombinationAnimFromType(0x22);
    if (pleg_anim_set) {
        float desired_alpha = (float)fabs(angle_delta) * 8.0f;
        if (anim_percent_done > 0.75f && NumAnimLoops == NumAnimLoopsToDo - 1)
            desired_alpha = 0.0f;
        float new_alpha = (desired_alpha - LegAnimAlpha) * 0.1f + LegAnimAlpha;
        pleg_anim_set->SetInternalAlpha(0, new_alpha, false);
        LegAnimAlpha = new_alpha;
    }
    CTCLook* ptc_look = 0;
    if (GetParentThing().GetTC(TCI_LOOK, &ptc_look)) {
        C3DVector look_pos;
        if (anim_percent_done > 0.5f && NumAnimLoops == NumAnimLoopsToDo - 1) {
            look_pos = GetParentThing().GetPos() + GetParentThing().GetPhysicsTC()->GetFacingVector() * 50.0f;
        } else {
            FlightSpline.GetSplinePosition(look_pos, (1.0f - percent_of_spline_travelled) * 0.5f + percent_of_spline_travelled);
        }
        ptc_look->LookAtPos(look_pos, 10);
    }
    UsePreviousPos = false;
    float end_percent = 0.999f;
    if (anim_percent_done > end_percent) {
        NumAnimLoops++;
        if (NumAnimLoops >= NumAnimLoopsToDo) {
            CRightHandedSet end_rhs = GetParentThing().GetPhysicsTC()->GetRHSet();
            end_rhs.SetUp(C3DVector(0.0f, 0.0f, 1.0f));
            C3DVector end_forward = end_rhs.GetForward();
            end_forward.Z = 0.0f;
            end_forward = end_forward.Normalise();
            end_rhs.SetForward(end_forward);
            GetParentThing().GetPhysicsTC()->SetRHSet(end_rhs);
            CCountedPointer<CCreatureActionBase> p_end_action = PEndAction;
            CTCCreatureModeManager* ptc_mode_manager;
            GetParentThing().GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_mode_manager);
            ptc_mode_manager->RemoveMode(MODE_DRAGON_FLYING);
            if (!p_end_action.IsNull())
                creature->SetCurrentAction(p_end_action);
        }
    }
}
}

// ---- independent double-precision model ----
struct V { double x, y, z; };
static V mk(double x, double y, double z) { V v; v.x = x; v.y = y; v.z = z; return v; }
static V nrm(V v) { double m = sqrt(v.x * v.x + v.y * v.y + v.z * v.z); return mk(v.x / m, v.y / m, v.z / m); }
static V spline(double t) { return mk(t * 100.0, t * t * 40.0 + 5.0, 20.0 - t * 8.0); }
static bool near1(double a, double b) { return fabs(a - b) <= 2e-5 * (1.0 + fabs(b)); }
static bool nearv(const C3DVector& a, V b) { return near1(a.X, b.x) && near1(a.Y, b.y) && near1(a.Z, b.z); }

struct Case {
    float pos[3], dest[3], facing[3], up0[3], fwd0[3];
    float percent; long loops, todo, count; bool first, end_action, use_prev, leg, look; float leg_alpha;
};

static void run(const Case& c)
{
    ++g_case;
    g_base_updates = g_rotate_calls = g_rotatexy_calls = g_set_rhs_calls = g_look_calls = g_alpha_calls = g_actions = 0;
    g_remove_mode = -1; g_look_priority = 0;
    CThingCreatureBase thing;
    CTCPhysicsBase phys;
    CTCAnimationComplex anim;
    CAnimSet leg;
    CTCLook look;
    CTCCreatureModeManager mm;
    CCreatureActionBase action; action.Kind = 7;
    thing.Pos = C3DVector(c.pos[0], c.pos[1], c.pos[2]); thing.Physics = &phys;
    thing.TCs[0] = 0; thing.TCs[TCI_ANIMATION] = &anim; thing.TCs[TCI_LOOK] = c.look ? &look : 0; thing.TCs[TCI_ENTITY_MODE_MANAGER] = &mm;
    phys.Owner = &thing; phys.Facing = C3DVector(c.facing[0], c.facing[1], c.facing[2]);
    phys.RHS.Up = C3DVector(c.up0[0], c.up0[1], c.up0[2]); phys.RHS.Forward = C3DVector(c.fwd0[0], c.fwd0[1], c.fwd0[2]);
    anim.Index = 2; anim.Durations[0] = anim.Durations[1] = anim.Durations[3] = 1.0f; anim.Durations[2] = 40.0f;
    anim.Anim.Main.Percent = c.percent; anim.Anim.Leg = c.leg ? &leg : 0;
    NCreatureMode::CModeDragonFlying m(thing);
    m.FlightSpline.Count = c.count; m.SplinePercentageComplete = -1.0f; m.LegAnimAlpha = c.leg_alpha;
    m.Destination = C3DVector(c.dest[0], c.dest[1], c.dest[2]); m.StartPos = C3DVector(0, 0, 0); m.PreviousPos = C3DVector(-9, -9, -9);
    m.PEndAction.P = c.end_action ? &action : 0; m.NumAnimLoops = c.loops; m.NumAnimLoopsToDo = c.todo;
    m.UsePreviousPos = c.use_prev; m.FirstFrame = c.first;

    m.FrameUpdate();

    // model
    double a = c.percent, L = (double)c.loops, T = (double)c.todo;
    double p = (L + a) / T;
    if (c.first) { double k = 1.0 / ((double)c.count - 1.0); p = p * (1.0 - k) + k; }
    if (p > 1.0) p = 1.0;
    V oldp = mk(c.pos[0], c.pos[1], c.pos[2]);
    V np = spline(p);
    V md = nrm(mk(np.x - oldp.x, np.y - oldp.y, np.z - oldp.z));
    double delta = 3.0 * (atan2(md.x, md.y) - atan2((double)c.facing[0], (double)c.facing[1]));
    if (delta > 0.125) delta = 0.125; else if (delta < -0.125) delta = -0.125;
    V md2 = nrm(mk(md.x, md.y, 0.0));
    V up = mk(0, 0, 1);
    bool last_loop = c.loops == c.todo - 1;
    bool bank = !(a > 0.75 && c.end_action && last_loop) && !c.use_prev;
    if (bank) {
        up.x += delta;
        double ang = atan2(md2.x, md2.y), cs = cos(ang), sn = sin(ang);
        V r = mk(up.x * cs - up.y * sn, up.x * sn + up.y * cs, up.z); up = nrm(r);
    }
    V rup = mk(c.up0[0] + (up.x - c.up0[0]) * 0.25, c.up0[1] + (up.y - c.up0[1]) * 0.25, c.up0[2] + (up.z - c.up0[2]) * 0.25);
    V rfw = mk(c.fwd0[0] + (md2.x - c.fwd0[0]) * 0.25, c.fwd0[1] + (md2.y - c.fwd0[1]) * 0.25, c.fwd0[2] + (md2.z - c.fwd0[2]) * 0.25);
    bool ends = a > 0.999 && c.loops + 1 >= c.todo;

    CHECK(g_base_updates == 1);
    CHECK(near1(m.SplinePercentageComplete, p));
    CHECK(nearv(m.PreviousPos, oldp));
    CHECK(nearv(thing.Pos, np));
    CHECK(g_rotate_calls == (bank ? 1 : 0) && g_rotatexy_calls == (bank ? 1 : 0));
    if (bank) { CHECK(near1(g_rotate_angle, delta)); CHECK(near1(g_rotatexy_angle, atan2(md2.x, md2.y))); }
    CHECK(near1(g_interp_t, 0.25));
    CHECK(g_set_rhs_calls == (ends ? 2 : 1));
    if (!ends) { CHECK(nearv(phys.RHS.Up, rup)); CHECK(nearv(phys.RHS.Forward, rfw)); }
    else {
        CHECK(nearv(phys.RHS.Up, mk(0, 0, 1)));
        CHECK(nearv(phys.RHS.Forward, nrm(mk(rfw.x, rfw.y, 0.0))));
    }
    if (c.leg) {
        double desired = fabs(delta) * 8.0;
        if (a > 0.75 && last_loop) desired = 0.0;
        double na = (desired - c.leg_alpha) * 0.1 + c.leg_alpha;
        CHECK(g_alpha_calls == 1 && near1(g_alpha_value, na) && near1(m.LegAnimAlpha, na));
    } else CHECK(g_alpha_calls == 0 && m.LegAnimAlpha == c.leg_alpha);
    if (c.look) {
        V t;
        if (a > 0.5 && last_loop) t = mk(c.facing[0] * 50.0 + np.x, c.facing[1] * 50.0 + np.y, c.facing[2] * 50.0 + np.z);
        else t = spline((1.0 - p) * 0.5 + p);
        CHECK(g_look_calls == 1 && g_look_priority == 10 && nearv(look.Target, t));
    } else CHECK(g_look_calls == 0);
    CHECK(m.UsePreviousPos == false);
    CHECK(m.NumAnimLoops == c.loops + (a > 0.999 ? 1 : 0));
    CHECK(g_remove_mode == (ends ? (int)MODE_DRAGON_FLYING : -1));
    CHECK(g_actions == ((ends && c.end_action) ? 7 : 0));
}

int main()
{
    //            pos               dest          facing            up0           fwd0           pct   loops todo cnt first  end  prev   leg   look  alpha
    Case cs[] = {
        { {0, 2, 30},    {50, 50, 0}, {0, 1, 0},      {0, 0, 1},    {0, 1, 0},     0.20f, 0, 4, 5, false, true,  false, true,  true,  0.2f },
        { {3, 1, 25},    {50, 50, 0}, {0.3f, 1, 0},   {0.1f, 0, 1}, {1, 0, 0},     0.60f, 1, 4, 5, true,  false, false, true,  true,  0.5f },
        { {10, 4, 20},   {0, 0, 0},   {1, 0, 0},      {0, 0.2f, 1}, {0, 1, 0.2f},  0.90f, 3, 4, 6, false, true,  false, true,  true,  0.0f },
        { {10, 4, 20},   {0, 0, 0},   {1, 0, 0},      {0, 0.2f, 1}, {0, 1, 0.2f},  0.9995f, 3, 4, 6, false, true, false, true, true, 0.3f },
        { {10, 4, 20},   {0, 0, 0},   {-1, 0.1f, 0},  {0, 0, 1},    {0.5f, 1, 0},  0.9995f, 3, 4, 6, false, false, true, false, false, 0.3f },
        { {-5, -5, 22},  {9, 9, 9},   {0, -1, 0},     {0, 0, 1},    {0, -1, 0},    0.40f, 2, 4, 7, true,  true,  true,  false, true,  0.9f },
        { {60, 30, 18},  {1, 2, 3},   {0.2f, 1, 0.1f},{0, 0, 1},    {1, 1, 0},     0.9995f, 1, 4, 5, false, true, false, true, true, 0.1f },
        { {99, 40, 14},  {1, 2, 3},   {1, 1, 0},      {0, 0, 1},    {1, 1, 0},     0.30f, 5, 4, 5, false, false, false, true,  true,  0.4f },
        { {5, 0, 19.6f}, {1, 1, 1},   {0.01f, 1, 0},  {0, 0, 1},    {0, 1, 0},     0.20f, 0, 4, 5, false, false, false, true,  true,  0.2f },
        { {5, 0, 21},    {1, 1, 1},   {-0.02f, 1, 0.3f}, {0.2f, 0.1f, 1}, {0.3f, 1, 0}, 0.20f, 0, 4, 5, false, true, false, true, true, 0.6f },
        { {0, 0, 0},     {1, 1, 1},   {0, 1, 0},      {0, 0, 1},    {0, 1, 0},     0.80f, 2, 4, 3, true,  true,  false, true,  false, 0.7f },
    };
    for (int i = 0; i < (int)(sizeof(cs) / sizeof(cs[0])); ++i) run(cs[i]);
    printf("008dbc80_TEST PASS\n");
    return 0;
}