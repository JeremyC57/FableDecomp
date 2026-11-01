// Behaviour test for CTCMeleeTrap::ProcessPossibleStrike @ 0x00788617: with hit locations on the thing, the triangle
// (current pair, previous point) is tested; on a hit the thing is hit along the (normalised) trail direction with the
// trap damage, and remembered in HitThings for 1.5 seconds of frames; no hit locations or no intersection: nothing.
#include <cstdio>
#include <cmath>
#include <vector>
namespace model {
const float EPSILON = 1e-4f;
long g_framesPerSecond = 30;
struct C3DVector { float X, Y, Z; C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
    void AccurateNormalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; } };
struct CPointPair { C3DVector first, second; };
struct CHitLocation { int id; };
struct CHitParameters { CHitLocation* loc; C3DVector pos, dir; float damage; bool knockDown;
    CHitParameters(CHitLocation* l, const C3DVector& p, const C3DVector& d, bool k) : loc(l), pos(p), dir(d), damage(0), knockDown(k) {}
    void AddDamageWithAttribute(int, float value) { if (value > -EPSILON) damage = value; } };
struct CThing {
    bool hasHitLocations; CHitLocation location; std::vector<CHitParameters> hits;
    CHitLocation* Intersect(const std::vector<C3DVector>& tri, C3DVector& hitA) { if (tri.size() != 3 || tri[0].X > 10.0f) return 0; hitA = tri[0]; return &location; }
    void OnHit(CHitParameters& h) { hits.push_back(h); }
};
struct CTCMeleeTrap {
    std::vector<std::pair<CThing*, long> > HitThings;
    void ProcessPossibleStrike(float, float damage, const CPointPair& previous, const CPointPair& current, CThing& thing)
    {
        if (!thing.hasHitLocations) return;
        CHitLocation* hitLocation; C3DVector hitPosition;
        { std::vector<C3DVector> triangle; triangle.push_back(current.first); triangle.push_back(current.second); triangle.push_back(previous.first);
          hitLocation = thing.Intersect(triangle, hitPosition); }
        if (hitLocation != 0) {
            C3DVector direction(current.first.X - previous.first.X, current.first.Y - previous.first.Y, current.first.Z - previous.first.Z);
            float lengthSquared = direction.X * direction.X + direction.Y * direction.Y + direction.Z * direction.Z;
            if (lengthSquared > EPSILON * EPSILON) direction.AccurateNormalise();
            CHitParameters hit(hitLocation, hitPosition, direction, true);
            hit.AddDamageWithAttribute(0, damage);
            thing.OnHit(hit);
            float frames = (float)g_framesPerSecond * 1.5f;
            HitThings.push_back(std::pair<CThing*, long>(&thing, (long)frames));
        }
    }
};
}
int main()
{
    using namespace model;
    CTCMeleeTrap trap; CThing thing; thing.hasHitLocations = true;
    CPointPair prev, cur; prev.first = C3DVector(0, 0, 0); prev.second = C3DVector(0, 1, 0); cur.first = C3DVector(3, 4, 0); cur.second = C3DVector(3, 5, 0);
    trap.ProcessPossibleStrike(0.25f, 7.5f, prev, cur, thing);
    bool ok = thing.hits.size() == 1 && trap.HitThings.size() == 1 && trap.HitThings[0].second == 45 && trap.HitThings[0].first == &thing;
    if (ok) { const CHitParameters& h = thing.hits[0]; ok = h.damage == 7.5f && h.knockDown && h.pos.X == 3.0f && std::fabs(h.dir.X - 0.6f) < 1e-5f && std::fabs(h.dir.Y - 0.8f) < 1e-5f; }
    CThing none; none.hasHitLocations = false; trap.ProcessPossibleStrike(0.25f, 7.5f, prev, cur, none);
    CThing far; far.hasHitLocations = true; CPointPair farCur = cur; farCur.first.X = 20.0f; trap.ProcessPossibleStrike(0.25f, 7.5f, prev, farCur, far);
    if (!none.hits.empty() || !far.hits.empty() || trap.HitThings.size() != 1) ok = false;
    if (!ok) { std::printf("00788617_TEST FAIL\n"); return 1; }
    std::printf("00788617_TEST PASS\n");
    return 0;
}