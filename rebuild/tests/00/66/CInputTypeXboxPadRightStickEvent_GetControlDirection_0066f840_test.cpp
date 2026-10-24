// Behaviour test for 0x0066F840 CInputTypeXboxPadRightStickEvent::GetControlDirection.
#include <cstdio>
#include <cmath>
struct C2DVector { float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} };
struct CInputEvent { C2DVector pos; C2DVector GetAnaloguePosition() const { return pos; } };
static const float g_InputDeadZone = 0.0001f;
static C2DVector GetControlDirection(const CInputEvent& event)
{
    C2DVector position = event.GetAnaloguePosition();
    double y = position.Y * -1.0f;
    double x = position.X;
    float length = (float)std::sqrt(x * x + y * y);
    if (length > g_InputDeadZone && length > 1.0f) {
        float scale = 1.0f / length;
        x *= scale; y *= scale;
    }
    return C2DVector((float)x, (float)y);
}
static bool Near(float a, float b) { return std::fabs(a - b) < 0.0001f; }
int main()
{
    CInputEvent e;
    e.pos = C2DVector(0.3f, 0.4f);        // inside the unit circle: only Y flips
    C2DVector d = GetControlDirection(e);
    if (!Near(d.X, 0.3f) || !Near(d.Y, -0.4f)) { std::printf("0066f840_TEST FAIL\n"); return 1; }
    e.pos = C2DVector(3.0f, 4.0f);        // length 5: normalised
    d = GetControlDirection(e);
    if (!Near(d.X, 0.6f) || !Near(d.Y, -0.8f)) { std::printf("0066f840_TEST FAIL\n"); return 1; }
    std::printf("0066f840_TEST PASS\n");
    return 0;
}
