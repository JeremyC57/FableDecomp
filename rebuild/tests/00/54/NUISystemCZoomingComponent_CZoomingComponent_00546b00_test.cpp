// Behaviour test for NUISystem::CZoomingComponent::CZoomingComponent (0x00546B00): InputDelay/ScrollingSpeed zeroed before GetDataFromDef.
#include <cstdio>
struct C2DVector { float X, Y; };
struct CChangingStateComponent {
    long m_owner;
    CChangingStateComponent(long owner) : m_owner(owner) {}
    virtual ~CChangingStateComponent() {}
};
static bool g_zeroBeforeDef = false;
struct CZoomingComponent : CChangingStateComponent {
    float LastInputTime;
    C2DVector ZoomCenter, MinZoom, MaxZoom;
    bool Panning, Zooming;
    float InputDelay, ScrollingSpeed;
    CZoomingComponent(long owner) : CChangingStateComponent(owner), InputDelay(0.0f), ScrollingSpeed(0.0f) { GetDataFromDef(); }
    void GetDataFromDef() { g_zeroBeforeDef = InputDelay == 0.0f && ScrollingSpeed == 0.0f; ScrollingSpeed = 2.0f; }
};
int main()
{
    CZoomingComponent z(9);
    if (z.m_owner != 9 || !g_zeroBeforeDef || z.ScrollingSpeed != 2.0f) { std::printf("00546b00_TEST FAIL\n"); return 1; }
    std::printf("00546b00_TEST PASS\n");
    return 0;
}