#include <cstdio>
struct C2DVector { float X, Y; C2DVector() { } C2DVector(float x, float y) : X(x), Y(y) { } };
struct CComp {
    virtual void ChangeZoom(const C2DVector& z, long t, long f) { got = z; gt = t; gf = f; }
    virtual void ChangeZoomDelta(const C2DVector& d, long t, long f) { ChangeZoom(C2DVector(Zoom.X + d.X, Zoom.Y + d.Y), t, f); }
    C2DVector Zoom; C2DVector got; long gt, gf;
};
int main()
{
    CComp c; c.Zoom = C2DVector(1.0f, 2.0f); c.ChangeZoomDelta(C2DVector(0.5f, -1.0f), 30, 7);
    if (c.got.X != 1.5f || c.got.Y != 1.0f || c.gt != 30 || c.gf != 7) { std::printf("0052ec20_TEST FAIL\n"); return 1; }
    std::printf("0052ec20_TEST PASS\n");
    return 0;
}