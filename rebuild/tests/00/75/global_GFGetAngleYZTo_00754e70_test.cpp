// Behaviour test for 0x00754E70 GFGetAngleYZTo(const C3DVector& from, const C3DVector& to)
#include <cstdio>
#include <math.h>

class C3DVector { public:
    float X, Y, Z;
};

static C3DVector V(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }

inline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

float GFGetAngleYZTo(const C3DVector& from, const C3DVector& to)
{
    float dx = from.X - to.X;
    float dy = from.Y - to.Y;
    float dz = to.Z - from.Z;
    float angle = (float)atan2(dz, sqrt(dy * dy + dx * dx)) * 0.15915494f;
    return WrapAngle(angle);
}

static int g_fail = 0;
static void Check(const C3DVector& from, const C3DVector& to, float expected)
{
    float got = GFGetAngleYZTo(from, to);
    if (fabs(got - expected) > 1e-4f || got < 0.0f || got >= 1.0f) {
        std::printf("  got %f expected %f\n", got, expected);
        g_fail = 1;
    }
}

int main()
{
    Check(V(0, 0, 0), V(0, 0, 1), 0.25f);        // straight up
    Check(V(0, 0, 0), V(3, 4, 0), 0.0f);         // level
    Check(V(0, 0, 0), V(3, 4, -5), 0.875f);      // 45 degrees down wraps to 7/8
    Check(V(1, 1, 2), V(1, 1, 1), 0.75f);        // straight down wraps to 3/4
    Check(V(1, 2, 3), V(4, 6, 8), 0.125f);       // 45 degrees up, offset origin
    Check(V(4, 6, 8), V(1, 2, 3), 0.875f);       // reversed: 45 degrees down
    Check(V(-2, 0, 0), V(2, 0, 4), 0.125f);      // distance sign does not matter (sqrt)
    if (g_fail) { std::printf("00754e70_TEST FAIL\n"); return 1; }
    std::printf("00754e70_TEST PASS\n");
    return 0;
}