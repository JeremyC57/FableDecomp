// Behaviour test for CQuaternion::IsIdentity() @ 0x00A88CB0
#include <cstdio>
#include <math.h>

const float g_QuaternionEpsilon = 0.0001f;
const float g_One = 1.0f;

struct CQuaternion {
    float X, Y, Z, W;
    bool IsIdentity() const
    {
        if (fabs(X) < g_QuaternionEpsilon && fabs(Y) < g_QuaternionEpsilon &&
            fabs(Z) < g_QuaternionEpsilon && fabs(W - g_One) < g_QuaternionEpsilon)
            return true;
        return false;
    }
};

int main()
{
    CQuaternion id = { 0.0f, 0.00005f, -0.00005f, 1.00005f };
    CQuaternion a = { 0.0f, 0.0f, 0.0f, 0.999f };
    CQuaternion b = { 0.0f, 0.0f, 0.001f, 1.0f };
    CQuaternion c = { -0.5f, 0.0f, 0.0f, 1.0f };
    if (!id.IsIdentity() || a.IsIdentity() || b.IsIdentity() || c.IsIdentity()) { std::printf("00a88cb0_TEST FAIL\n"); return 1; }
    std::printf("00a88cb0_TEST PASS\n");
    return 0;
}