// Behaviour test for CQuaternion::Equals(const CQuaternion&) @ 0x00A88C50
#include <cstdio>
#include <math.h>

const float g_QuaternionEpsilon = 0.0001f;

struct CQuaternion {
    float X, Y, Z, W;
    bool Equals(const CQuaternion& quaternion) const
    {
        if (fabs(X - quaternion.X) < g_QuaternionEpsilon && fabs(Y - quaternion.Y) < g_QuaternionEpsilon &&
            fabs(Z - quaternion.Z) < g_QuaternionEpsilon && fabs(W - quaternion.W) < g_QuaternionEpsilon)
            return true;
        return false;
    }
};

int main()
{
    CQuaternion a = { 0.1f, 0.2f, 0.3f, 0.9f };
    CQuaternion b = { 0.10005f, 0.2f, 0.29995f, 0.9f };
    CQuaternion c = { 0.1f, 0.2f, 0.3f, 0.9002f };
    CQuaternion d = { 0.1f, 0.2003f, 0.3f, 0.9f };
    if (!a.Equals(a) || !a.Equals(b) || a.Equals(c) || a.Equals(d)) { std::printf("00a88c50_TEST FAIL\n"); return 1; }
    std::printf("00a88c50_TEST PASS\n");
    return 0;
}