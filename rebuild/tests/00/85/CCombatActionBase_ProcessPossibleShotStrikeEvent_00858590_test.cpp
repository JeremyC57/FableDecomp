#include <cstdio>
#include <cmath>
#include <vector>
// behaviour test: CCombatActionBase::ProcessPossibleShotStrikeEvent (0x00858590)
class C3DVector { public:
    float X, Y, Z;
    void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
};
struct CVecPair { C3DVector first, second; };
static const float g_Epsilon = 0.0001f;
static long g_StrikeHitPauseFrames = 7;
struct CHitLocation { int id; };
class CHitParameters { public: C3DVector Dir; C3DVector Pos; int Weapon; int Hitter; };
class CTCHitLocations { public:
    CHitLocation* Result;
    int Calls;
    CHitLocation* Intersect(const std::vector<C3DVector>& tri, C3DVector* pos, C3DVector* nrm, float tol)
    {
        ++Calls;
        if (tri.size() != 3 || tol != 0.25f) return 0;
        pos->X = 1.0f; pos->Y = 2.0f; pos->Z = 3.0f;
        nrm->X = 0.0f; nrm->Y = 0.0f; nrm->Z = 1.0f;
        return Result;
    }
};
class CThing { public:
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
class CCombatActionBase { public:
    CThing* Creature;
    CThing* Target;
    CThing* Weapon;
    float RunningTimeSeconds, LastRunningTimeSeconds;
    bool HitThing;
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
        {
            std::vector<C3DVector> triangle;
            triangle.push_back(current.first);
            triangle.push_back(current.second);
            triangle.push_back(previous.first);
            hit = hitLocations->Intersect(triangle, &hitPosition, &hitNormal, 0.25f);
        }
        if (hit) {
            Struck.push_back(&thing);
            C3DVector direction;
            direction.X = current.first.X - previous.first.X;
            direction.Y = current.first.Y - previous.first.Y;
            direction.Z = current.first.Z - previous.first.Z;
            if (direction.Z * direction.Z + direction.Y * direction.Y + direction.X * direction.X > g_Epsilon * g_Epsilon)
                direction.Normalise();
            CThing* weapon = Weapon;
            CHitParameters params;
            thing.GetMeleeHitParameters(hit, *Creature, weapon, hitPosition, direction, params);
            thing.OnHit(params);
            if (Target == &thing || (!(Target && !Target->Dead) && !HitThing))
                FrameUpdatePauseFrames = g_StrikeHitPauseFrames;
            HitThing = true;
            ProcessFrames(hitPosition);
        }
    }
};
static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); g_fail = 1; } }
int main()
{
    CHitLocation loc = { 1 };
    CTCHitLocations tc = { &loc, 0 };
    CThing hero = { 10, false, 0 }; hero.Hits = 0;
    CThing sword = { 20, false, 0 }; sword.Hits = 0;
    CThing foe = { 30, false, &tc }; foe.Hits = 0;
    CThing rock = { 40, false, 0 }; rock.Hits = 0;
    CCombatActionBase a;
    a.Creature = &hero; a.Target = &foe; a.Weapon = &sword; a.HitThing = false; a.FrameUpdatePauseFrames = 0; a.FramesRunCalls = 0;
    CVecPair prev = { { 0, 0, 0 }, { 0, 0, 1 } };
    CVecPair cur = { { 3, 4, 0 }, { 3, 4, 1 } };
    a.Process(0.0f, prev, cur, rock);
    Check(rock.Hits == 0 && a.Struck.empty(), "no hit-location interface");
    a.Process(0.0f, prev, cur, foe);
    Check(foe.Hits == 1 && a.Struck.size() == 1, "hit recorded");
    Check(std::fabs(foe.LastHit.Dir.X - 0.6f) < 1e-5f && std::fabs(foe.LastHit.Dir.Y - 0.8f) < 1e-5f, "direction normalised");
    Check(foe.LastHit.Pos.Y == 2.0f && foe.LastHit.Weapon == 20 && foe.LastHit.Hitter == 10, "hit parameters");
    Check(a.FrameUpdatePauseFrames == 7 && a.HitThing && a.FramesRunCalls == 1, "pause and frames");
    a.Process(0.0f, prev, cur, foe);
    Check(foe.Hits == 1 && tc.Calls == 1, "already struck");
    CThing other = { 50, false, &tc }; other.Hits = 0;
    a.FrameUpdatePauseFrames = 0;
    CVecPair same = { { 0, 0, 0 }, { 0, 0, 1 } };
    a.Process(0.0f, prev, same, other);
    Check(other.Hits == 1 && other.LastHit.Dir.X == 0.0f && other.LastHit.Dir.Z == 0.0f, "zero sweep not normalised");
    Check(a.FrameUpdatePauseFrames == 0, "live other target, already hit: no pause");
    tc.Result = 0;
    CThing miss = { 60, false, &tc }; miss.Hits = 0;
    a.Process(0.0f, prev, cur, miss);
    Check(miss.Hits == 0 && a.Struck.size() == 2, "miss");
    if (!g_fail) std::printf("00858590_TEST PASS\n");
    return g_fail;
}