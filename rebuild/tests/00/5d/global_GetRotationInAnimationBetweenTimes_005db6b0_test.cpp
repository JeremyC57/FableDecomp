// Behaviour test for GetRotationInAnimationBetweenTimes (0x005DB6B0): an animation without root movement gives 0; a
// root turn of a quarter revolution to the left gives +0.25 and to the right -0.25 (the half-way movement signs it);
// a half turn gives 0.5; a negative end time is replaced by the animation's duration -- through a copy of the body
// over stand-in stats returning rotation matrices about Z.
#include <cmath>
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
static float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct CMatrix3x4 {
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
};
static C3DVector operator*(const C3DVector& v, const CMatrix3x4& m)
{
    return C3DVector(v.Z * m.E31 + v.Y * m.E21 + v.X * m.E11,
                     v.Z * m.E32 + v.Y * m.E22 + v.X * m.E12,
                     v.Z * m.E33 + v.Y * m.E23 + v.X * m.E13);
}

struct Stats {
    bool hasMovement;
    float turnPerSecond;                                  // revolutions per second (signed)
    float lastEndTime;
    bool HasMovement() const { return hasMovement; }
    CMatrix3x4 GetMovementBetweenTimes(float startTime, float endTime)
    {
        lastEndTime = endTime;
        float angle = (endTime - startTime) * turnPerSecond * 6.2831853f;
        float c = (float)std::cos(angle), s = (float)std::sin(angle);
        CMatrix3x4 m = { c, -s, 0.0f, s, c, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f };
        return m;
    }
};

static void GetRotationInAnimationBetweenTimes(Stats* stats, float duration, float& rotation, float startTime,
                                               float endTime)
{
    if (endTime < -0.0001f)
        endTime = duration;
    if (!stats->HasMovement()) {
        rotation = 0.0f;
        return;
    }
    CMatrix3x4 endTransform = stats->GetMovementBetweenTimes(startTime, endTime);
    C3DVector endForward = C3DVector(1.0f, 0.0f, 0.0f) * endTransform;
    CMatrix3x4 midTransform = stats->GetMovementBetweenTimes(startTime, (startTime + endTime) * 0.5f);
    C3DVector midForward = C3DVector(1.0f, 0.0f, 0.0f) * midTransform;
    float cosAngle = DotProduct(endForward, C3DVector(1.0f, 0.0f, 0.0f));
    if (cosAngle < -1.0f)
        cosAngle = -1.0f;
    else if (cosAngle > 1.0f)
        cosAngle = 1.0f;
    rotation = (float)std::acos(cosAngle) * 0.15915494f;
    float sign = 1.0f;
    if (DotProduct(midForward, C3DVector(0.0f, -1.0f, 0.0f)) < 0.0f)
        sign = -1.0f;
    rotation *= sign;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 0.001f; }

int main()
{
    Stats still = { false, 0.0f, 0.0f };
    float rotation = 9.0f;
    GetRotationInAnimationBetweenTimes(&still, 2.0f, rotation, 0.0f, 1.0f);
    if (rotation != 0.0f) {
        std::printf("005db6b0_TEST FAIL\n");
        return 1;
    }
    Stats left = { true, 0.25f, 0.0f };
    GetRotationInAnimationBetweenTimes(&left, 2.0f, rotation, 0.0f, 1.0f);
    float leftTurn = rotation;
    Stats right = { true, -0.25f, 0.0f };
    GetRotationInAnimationBetweenTimes(&right, 2.0f, rotation, 0.0f, 1.0f);
    float rightTurn = rotation;
    if (!Near(leftTurn, 0.25f) || !Near(rightTurn, -0.25f)) {
        std::printf("005db6b0_TEST FAIL\n");
        return 1;
    }
    Stats half = { true, 0.25f, 0.0f };
    GetRotationInAnimationBetweenTimes(&half, 2.0f, rotation, 0.0f, -1.0f);
    if (half.lastEndTime != 1.0f || !Near(rotation, 0.5f)) {      // the last call is the half-way one: (0 + 2) / 2
        std::printf("005db6b0_TEST FAIL\n");
        return 1;
    }
    std::printf("005db6b0_TEST PASS\n");
    return 0;
}