#include <cstdio>
#include <math.h>
// Behaviour test for NUISystem::CComponent::UpdateZoom @ 0x0052F5C0: the zoom eases from the initial to the target zoom
// (s = a t^2 / 2 + v t, a = -2 d / T^2, v = 2 d / T) and snaps to the target (initial too) once the elapsed time reaches
// ZoomTime; the render zoom is the zoom times the parent zoom (or the bare zoom when parent zoom is ignored), the
// relative render zoom the same with the relative parent zoom and an optional screen conversion, and a root or
// zoom-ignoring component applies the UI scale to both. Same C2DVector forms as the reconstruction.
struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator*(float f) const { return C2DVector(X * f, Y * f); }
    C2DVector operator/(float f) const { return *this * (1.0f / f); }
};

struct Converter {
    static bool relativeCoords;
    static float backWidth, backHeight, screenWidth, screenHeight;
    static float Width(float x) { if (relativeCoords) return x / screenWidth * backWidth; return x; }
    static float Height(float y) { if (relativeCoords) return y / screenHeight * backHeight; return y; }
    static C2DVector Vector(C2DVector v) { return C2DVector(Width(v.X), Height(v.Y)); }
};
bool Converter::relativeCoords = true;
float Converter::backWidth = 1280.0f;
float Converter::backHeight = 960.0f;
float Converter::screenWidth = 640.0f;
float Converter::screenHeight = 480.0f;
static C2DVector g_uiScale(2.0f, 4.0f);

struct Comp {
    bool zoomIgnored, screenConverted;
    Comp* PParent;
    C2DVector Zoom, TargetZoom, InitialZoom, ParentZoom, RenderZoom, RelativeRenderZoom, RelativeParentZoom;
    float ZoomTimeElapsed, ZoomTime;
    bool IsZoomIgnored() const { return zoomIgnored; }
    bool IsZoomScreenConverted() const { return screenConverted; }
    void UpdateZoom(float deltaTime)
    {
        if (ZoomTimeElapsed < ZoomTime) {
            ZoomTimeElapsed += deltaTime;
            C2DVector acceleration;
            C2DVector velocity;
            acceleration = (TargetZoom - InitialZoom) * -2.0f / (float)pow(ZoomTime, 2);
            velocity = (TargetZoom - InitialZoom) * 2.0f / ZoomTime;
            Zoom = acceleration * 0.5f * (float)pow(ZoomTimeElapsed, 2) + velocity * ZoomTimeElapsed + InitialZoom;
            if (ZoomTimeElapsed >= ZoomTime) {
                Zoom = TargetZoom;
                InitialZoom = Zoom;
            }
        }
        if (!IsZoomIgnored()) {
            RenderZoom.X = Zoom.X * ParentZoom.X;
            RenderZoom.Y = Zoom.Y * ParentZoom.Y;
            if (IsZoomScreenConverted()) {
                RelativeRenderZoom.X = Converter::Width(Zoom.X) * RelativeParentZoom.X;
                RelativeRenderZoom.Y = Converter::Height(Zoom.Y) * RelativeParentZoom.Y;
            } else {
                RelativeRenderZoom.X = Zoom.X * RelativeParentZoom.X;
                RelativeRenderZoom.Y = Zoom.Y * RelativeParentZoom.Y;
            }
        } else {
            RenderZoom = Zoom;
            if (IsZoomScreenConverted())
                RelativeRenderZoom = Converter::Vector(Zoom);
            else
                RelativeRenderZoom = Zoom;
        }
        if (!PParent || IsZoomIgnored()) {
            RelativeRenderZoom.X *= g_uiScale.X;
            RelativeRenderZoom.Y *= g_uiScale.Y;
            RenderZoom.X *= g_uiScale.X;
            RenderZoom.Y *= g_uiScale.Y;
        }
    }
};

static bool Same(const C2DVector& v, float x, float y) { return v.X == x && v.Y == y; }
static int Fail(int n) { std::printf("0052f5c0_TEST FAIL %d\n", n); return 1; }

static void Reset(Comp& c, Comp* parent, bool ignored, bool screen)
{
    c.zoomIgnored = ignored; c.screenConverted = screen; c.PParent = parent;
    c.Zoom = C2DVector(1.0f, 1.0f);
    c.TargetZoom = C2DVector(3.0f, 5.0f);
    c.InitialZoom = C2DVector(1.0f, 1.0f);
    c.ParentZoom = C2DVector(2.0f, 0.5f);
    c.RelativeParentZoom = C2DVector(4.0f, 0.25f);
    c.RenderZoom = c.RelativeRenderZoom = C2DVector(-1.0f, -1.0f);
    c.ZoomTimeElapsed = 0.0f; c.ZoomTime = 2.0f;
}

int main()
{
    Comp parent;
    Reset(parent, 0, false, false);
    // mid-transition: d = (2,4), T = 2, t = 1 -> a = (-1,-2), v = (2,4), zoom = a/2 + v + initial = (2.5, 4)
    Comp c;
    Reset(c, &parent, false, false);
    c.UpdateZoom(1.0f);
    if (c.ZoomTimeElapsed != 1.0f || !Same(c.Zoom, 2.5f, 4.0f) || !Same(c.InitialZoom, 1.0f, 1.0f)) return Fail(1);
    if (!Same(c.RenderZoom, 5.0f, 2.0f) || !Same(c.RelativeRenderZoom, 10.0f, 1.0f)) return Fail(2);
    // end of the transition: snap to the target, initial follows
    c.UpdateZoom(1.5f);
    if (c.ZoomTimeElapsed != 2.5f || !Same(c.Zoom, 3.0f, 5.0f) || !Same(c.InitialZoom, 3.0f, 5.0f)) return Fail(3);
    // finished: the zoom no longer moves
    c.TargetZoom = C2DVector(9.0f, 9.0f);
    c.UpdateZoom(1.0f);
    if (c.ZoomTimeElapsed != 2.5f || !Same(c.Zoom, 3.0f, 5.0f)) return Fail(4);
    // screen-converted child: relative render zoom is the converted zoom (x2 each axis) times the relative parent zoom
    Reset(c, &parent, false, true);
    c.ZoomTimeElapsed = c.ZoomTime;
    c.UpdateZoom(1.0f);
    if (!Same(c.Zoom, 1.0f, 1.0f) || !Same(c.RenderZoom, 2.0f, 0.5f) || !Same(c.RelativeRenderZoom, 8.0f, 0.5f)) return Fail(5);
    // relative coordinates off: no conversion
    Converter::relativeCoords = false;
    c.UpdateZoom(1.0f);
    if (!Same(c.RelativeRenderZoom, 4.0f, 0.25f)) return Fail(6);
    Converter::relativeCoords = true;
    // zoom ignored (with a parent): bare zoom, then the UI scale (2,4)
    Reset(c, &parent, true, false);
    c.Zoom = C2DVector(1.5f, 2.0f); c.ZoomTimeElapsed = c.ZoomTime;
    c.UpdateZoom(1.0f);
    if (!Same(c.RenderZoom, 3.0f, 8.0f) || !Same(c.RelativeRenderZoom, 3.0f, 8.0f)) return Fail(7);
    // zoom ignored and screen converted: converted zoom (x2) then the UI scale
    Reset(c, &parent, true, true);
    c.Zoom = C2DVector(1.5f, 2.0f); c.ZoomTimeElapsed = c.ZoomTime;
    c.UpdateZoom(1.0f);
    if (!Same(c.RenderZoom, 3.0f, 8.0f) || !Same(c.RelativeRenderZoom, 6.0f, 16.0f)) return Fail(8);
    // root component (no parent), parent zoom used: products, then the UI scale
    Reset(c, 0, false, false);
    c.ZoomTimeElapsed = c.ZoomTime;
    c.UpdateZoom(1.0f);
    if (!Same(c.RenderZoom, 4.0f, 2.0f) || !Same(c.RelativeRenderZoom, 8.0f, 1.0f)) return Fail(9);
    std::printf("0052f5c0_TEST PASS\n");
    return 0;
}
