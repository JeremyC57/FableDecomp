// Behaviour test for 00c45810: CAmbientCube::UpdateDistances -- keep each apex's smallest (height-raised) listener distance
#include <cstdio>
#include <math.h>

class C3DVector { public:
    float X, Y, Z;
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
static C3DVector Sub(const C3DVector& a, const C3DVector& b) { C3DVector r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
struct SAmbientApex { C3DVector m_offset, m_position; float m_minDistance; };
struct CAmbientCube {
    SAmbientApex m_apexes[8];
    void UpdateDistances(const C3DVector& listener, float height)
    {
        for (int i = 0; i < 8; ++i) {
            C3DVector point = listener;
            if (m_apexes[i].m_position.Z > point.Z) {
                float top = point.Z + height;
                point.Z = top > m_apexes[i].m_position.Z ? m_apexes[i].m_position.Z : top;
            }
            C3DVector delta = Sub(point, m_apexes[i].m_position);
            float distance = delta.GetMagnitude();
            if (distance < m_apexes[i].m_minDistance)
                m_apexes[i].m_minDistance = distance;
        }
    }
};
static bool Near(float a, float b) { return fabs(a - b) < 0.001f; }

int main()
{
    CAmbientCube cube;
    for (int i = 0; i < 8; ++i) {
        C3DVector zero = { 0, 0, 0 };
        C3DVector p = { (float)(i & 1) * 10.0f, (float)((i >> 1) & 1) * 10.0f, (float)((i >> 2) & 1) * 10.0f };
        cube.m_apexes[i].m_offset = zero; cube.m_apexes[i].m_position = p; cube.m_apexes[i].m_minDistance = 1000.0f;
    }
    C3DVector listener = { 0, 0, 0 };
    cube.UpdateDistances(listener, 4.0f);
    int ok = Near(cube.m_apexes[0].m_minDistance, 0.0f);
    ok &= Near(cube.m_apexes[4].m_minDistance, 6.0f);                    // apex 10 up, listener raised by 4
    ok &= Near(cube.m_apexes[1].m_minDistance, 10.0f);                   // same height: not raised
    cube.UpdateDistances(listener, 20.0f);
    ok &= Near(cube.m_apexes[4].m_minDistance, 0.0f);                    // raised no higher than the apex
    C3DVector distant = { 100, 100, 100 };
    cube.UpdateDistances(distant, 0.0f);
    ok &= Near(cube.m_apexes[1].m_minDistance, 10.0f);                   // a farther listener keeps the minimum
    if (!ok) { std::printf("00c45810_TEST FAIL\n"); return 1; }
    std::printf("00c45810_TEST PASS\n");
    return 0;
}