// Behaviour test for 0x0052EBB0 NUISystem::CComponent::ChangeZoom: a timed change records target/initial and restarts the timer, an instant change sets all three
#include <cstdio>
struct C2DVector { float X; float Y; };
struct CComponent {
    unsigned char _pad[0x5c];
    C2DVector Zoom;
    C2DVector TargetZoom;
    C2DVector InitialZoom;
    unsigned char _pad74[0xa0 - 0x74];
    float ZoomTimeElapsed;
    float ZoomTime;
    void ChangeZoom(const C2DVector& zoom, float time, bool unused);
};
void CComponent::ChangeZoom(const C2DVector& zoom, float time, bool unused)
{
    ZoomTime = time;
    if (time > 0.0f) {
        TargetZoom = zoom;
        InitialZoom = Zoom;
        ZoomTimeElapsed = 0.0f;
    } else {
        TargetZoom = zoom;
        InitialZoom = zoom;
        Zoom = zoom;
    }
}
int main()
{
    CComponent c;
    c.Zoom.X = 1.0f; c.Zoom.Y = 2.0f;
    c.ZoomTimeElapsed = 5.0f;
    C2DVector z; z.X = 3.0f; z.Y = 4.0f;
    c.ChangeZoom(z, 2.0f, false);
    if (c.ZoomTime != 2.0f || c.TargetZoom.X != 3.0f || c.TargetZoom.Y != 4.0f || c.InitialZoom.X != 1.0f ||
        c.InitialZoom.Y != 2.0f || c.ZoomTimeElapsed != 0.0f || c.Zoom.X != 1.0f) { std::printf("0052ebb0_TEST FAIL\n"); return 1; }
    c.ZoomTimeElapsed = 7.0f;
    z.X = 9.0f; z.Y = 8.0f;
    c.ChangeZoom(z, 0.0f, true);
    if (c.ZoomTime != 0.0f || c.Zoom.X != 9.0f || c.Zoom.Y != 8.0f || c.InitialZoom.X != 9.0f || c.TargetZoom.Y != 8.0f ||
        c.ZoomTimeElapsed != 7.0f) { std::printf("0052ebb0_TEST FAIL\n"); return 1; }
    std::printf("0052ebb0_TEST PASS\n");
    return 0;
}