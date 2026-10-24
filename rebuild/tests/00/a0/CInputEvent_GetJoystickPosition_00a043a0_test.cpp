// Behaviour test for 0x00A043A0 CInputEvent::GetJoystickPosition: returns the helper-transformed stick position by value.
#include <cstdio>
#if !defined(_MSC_VER)
#define __fastcall
#endif

class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

static int g_calls = 0;
static const C2DVector* g_seen = 0;
// test stand-in for the radial -> rectangular transform: clamps each axis to [-1, 1] and doubles it
C2DVector __fastcall TransformControllerInputFromRadiallyToRectangularlyLimited(const C2DVector& input)
{
    ++g_calls;
    g_seen = &input;
    C2DVector r;
    r.X = input.X > 1.0f ? 2.0f : (input.X < -1.0f ? -2.0f : input.X * 2.0f);
    r.Y = input.Y > 1.0f ? 2.0f : (input.Y < -1.0f ? -2.0f : input.Y * 2.0f);
    return r;
}

class CInputEvent {
public:
    C2DVector     JoystickPos;
    unsigned char _data[0x18];
    long          DeviceType;
    long          DeviceNumber;
    long          Type;
    float         CurrentTime;
    float         StartTime;
    C2DVector GetJoystickPosition() const;
};

C2DVector CInputEvent::GetJoystickPosition() const
{
    C2DVector position = TransformControllerInputFromRadiallyToRectangularlyLimited(JoystickPos);
    return position;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    CInputEvent e;
    e.JoystickPos.X = 0.25f; e.JoystickPos.Y = -3.0f;
    C2DVector p = e.GetJoystickPosition();
    if (!Near(p.X, 0.5f) || !Near(p.Y, -2.0f) || g_calls != 1 || g_seen != &e.JoystickPos) { std::printf("00a043a0_TEST FAIL\n"); return 1; }
    e.JoystickPos.X = 5.0f; e.JoystickPos.Y = 0.0f;
    p = e.GetJoystickPosition();
    if (!Near(p.X, 2.0f) || !Near(p.Y, 0.0f) || g_calls != 2) { std::printf("00a043a0_TEST FAIL\n"); return 1; }
    std::printf("00a043a0_TEST PASS\n");
    return 0;
}