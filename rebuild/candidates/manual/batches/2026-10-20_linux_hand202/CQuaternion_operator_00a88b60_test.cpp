// Behaviour test for CQuaternion::operator* (0x00A88B60): Hamilton products of the unit quaternions (i*j = k,
// j*i = -k, i*i = -1), the identity, and a general product checked against the textbook formula.
#include <cstdio>
#include <cmath>
struct CQuaternion {
    float X, Y, Z, W;
    CQuaternion operator*(const CQuaternion& q) const;
};
CQuaternion CQuaternion::operator*(const CQuaternion& q) const
{
    CQuaternion r;
    r.W = W * q.W - q.X * X - Y * q.Y - Z * q.Z;
    r.X = W * q.X + X * q.W + Y * q.Z - Z * q.Y;
    r.Y = q.Y * W + Y * q.W + Z * q.X - q.Z * X;
    r.Z = q.Z * W + q.Y * X + Z * q.W - Y * q.X;
    return r;
}
static bool Near(const CQuaternion& a, float x, float y, float z, float w)
{
    return std::fabs(a.X - x) < 1e-4f && std::fabs(a.Y - y) < 1e-4f && std::fabs(a.Z - z) < 1e-4f && std::fabs(a.W - w) < 1e-4f;
}
int main()
{
    CQuaternion i = { 1, 0, 0, 0 }, j = { 0, 1, 0, 0 }, one = { 0, 0, 0, 1 };
    CQuaternion a = { 1, 2, 3, 4 }, b = { 5, 6, 7, 8 };
    int bad = 0;
    if (!Near(i * j, 0, 0, 1, 0)) bad |= 1;
    if (!Near(j * i, 0, 0, -1, 0)) bad |= 2;
    if (!Near(i * i, 0, 0, 0, -1)) bad |= 4;
    if (!Near(one * a, 1, 2, 3, 4) || !Near(a * one, 1, 2, 3, 4)) bad |= 8;
    // (a.W*b.W - a.v.b.v, a.W*b.v + b.W*a.v + a.v x b.v)
    if (!Near(a * b, 4*5 + 8*1 + (2*7 - 3*6), 4*6 + 8*2 + (3*5 - 1*7), 4*7 + 8*3 + (1*6 - 2*5), 4*8 - (1*5 + 2*6 + 3*7))) bad |= 16;
    if (bad) { std::printf("00a88b60_TEST FAIL\n"); return 1; }
    std::printf("00a88b60_TEST PASS\n");
    return 0;
}
