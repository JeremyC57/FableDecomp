// Behaviour test for 0x00547050 NUISystem::CScrollingComponent::UpdatePosition(float): a model of the row over a stub
// component; checks the linear scroll (position = (target - initial) / time * elapsed + initial), the snap once the
// elapsed time reaches the move time (position and initial := target), no move once the time is up, and the render
// position (the position, scaled by the parent zoom and offset by the parent position unless IsIndependent()).
#include <cstdio>
#include <cmath>
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector operator/(float s) const { float inv = 1.0f / s; return C2DVector(X * inv, Y * inv); }
    const C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
};
struct CScrollingComponent {
    bool Independent; int IndependentCalls;
    C2DVector Position, TargetPosition, InitialPosition, ParentPosition, RenderPosition, ParentZoom;
    float PositionTimeElapsed, PositionTime;
    bool IsIndependent() { ++IndependentCalls; return Independent; }
    void UpdatePosition(float deltaTime)
    {
        if (PositionTimeElapsed < PositionTime) {
            PositionTimeElapsed += deltaTime;
            C2DVector velocity;
            velocity = (TargetPosition - InitialPosition) / PositionTime;
            Position = velocity * PositionTimeElapsed + InitialPosition;
            if (PositionTimeElapsed >= PositionTime)
                InitialPosition = Position = TargetPosition;
        }
        RenderPosition = Position;
        if (!IsIndependent()) {
            C2DVector& render = RenderPosition;
            render.X *= ParentZoom.X;
            render.Y *= ParentZoom.Y;
            render += ParentPosition;
        }
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    int ok = 1;
    CScrollingComponent c;
    c.Independent = false; c.IndependentCalls = 0;
    c.Position = C2DVector(0, 0); c.InitialPosition = C2DVector(0, 0); c.TargetPosition = C2DVector(10, 20);
    c.ParentPosition = C2DVector(1, 2); c.ParentZoom = C2DVector(2, 3);
    c.PositionTimeElapsed = 0; c.PositionTime = 4;
    c.UpdatePosition(1.0f);
    ok &= near(c.Position.X, 2.5f) && near(c.Position.Y, 5.0f);
    ok &= near(c.RenderPosition.X, 2.5f * 2 + 1) && near(c.RenderPosition.Y, 5.0f * 3 + 2);
    ok &= near(c.InitialPosition.X, 0) && c.IndependentCalls == 1;
    c.UpdatePosition(5.0f);                                   // past the end: snap
    ok &= near(c.Position.X, 10) && near(c.Position.Y, 20) && near(c.InitialPosition.X, 10) && near(c.InitialPosition.Y, 20);
    c.Independent = true; c.TargetPosition = C2DVector(-1, -1);
    c.UpdatePosition(1.0f);                                   // time is up: no move, render = position
    ok &= near(c.Position.X, 10) && near(c.RenderPosition.X, 10) && near(c.RenderPosition.Y, 20) && c.IndependentCalls == 3;
    std::printf(ok ? "00547050_TEST PASS\n" : "00547050_TEST FAIL\n");
    return ok ? 0 : 1;
}