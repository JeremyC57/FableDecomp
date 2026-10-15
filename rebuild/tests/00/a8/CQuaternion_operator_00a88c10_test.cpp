// Behaviour test for CQuaternion::operator*=(const CQuaternion&) @ 0x00A88C10 (stub operator* = Hamilton product)
#include <cstdio>

struct CQuaternion {
    float X, Y, Z, W;
    CQuaternion operator*(const CQuaternion& q) const
    {
        CQuaternion r;
        r.X = W * q.X + X * q.W + Y * q.Z - Z * q.Y;
        r.Y = W * q.Y - X * q.Z + Y * q.W + Z * q.X;
        r.Z = W * q.Z + X * q.Y - Y * q.X + Z * q.W;
        r.W = W * q.W - X * q.X - Y * q.Y - Z * q.Z;
        return r;
    }
    CQuaternion& operator*=(const CQuaternion& q)
    {
        *this = *this * q;
        return *this;
    }
};

int main()
{
    CQuaternion i = { 1.0f, 0.0f, 0.0f, 0.0f };
    CQuaternion j = { 0.0f, 1.0f, 0.0f, 0.0f };
    CQuaternion a = i;
    CQuaternion& r = (a *= j);              // i * j = k
    if (&r != &a || a.X != 0.0f || a.Y != 0.0f || a.Z != 1.0f || a.W != 0.0f) { std::printf("00a88c10_TEST FAIL\n"); return 1; }
    CQuaternion one = { 0.0f, 0.0f, 0.0f, 1.0f };
    CQuaternion b = { 0.5f, -1.0f, 2.0f, 3.0f };
    b *= one;
    if (b.X != 0.5f || b.Y != -1.0f || b.Z != 2.0f || b.W != 3.0f) { std::printf("00a88c10_TEST FAIL\n"); return 1; }
    std::printf("00a88c10_TEST PASS\n");
    return 0;
}