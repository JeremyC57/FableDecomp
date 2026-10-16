#include <cstdio>
// Behaviour model of CTCPhysicsStandard::UpdateOnSurface (0x008E55E0)
struct CTCPhysicsStandard {
    bool OnMesh, OnGround; void* standingOn; bool resMesh, resGround; void* resOn;
    void GetOnSurfaceParameters(bool& onMesh, bool& onGround, void*& on) { onMesh = resMesh; onGround = resGround; on = resOn; }
    void SetThingStandingOn(void* p) { standingOn = p; }
    void UpdateOnSurface() {
        bool onMesh = OnMesh; bool onGround = OnGround; void* on = 0;
        GetOnSurfaceParameters(onMesh, onGround, on);
        OnMesh = onMesh; OnGround = onGround;
        SetThingStandingOn(OnGround ? 0 : on);
    } };
int main()
{
    int t;
    CTCPhysicsStandard a = { false, false, 0, true, false, &t };
    a.UpdateOnSurface();
    if (!a.OnMesh || a.OnGround || a.standingOn != &t) { std::printf("008e55e0_TEST FAIL\n"); return 1; }
    CTCPhysicsStandard b = { false, false, &t, false, true, &t };
    b.UpdateOnSurface();
    if (b.OnMesh || !b.OnGround || b.standingOn) { std::printf("008e55e0_TEST FAIL\n"); return 1; }
    std::printf("008e55e0_TEST PASS\n");
    return 0;
}
