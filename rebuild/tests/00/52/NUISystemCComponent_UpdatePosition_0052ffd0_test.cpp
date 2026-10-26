// Behaviour test for 0x0052FFD0 NUISystem::CComponent::UpdatePosition(float): runs the reconstructed body over a stub
// component; checks the decelerating move (position = accel/2 * t^2 + vel * t + initial with accel = -2(target -
// initial)/T^2 and vel = 2(target - initial)/T), the snap to the target (and initial := target) once the elapsed time
// reaches the move time, no move once the time is up, the render position (scaled by the parent zoom and offset by the
// parent position unless position-independent) and the saved render position (screen-converted when the component is
// position-scaled, then scaled by the relative render zoom and offset by the relative render position).
#include <cstdio>
#include <math.h>

#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif

static int g_failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++g_failures; } } while (0)

class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector operator/(float s) const { float inv = 1.0f / s; return C2DVector(X * inv, Y * inv); }
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
};
inline void Add(C2DVector& a, const C2DVector& b) { a.X += b.X; a.Y += b.Y; }

static int g_ConvertCalls = 0;
static C2DVector g_ConvertArg(0.0f, 0.0f);
C2DVector __fastcall ConvertToScreen(C2DVector position)
{
    ++g_ConvertCalls;
    g_ConvertArg = position;
    return C2DVector(position.X * 2.0f + 1.0f, position.Y * 3.0f - 1.0f);
}

namespace NUISystem {
class CComponent { public:
    bool Scaled;
    bool Independent;
    int  ScaledCalls;
    int  IndependentCalls;
    virtual ~CComponent() { }
    virtual bool IsPositionIndependent() { ++IndependentCalls; return Independent; }
    virtual bool IsPositionScaled() { ++ScaledCalls; return Scaled; }
    float     Time;
    C2DVector Position;
    C2DVector TargetPosition;
    C2DVector InitialPosition;
    C2DVector ParentPosition;
    C2DVector RenderPosition;
    C2DVector Zoom;
    C2DVector TargetZoom;
    C2DVector InitialZoom;
    C2DVector ParentZoom;
    C2DVector RenderZoom;
    float     PositionTimeElapsed;
    float     PositionTime;
    C2DVector RenderPositionSave;
    C2DVector RelativeRenderPosition;
    C2DVector RelativeParentPosition;
    C2DVector RelativeRenderZoom;
    void UpdatePosition(float dt);
};

void CComponent::UpdatePosition(float dt)
{
    if (PositionTimeElapsed < PositionTime) {
        PositionTimeElapsed += dt;
        C2DVector vel;
        C2DVector accel;
        accel = (TargetPosition - InitialPosition) * -2.0f / pow(PositionTime, 2);
        vel = (TargetPosition - InitialPosition) * 2.0f / PositionTime;
        Position = accel * 0.5f * pow(PositionTimeElapsed, 2) + vel * PositionTimeElapsed + InitialPosition;
        if (PositionTimeElapsed >= PositionTime) {
            Position = TargetPosition;
            InitialPosition = TargetPosition;
        }
    }
    RenderPosition = Position;
    if (IsPositionScaled())
        RenderPositionSave = ConvertToScreen(Position);
    else
        RenderPositionSave = Position;
    if (!IsPositionIndependent()) {
        RenderPosition.X *= ParentZoom.X;
        RenderPosition.Y *= ParentZoom.Y;
        RenderPosition += ParentPosition;
        RenderPositionSave.X *= RelativeRenderZoom.X;
        RenderPositionSave.Y *= RelativeRenderZoom.Y;
        Add(RenderPositionSave, RelativeRenderPosition);
    }
}
}

static bool Eq(const C2DVector& v, float x, float y) { return v.X == x && v.Y == y; }

static void Setup(NUISystem::CComponent& c, bool scaled, bool independent)
{
    c.Scaled = scaled;
    c.Independent = independent;
    c.ScaledCalls = 0;
    c.IndependentCalls = 0;
    c.Time = 0.0f;
    c.Position = C2DVector(2.0f, 4.0f);
    c.InitialPosition = C2DVector(2.0f, 4.0f);
    c.TargetPosition = C2DVector(10.0f, 20.0f);
    c.ParentPosition = C2DVector(100.0f, 200.0f);
    c.ParentZoom = C2DVector(2.0f, 0.5f);
    c.RelativeRenderZoom = C2DVector(3.0f, 4.0f);
    c.RelativeRenderPosition = C2DVector(-1.0f, -2.0f);
    c.RelativeParentPosition = C2DVector(55.0f, 66.0f);
    c.Zoom = C2DVector(1.0f, 1.0f);
    c.RenderZoom = C2DVector(7.0f, 7.0f);
    c.RenderPosition = C2DVector(-9.0f, -9.0f);
    c.RenderPositionSave = C2DVector(-9.0f, -9.0f);
    c.PositionTimeElapsed = 0.0f;
    c.PositionTime = 2.0f;
    g_ConvertCalls = 0;
    g_ConvertArg = C2DVector(0.0f, 0.0f);
}

int main()
{
    NUISystem::CComponent c;

    // Mid-move, not scaled, parent-relative: accel = (8,16)*-2/4 = (-4,-8), vel = (8,16)*2/2 = (8,16);
    // t = 1: position = (-4,-8)*0.5*1 + (8,16)*1 + (2,4) = (8,16).
    Setup(c, false, false);
    c.UpdatePosition(1.0f);
    CHECK(c.PositionTimeElapsed == 1.0f);
    CHECK(Eq(c.Position, 8.0f, 16.0f));
    CHECK(Eq(c.InitialPosition, 2.0f, 4.0f));
    CHECK(Eq(c.RenderPosition, 8.0f * 2.0f + 100.0f, 16.0f * 0.5f + 200.0f));
    CHECK(Eq(c.RenderPositionSave, 8.0f * 3.0f - 1.0f, 16.0f * 4.0f - 2.0f));
    CHECK(g_ConvertCalls == 0);
    CHECK(c.ScaledCalls == 1);
    CHECK(c.IndependentCalls == 1);
    CHECK(Eq(c.RenderZoom, 7.0f, 7.0f));
    CHECK(Eq(c.RelativeParentPosition, 55.0f, 66.0f));

    // t = 0.5: position = (-4,-8)*0.5*0.25 + (8,16)*0.5 + (2,4) = (-0.5,-1) + (4,8) + (2,4) = (5.5,11).
    Setup(c, false, true);
    c.UpdatePosition(0.5f);
    CHECK(c.PositionTimeElapsed == 0.5f);
    CHECK(Eq(c.Position, 5.5f, 11.0f));
    CHECK(Eq(c.RenderPosition, 5.5f, 11.0f));
    CHECK(Eq(c.RenderPositionSave, 5.5f, 11.0f));
    CHECK(c.IndependentCalls == 1);

    // Scaled: the saved position is screen-converted from the (new) position, then made relative.
    Setup(c, true, false);
    c.UpdatePosition(1.0f);
    CHECK(g_ConvertCalls == 1);
    CHECK(Eq(g_ConvertArg, 8.0f, 16.0f));
    CHECK(Eq(c.RenderPositionSave, (8.0f * 2.0f + 1.0f) * 3.0f - 1.0f, (16.0f * 3.0f - 1.0f) * 4.0f - 2.0f));
    CHECK(Eq(c.RenderPosition, 116.0f, 208.0f));

    // Scaled and independent: the saved position is the plain screen conversion.
    Setup(c, true, true);
    c.UpdatePosition(1.0f);
    CHECK(Eq(c.RenderPositionSave, 17.0f, 47.0f));
    CHECK(Eq(c.RenderPosition, 8.0f, 16.0f));

    // Reaching the move time snaps to the target and makes it the new initial position.
    Setup(c, false, true);
    c.PositionTimeElapsed = 1.5f;
    c.UpdatePosition(1.0f);
    CHECK(c.PositionTimeElapsed == 2.5f);
    CHECK(Eq(c.Position, 10.0f, 20.0f));
    CHECK(Eq(c.InitialPosition, 10.0f, 20.0f));
    CHECK(Eq(c.RenderPosition, 10.0f, 20.0f));
    // Exactly at the move time also snaps.
    Setup(c, false, true);
    c.PositionTimeElapsed = 1.0f;
    c.UpdatePosition(1.0f);
    CHECK(Eq(c.Position, 10.0f, 20.0f));
    CHECK(Eq(c.InitialPosition, 10.0f, 20.0f));

    // Time already up: no move, elapsed unchanged; the render positions are still derived.
    Setup(c, false, false);
    c.PositionTimeElapsed = 2.0f;
    c.Position = C2DVector(1.0f, 2.0f);
    c.UpdatePosition(5.0f);
    CHECK(c.PositionTimeElapsed == 2.0f);
    CHECK(Eq(c.Position, 1.0f, 2.0f));
    CHECK(Eq(c.InitialPosition, 2.0f, 4.0f));
    CHECK(Eq(c.RenderPosition, 102.0f, 201.0f));
    CHECK(Eq(c.RenderPositionSave, 2.0f, 6.0f));

    if (g_failures == 0)
        std::printf("0052ffd0_TEST PASS\n");
    return g_failures;
}