// Behaviour test for CLibpkObject1D8::NormaliseCopy (0x00C418F0): zero input -> (0,0,1) and 0; else copy, normalise, return length.
#include <cstdio>
#include <cmath>

struct CPKVector3 {
    float x, y, z;
    float Normalise()
    {
        if (x == 0.0f && y == 0.0f && z == 0.0f) return 0.0f;
        float length = (float)std::sqrt(z * z + y * y + x * x);
        if (1.0f != length) { float s = 1.0f / length; x *= s; y *= s; z *= s; }
        return length;
    }
    float GetLength() const { return (float)std::sqrt(y * y + z * z + x * x); }
};

static float NormaliseCopy(CPKVector3* pOut, const CPKVector3* pIn)
{
    if (pIn->x == 0.0f && pIn->y == 0.0f && pIn->z == 0.0f) {
        pOut->x = 0.0f; pOut->y = 0.0f; pOut->z = 1.0f;
        return 0.0f;
    }
    *pOut = *pIn;
    double length = pIn->GetLength();
    float result = (float)length;
    pOut->Normalise();
    return result;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    CPKVector3 in = {0.0f, 0.0f, 0.0f}, out = {5.0f, 5.0f, 5.0f};
    if (NormaliseCopy(&out, &in) != 0.0f || out.x != 0.0f || out.y != 0.0f || out.z != 1.0f) { std::printf("00c418f0_TEST FAIL\n"); return 1; }
    CPKVector3 in2 = {3.0f, 0.0f, 4.0f};
    float len = NormaliseCopy(&out, &in2);
    if (!Near(len, 5.0f) || !Near(out.x, 0.6f) || !Near(out.y, 0.0f) || !Near(out.z, 0.8f) || in2.x != 3.0f) { std::printf("00c418f0_TEST FAIL\n"); return 1; }
    CPKVector3 in3 = {0.0f, 2.0f, 0.0f};
    len = NormaliseCopy(&out, &in3);
    if (!Near(len, 2.0f) || !Near(out.y, 1.0f)) { std::printf("00c418f0_TEST FAIL\n"); return 1; }
    std::printf("00c418f0_TEST PASS\n");
    return 0;
}