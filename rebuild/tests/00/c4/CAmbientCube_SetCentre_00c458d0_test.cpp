// Behaviour test for 00c458d0: CAmbientCube::SetCentre -- apex position = centre + apex offset
#include <cstdio>

struct C3DVector { float X, Y, Z; };
static C3DVector Add(const C3DVector& a, const C3DVector& b) { C3DVector r = { a.X + b.X, a.Y + b.Y, a.Z + b.Z }; return r; }
struct SAmbientApex { C3DVector m_offset, m_position; float m_minDistance; };
struct CAmbientCube {
    SAmbientApex m_apexes[8];
    void SetCentre(const C3DVector& centre)
    {
        for (int i = 0; i < 8; ++i)
            m_apexes[i].m_position = Add(centre, m_apexes[i].m_offset);
    }
};

int main()
{
    CAmbientCube cube;
    for (int i = 0; i < 8; ++i) {
        C3DVector off = { (float)i, (float)-i, 0.5f };
        cube.m_apexes[i].m_offset = off; cube.m_apexes[i].m_minDistance = 3.0f;
    }
    C3DVector centre = { 10.0f, 20.0f, 30.0f };
    cube.SetCentre(centre);
    int ok = 1;
    for (int i = 0; i < 8; ++i) {
        float dx = cube.m_apexes[i].m_position.X - (10.0f + i), dy = cube.m_apexes[i].m_position.Y - (20.0f - i), dz = cube.m_apexes[i].m_position.Z - 30.5f;
        ok &= dx * dx + dy * dy + dz * dz < 1e-6f && cube.m_apexes[i].m_minDistance == 3.0f;
    }
    if (!ok) { std::printf("00c458d0_TEST FAIL\n"); return 1; }
    std::printf("00c458d0_TEST PASS\n");
    return 0;
}