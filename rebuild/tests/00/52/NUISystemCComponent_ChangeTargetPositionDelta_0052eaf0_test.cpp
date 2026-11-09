// Behaviour test for NUISystem::CComponent::ChangeTargetPositionDelta 0x0052EAF0: the delta is un-converted from back-buffer
// units and scaled by the UI scale only for a component using relative coordinates, then added to the TARGET position
// and handed to ChangePosition with the time and the immediate flag (adapted from the landed 0x0052EA30 twin's test).
#include <cstdio>
#include <cmath>

class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X), Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X + vector.X, Y + vector.Y); }
};

static float gBackBufferWidth = 1280.0f;
static float gBackBufferHeight = 960.0f;
static float gScreenWidth = 640.0f;
static float gScreenHeight = 480.0f;
static C2DVector gUIScale(2.0f, 3.0f);

class CUIScreenUnitConverter {
public:
    static C2DVector UnConvertVector(C2DVector v)
    {
        return C2DVector(v.X / gBackBufferWidth * gScreenWidth, v.Y / gBackBufferHeight * gScreenHeight);
    }
};

class CManager {
public:
    static CManager* GetInstance() { static CManager instance; return &instance; }
    C2DVector GetUIScale() const { return gUIScale; }
};

class CComponent {
public:
    C2DVector Position;
    C2DVector TargetPosition;
    bool Relative;
    C2DVector LastPosition;
    float LastTime;
    bool LastImmediate;
    int Calls;
    CComponent(float x, float y, bool relative) : Position(-1.0f, -1.0f), TargetPosition(x, y), Relative(relative), LastPosition(0.0f, 0.0f), LastTime(0.0f), LastImmediate(false), Calls(0) { }
    virtual bool IsUsingRelativePosition() const { return Relative; }
    virtual void ChangePosition(const C2DVector& position, float time, bool immediate)
    {
        LastPosition = position; LastTime = time; LastImmediate = immediate; ++Calls;
    }
    void ChangeTargetPositionDelta(const C2DVector& delta, float time, bool immediate)
    {
        C2DVector d = delta;
        if (IsUsingRelativePosition()) {
            d = CUIScreenUnitConverter::UnConvertVector(d);
            d.X *= CManager::GetInstance()->GetUIScale().X;
            d.Y *= CManager::GetInstance()->GetUIScale().Y;
        }
        ChangePosition(d + TargetPosition, time, immediate);
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    bool ok = true;
    // absolute component: the delta is added as given
    CComponent a(10.0f, 20.0f, false);
    a.ChangeTargetPositionDelta(C2DVector(1.5f, -2.5f), 0.25f, true);
    if (a.Calls != 1 || !Near(a.LastPosition.X, 11.5f) || !Near(a.LastPosition.Y, 17.5f) || !Near(a.LastTime, 0.25f) || !a.LastImmediate) ok = false;
    // relative component: 1280 -> 640 wide (x0.5), 960 -> 480 high (x0.5), then the UI scale (2, 3)
    CComponent r(100.0f, 200.0f, true);
    r.ChangeTargetPositionDelta(C2DVector(64.0f, 48.0f), 0.0f, false);
    // X: 64 / 1280 * 640 = 32, * 2 = 64 ; Y: 48 / 960 * 480 = 24, * 3 = 72
    if (r.Calls != 1 || !Near(r.LastPosition.X, 164.0f) || !Near(r.LastPosition.Y, 272.0f) || !Near(r.LastTime, 0.0f) || r.LastImmediate) ok = false;
    // the stored position itself is left to ChangePosition
    if (!Near(r.TargetPosition.X, 100.0f) || !Near(r.TargetPosition.Y, 200.0f) || !Near(r.Position.X, -1.0f)) ok = false;
    if (!ok) { std::printf("0052eaf0_TEST FAIL\n"); return 1; }
    std::printf("0052eaf0_TEST PASS\n");
    return 0;
}