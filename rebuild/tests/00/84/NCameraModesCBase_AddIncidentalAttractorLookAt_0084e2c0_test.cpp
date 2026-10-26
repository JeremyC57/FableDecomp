// Behaviour test for NCameraModes::CBase::AddIncidentalAttractorLookAt (0x0084E2C0): the attractor is appended only when
// the line-of-sight test from the go-to position succeeds; the LOS call gets test type 7 and flag true.
#include <cstdio>
#include <list>
#include <utility>
class C3DVector { public: float X, Y, Z; };
class CThing;
static int g_losCalls = 0;
struct CTools {
    bool Clear;
    bool IsLineOfSight(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, float& distance,
                       const CThing** hitThing, int test, bool flag) const
    {
        ++g_losCalls;
        if (*hitThing != 0 || test != 7 || !flag || from.X != 1.0f || to.X != 5.0f) return false;
        distance = 4.0f;
        hitPos = to;
        hitNormal = from;
        return Clear;
    }
};
struct CGameCameraManagerView { CTools Tools; };
struct CPhysicsState {
    C3DVector Position;
    std::list<std::pair<C3DVector, float> > IncidentalAttractors;
};
struct CBase {
    CPhysicsState LookAtPhysics;
    CGameCameraManagerView* CameraManager;
    CPhysicsState GoToPhysics;
    void AddIncidentalAttractorLookAt(const C3DVector& pos, float weight)
    {
        CGameCameraManagerView* manager = CameraManager;
        C3DVector hitPos;
        C3DVector hitNormal;
        float distance;
        const CThing* hitThing = 0;
        if (manager->Tools.IsLineOfSight(GoToPhysics.Position, pos, hitPos, hitNormal, distance, &hitThing, 7, true))
            LookAtPhysics.IncidentalAttractors.push_back(std::make_pair(pos, weight));
    }
};

int main()
{
    CGameCameraManagerView manager;
    manager.Tools.Clear = false;
    CBase mode;
    mode.CameraManager = &manager;
    mode.GoToPhysics.Position.X = 1.0f;
    C3DVector target = { 5.0f, 6.0f, 7.0f };
    mode.AddIncidentalAttractorLookAt(target, 0.25f);          // blocked: nothing added
    if (g_losCalls != 1 || !mode.LookAtPhysics.IncidentalAttractors.empty()) { std::printf("0084e2c0_TEST FAIL\n"); return 1; }
    manager.Tools.Clear = true;
    mode.AddIncidentalAttractorLookAt(target, 0.25f);
    if (mode.LookAtPhysics.IncidentalAttractors.size() != 1) { std::printf("0084e2c0_TEST FAIL\n"); return 1; }
    const std::pair<C3DVector, float>& a = mode.LookAtPhysics.IncidentalAttractors.back();
    if (a.first.Y != 6.0f || a.first.Z != 7.0f || a.second != 0.25f) { std::printf("0084e2c0_TEST FAIL\n"); return 1; }
    std::printf("0084e2c0_TEST PASS\n");
    return 0;
}