// Test: CMurderTwistAssassin::IsFeelingThreatened -- true only when the player threatens Me and the hero is within ThreatDistSq (squared distance, inclusive); always false once the thread is terminating; the terminating check runs on every path.
#include <cstdio>
static int g_termChecks = 0;
class CThing { public: float x; };
class CGame { public: CThing hero; CThing& GetHero() { return hero; } };
static int g_threat = 0;
static int IsPlayerThreateningEntity(const CThing&) { return g_threat; }
static float GetSquaredDistanceBetweenThings(const CThing& a, const CThing& b) { float d = a.x - b.x; return d * d; }
struct CAssassin { CGame* Game; CThing Me; float ThreatDistSq; bool terminating;
    bool IsActiveThreadTerminating() const { ++g_termChecks; return terminating; }
    bool IsFeelingThreatened() {
        if (IsPlayerThreateningEntity(Me) && GetSquaredDistanceBetweenThings(Game->GetHero(), Me) <= ThreatDistSq) {
            if (IsActiveThreadTerminating()) return false; return true; }
        if (IsActiveThreadTerminating()) return false; return false; } };
int main()
{
    CGame g; g.hero.x = 3.0f; CAssassin a; a.Game = &g; a.Me.x = 0.0f; a.ThreatDistSq = 9.0f; a.terminating = false;
    g_threat = 1; bool ok = a.IsFeelingThreatened();                    // distance^2 == 9, inclusive
    a.ThreatDistSq = 8.0f; ok = ok && !a.IsFeelingThreatened();        // too far
    a.ThreatDistSq = 100.0f; g_threat = 0; ok = ok && !a.IsFeelingThreatened();   // not threatening
    g_threat = 1; a.terminating = true; ok = ok && !a.IsFeelingThreatened();       // terminating
    ok = ok && g_termChecks == 4;
    std::printf(ok ? "00eb4710_TEST PASS\n" : "00eb4710_TEST FAIL\n");
    return ok ? 0 : 1;
}