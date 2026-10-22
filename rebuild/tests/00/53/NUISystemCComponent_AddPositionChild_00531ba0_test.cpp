#include <cstdio>
// Behaviour test for NUISystem::CComponent::AddPositionChild @ 0x00531BA0: the child is added to the set, gets this
// component as its position parent, the two render zooms, and a parent / relative parent position chosen by the three
// virtual predicates (relative root, zoom ignored, screen converted). Same C2DVector forms as the reconstruction.
struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
    C2DVector& operator*=(const C2DVector& o) { X *= o.X; Y *= o.Y; return *this; }
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
static C2DVector ConvertToScreen(C2DVector position) { return C2DVector(position.X * 2.0f + 1.0f, position.Y * 2.0f + 3.0f); }

struct Comp {
    bool relative, zoomIgnored, screen;
    int childCount;
    Comp* positionParent;
    C2DVector Position, ParentPosition, ParentZoom, RenderZoom;
    C2DVector RelativeParentPosition, RelativeRenderZoom, RelativeParentZoom;
    C2DVector gotParentPosition, gotParentZoom, gotRelativeParentPosition, gotRelativeParentZoom;
    bool IsPositionParentRelative() const { return relative; }
    bool IsZoomIgnored() const { return zoomIgnored; }
    bool IsScreenConverted() const { return screen; }
    void SetPositionParent(Comp* p) { positionParent = p; }
    void SetParentZoom(const C2DVector& v) { gotParentZoom = v; }
    void SetRelativeParentZoom(const C2DVector& v) { gotRelativeParentZoom = v; }
    void SetParentPosition(const C2DVector& v) { gotParentPosition = v; }
    void SetRelativeParentPosition(const C2DVector& v) { gotRelativeParentPosition = v; }
    void AddPositionChild(Comp* child)
    {
        ++childCount;
        child->SetPositionParent(this);
        child->SetParentZoom(RenderZoom);
        child->SetRelativeParentZoom(RelativeRenderZoom);
        if (IsPositionParentRelative()) {
            if (IsZoomIgnored()) {
                child->SetParentPosition(Position);
            } else {
                C2DVector position = Position;
                position *= ParentZoom;
                child->SetParentPosition(position);
            }
            if (IsScreenConverted()) {
                if (IsZoomIgnored()) {
                    C2DVector screen = ConvertToScreen(Position);
                    child->SetRelativeParentPosition(screen);
                } else {
                    C2DVector screen = ConvertToScreen(Position);
                    screen *= RelativeParentZoom;
                    child->SetRelativeParentPosition(screen);
                }
            } else {
                if (IsZoomIgnored()) {
                    child->SetRelativeParentPosition(Position);
                } else {
                    C2DVector position = Position;
                    position *= RelativeParentZoom;
                    child->SetRelativeParentPosition(position);
                }
            }
        } else {
            if (IsZoomIgnored()) {
                child->SetParentPosition(ParentPosition + Position);
            } else {
                C2DVector position = Position;
                position *= ParentZoom;
                position += ParentPosition;
                child->SetParentPosition(position);
            }
            if (IsScreenConverted()) {
                if (IsZoomIgnored()) {
                    child->SetRelativeParentPosition(ConvertToScreen(Position) + RelativeParentPosition);
                } else {
                    C2DVector screen = ConvertToScreen(Position);
                    screen *= RelativeParentZoom;
                    screen += RelativeParentPosition;
                    child->SetRelativeParentPosition(screen);
                }
            } else {
                if (IsZoomIgnored()) {
                    child->SetRelativeParentPosition(C2DVector(RelativeParentPosition.X + Position.X, RelativeParentPosition.Y + Position.Y));
                } else {
                    C2DVector position = Position;
                    position *= RelativeParentZoom;
                    position += RelativeParentPosition;
                    child->SetRelativeParentPosition(position);
                }
            }
        }
    }
};

static bool Same(const C2DVector& v, float x, float y) { return v.X == x && v.Y == y; }
static int Fail(int n) { std::printf("00531ba0_TEST FAIL %d\n", n); return 1; }

int main()
{
    // expected parent position (px,py) and relative parent position (rx,ry) per (relative, zoomIgnored, screen)
    // Position (3,5), ParentPosition (10,20), ParentZoom (2,4), RelativeParentPosition (100,200), RelativeParentZoom (3,0.5)
    // screen(Position) = (7,13)
    static const float expect[8][4] = {
        { 6.0f + 10.0f, 20.0f + 20.0f, 9.0f + 100.0f, 2.5f + 200.0f },     // root, zoomed, plain
        { 6.0f + 10.0f, 20.0f + 20.0f, 21.0f + 100.0f, 6.5f + 200.0f },    // root, zoomed, screen
        { 13.0f, 25.0f, 103.0f, 205.0f },                                  // root, zoom ignored, plain
        { 13.0f, 25.0f, 107.0f, 213.0f },                                  // root, zoom ignored, screen
        { 6.0f, 20.0f, 9.0f, 2.5f },                                       // relative, zoomed, plain
        { 6.0f, 20.0f, 21.0f, 6.5f },                                      // relative, zoomed, screen
        { 3.0f, 5.0f, 3.0f, 5.0f },                                        // relative, zoom ignored, plain
        { 3.0f, 5.0f, 7.0f, 13.0f },                                       // relative, zoom ignored, screen
    };
    for (int i = 0; i < 8; ++i) {
        Comp parent;
        parent.relative = (i & 4) != 0; parent.zoomIgnored = (i & 2) != 0; parent.screen = (i & 1) != 0;
        parent.childCount = 0; parent.positionParent = 0;
        parent.Position = C2DVector(3.0f, 5.0f);
        parent.ParentPosition = C2DVector(10.0f, 20.0f);
        parent.ParentZoom = C2DVector(2.0f, 4.0f);
        parent.RenderZoom = C2DVector(1.5f, 2.5f);
        parent.RelativeParentPosition = C2DVector(100.0f, 200.0f);
        parent.RelativeRenderZoom = C2DVector(0.25f, 0.75f);
        parent.RelativeParentZoom = C2DVector(3.0f, 0.5f);
        Comp child;
        child.childCount = 0; child.positionParent = 0;
        child.gotParentPosition = child.gotParentZoom = child.gotRelativeParentPosition = child.gotRelativeParentZoom = C2DVector(-1.0f, -1.0f);
        parent.AddPositionChild(&child);
        if (parent.childCount != 1 || child.positionParent != &parent) return Fail(100 + i);
        if (!Same(child.gotParentZoom, 1.5f, 2.5f) || !Same(child.gotRelativeParentZoom, 0.25f, 0.75f)) return Fail(200 + i);
        if (!Same(child.gotParentPosition, expect[i][0], expect[i][1])) return Fail(300 + i);
        if (!Same(child.gotRelativeParentPosition, expect[i][2], expect[i][3])) return Fail(400 + i);
    }
    std::printf("00531ba0_TEST PASS\n");
    return 0;
}