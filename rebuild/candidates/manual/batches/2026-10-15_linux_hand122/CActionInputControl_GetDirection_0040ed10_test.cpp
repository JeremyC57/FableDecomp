// Behaviour test for CActionInputControl::GetDirection (0x0040ED10).
#include <cstdio>

const float g_floatEpsilon = 0.0001f;
const float g_floatZero = 0.0f;

struct C2DVector { float X, Y; };

struct CActionInputControl {
    long GameAction; long ControllerType; long KeyboardKey; long XboxButton; long MouseButton;
    C2DVector ControlDirection;
    long GetDirection() const
    {
        if (ControlDirection.X * ControlDirection.X + ControlDirection.Y * ControlDirection.Y
                > g_floatEpsilon * g_floatEpsilon
            && (GameAction == 0x3C || GameAction == 0x72)) {
            if (ControlDirection.X > g_floatZero) return 4;
            if (ControlDirection.X < g_floatZero) return 3;
            if (ControlDirection.Y > g_floatZero) return 1;
            if (ControlDirection.Y < g_floatZero) return 2;
        }
        return 0;
    }
};

static long Dir(long action, float x, float y)
{
    CActionInputControl c; c.GameAction = action; c.ControllerType = c.KeyboardKey = c.XboxButton = c.MouseButton = 0;
    c.ControlDirection.X = x; c.ControlDirection.Y = y;
    return c.GetDirection();
}

int main()
{
    if (Dir(0x3C, 1.0f, 0.0f) != 4 || Dir(0x72, -1.0f, 5.0f) != 3 || Dir(0x3C, 0.0f, 2.0f) != 1
        || Dir(0x3C, 0.0f, -2.0f) != 2 || Dir(0x10, 1.0f, 0.0f) != 0 || Dir(0x3C, 0.00001f, 0.0f) != 0) {
        std::printf("0040ed10_TEST FAIL\n");
        return 1;
    }
    std::printf("0040ed10_TEST PASS\n");
    return 0;
}
