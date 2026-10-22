#include <cstdio>
#include <cmath>
#include <vector>
// behaviour test: CCombatActionBase::ProcessPossibleCreatureStrikeEvent (0x00858130)
struct C3DVector {
    float X, Y, Z;
    void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
};
struct CVecPair { C3DVector first, second; };
static const float g_Epsilon = 0.0001f;
static long g_StrikeHitPauseFrames = 7;
struct CHitLocation { int id; };
struct CHitParameters { C3DVector Dir; C3DVector Pos; int Weapon; int Hitter; };
struct CTCHitLocations {
    CHitLocation* Result;
    int Calls;
    int HitOnCall;
    float FirstX;
    CHitLocation* Intersect(const std::vector<C3DVector>& tri, C3DVector* pos, C3DVector* nrm, float tol)
    {
        ++Calls;
        FirstX = tri[0].X;
        if (tri.size() != 3 || tol != 0.25f || Calls != HitOnCall) return 0;
        pos->X = 1.0f; pos->Y = 2.0f; pos->Z = 3.0f;
        nrm->X = 0.0f; nrm->Y = 0.0f; nrm->Z = 1.0f;
        return Result;
    }
};
struct CThing {
    int Id;
    bool Dead;
    CTCHitLocations* HitLocs;   // null = no interface 0x7f
    CHitParameters LastHit;
    int Hits;
    bool GetInterface(CTCHitLocations** out) { if (!HitLocs) return false; *out = HitLocs; return true; }
    void GetMeleeHitParameters(CHitLocation*, CThing& hitter, CThing* weapon, const C3DVector& pos, const C3DVector& dir, CHitParameters& out) const
    { out.Dir = dir; out.Pos = pos; out.Weapon = weapon ? weapon->Id : 0; out.Hitter = hitter.Id; }
    void OnHit(CHitParameters& h) { LastHit = h; ++Hits; }
};
struct CCombatActionBase {
    CThing* Creature;
    CThing* Target;
    CThing* Weapon;
    float RunningTimeSeconds, LastRunningTimeSeconds;
    bool HitThing, HitCreature, PerformedAction, PerformOnStrike;
    int Performs;
    bool HasPerformedAction() const { return PerformedAction; }
    bool IsToPerformActionOnStrike() const { return PerformOnStrike; }
    void PerformAction() { ++Performs; }
    long FrameUpdatePauseFrames;
    std::vector<CThing*> Struck;
    int FramesRunCalls;
    bool IsThingInStruckThingList(const CThing& t) const { for (size_t i = 0; i < Struck.size(); ++i) if (Struck[i] == &t) return true; return false; }
    void ProcessFrames(const C3DVector&) { ++FramesRunCalls; }
    void Process(float, const CVecPair& previous, const CVecPair& current, CThing& thing)
    {
        if (IsThingInStruckThingList(thing))
            return;
        CTCHitLocations* hitLocations;
        if (!thing.GetInterface(&hitLocations))
            return;
        C3DVector hitPosition, hitNormal;
        CHitLocation* hit;
        const C3DVector* tip;
        {
            std::vector<C3DVector> triangle;
            triangle.push_back(current.first);
            tip = &current.second;
            triangle.push_back(*tip);
            triangle.push_back(previous.first);
            hit = hitLocations->Intersect(triangle, &hitPosition, &hitNormal, 0.25f);
        }
        if (!hit) {
            std::vector<C3DVector> triangle;
            triangle.push_back(*tip);
            triangle.push_back(previous.second);
            triangle.push_back(previous.first);
            hit = hitLocations->Intersect(triangle, &hitPosition, &hitNormal, 0.25f);
        }
        if (hit) {
            Struck.push_back(&thing);
            if (!HasPerformedAction() && IsToPerformActionOnStrike() && Target == &thing) {
                PerformAction();
                PerformedAction = true;
            }
            C3DVector direction;
            direction.X = tip->X - previous.second.X;
            direction.Y = tip->Y - previous.second.Y;
            direction.Z = tip->Z - previous.second.Z;
            if (direction.Z * direction.Z + direction.Y * direction.Y + direction.X * direction.X > g_Epsilon * g_Epsilon)
                direction.Normalise();
            CThing* weapon = Weapon;
            CHitParameters params;
            thing.GetMeleeHitParameters(hit, *Creature, weapon, hitPosition, direction, params);
            thing.OnHit(params);
            if (Target == &thing || (!(Target && !Target->Dead) && !HitThing))
                FrameUpdatePauseFrames = g_StrikeHitPauseFrames;
            HitThing = true;
            HitCreature = true;
            ProcessFrames(hitPosition);
        }
    }
};
static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); g_fail = 1; } }
int main()
{
    CHitLocation loc = { 1 };
    CTCHitLocations tc = { &loc, 0, 2 };
    CThing hero = { 10, false, 0 }; hero.Hits = 0;
    CThing sword = { 20, false, 0 }; sword.Hits = 0;
    CThing foe = { 30, false, &tc }; foe.Hits = 0;
    CThing rock = { 40, false, 0 }; rock.Hits = 0;
    CCombatActionBase a;
    a.Creature = &hero; a.Target = &foe; a.Weapon = &sword; a.HitThing = false; a.HitCreature = false;
    a.PerformedAction = false; a.PerformOnStrike = true; a.Performs = 0; a.FrameUpdatePauseFrames = 0; a.FramesRunCalls = 0;
    CVecPair prev = { { 0, 0, 0 }, { 50, 0, 1 } };
    CVecPair cur = { { 9, 9, 9 }, { 53, 4, 1 } };
    a.Process(0.0f, prev, cur, rock);
    Check(rock.Hits == 0 && a.Struck.empty(), "no hit-location interface");
    a.Process(0.0f, prev, cur, foe);
    Check(tc.Calls == 2 && tc.FirstX == 53.0f, "second triangle starts at the current tip");
    Check(foe.Hits == 1 && a.Struck.size() == 1 && a.Performs == 1 && a.PerformedAction, "hit and action on strike");
    Check(std::fabs(foe.LastHit.Dir.X - 0.6f) < 1e-5f && std::fabs(foe.LastHit.Dir.Y - 0.8f) < 1e-5f, "tip sweep normalised");
    Check(a.FrameUpdatePauseFrames == 7 && a.HitThing && a.HitCreature && a.FramesRunCalls == 1, "pause, flags and frames");
    a.Process(0.0f, prev, cur, foe);
    Check(foe.Hits == 1 && tc.Calls == 2, "already struck");
    CThing other = { 50, false, &tc }; other.Hits = 0;
    tc.Calls = 0; tc.HitOnCall = 1;
    a.PerformedAction = false;
    a.Process(0.0f, prev, cur, other);
    Check(other.Hits == 1 && tc.Calls == 1 && tc.FirstX == 9.0f && a.Performs == 1, "first triangle hits; not the target: no action");
    CThing miss = { 60, false, &tc }; miss.Hits = 0;
    tc.Calls = 0; tc.HitOnCall = 0;
    a.Process(0.0f, prev, cur, miss);
    Check(miss.Hits == 0 && tc.Calls == 2 && a.Struck.size() == 2, "both triangles miss");
    if (!g_fail) std::printf("00858130_TEST PASS\n");
    return g_fail;
}
