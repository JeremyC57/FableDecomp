#include <cstdio>
class C2DVector { public: float X; float Y; };
struct CComponent {
    C2DVector Zoom, TargetZoom, InitialZoom;
    void SetZoom(const C2DVector& zoom) { TargetZoom = zoom; InitialZoom = zoom; Zoom = zoom; }
};
int main()
{
    CComponent c;
    C2DVector z0 = { 0.0f, 0.0f };
    c.Zoom = z0; c.TargetZoom = z0; c.InitialZoom = z0;
    C2DVector z = { 1.5f, 2.5f };
    c.SetZoom(z);
    if (c.Zoom.X != 1.5f || c.Zoom.Y != 2.5f || c.TargetZoom.X != 1.5f || c.TargetZoom.Y != 2.5f ||
        c.InitialZoom.X != 1.5f || c.InitialZoom.Y != 2.5f) { std::printf("0052e750_TEST FAIL\n"); return 1; }
    c.SetZoom(c.TargetZoom);   /* aliasing argument */
    if (c.Zoom.X != 1.5f || c.InitialZoom.Y != 2.5f) { std::printf("0052e750_TEST FAIL\n"); return 1; }
    std::printf("0052e750_TEST PASS\n");
    return 0;
}