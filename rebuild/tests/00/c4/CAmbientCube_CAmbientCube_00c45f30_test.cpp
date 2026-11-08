// Behaviour test for CAmbientCube::CAmbientCube(float size) (0x00C45F30): the eight apex offsets are the unit-cube corners
// (X, Y, Z bit order) scaled by size, the positions equal the offsets (the cube centred on the origin), the minimum
// distances are 0 and the ramped level starts at 0 with the angle flag clear.
#include <cstdio>
#include <cstring>
#include <new>
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static V3 Mul(const V3& v, float s) { return V3(v.X * s, v.Y * s, v.Z * s); }
static V3 Add(const V3& a, const V3& b) { return V3(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
struct CRampedParam {
    float m_value, m_target, m_rest[10]; bool m_bAngle;
    void SetImmediate(float v) { m_value = m_target = v; for (int i = 0; i < 10; ++i) m_rest[i] = 0.0f; }
};
struct SAmbientApex { V3 m_offset, m_position; float m_minDistance; SAmbientApex() : m_offset(0, 0, 0), m_position(0, 0, 0) {} };
struct CAmbientCube {
    SAmbientApex m_apexes[8]; CRampedParam m_level;
    void SetCentre(const V3& c) { for (int i = 0; i < 8; ++i) m_apexes[i].m_position = Add(c, m_apexes[i].m_offset); }
    CAmbientCube(float size)
    {
        m_level.m_bAngle = false; m_level.SetImmediate(0.0f);
        for (int i = 0; i < 8; ++i) m_apexes[i].m_minDistance = 0.0f;
        m_apexes[0].m_offset = Mul(V3(0, 0, 0), size); m_apexes[1].m_offset = Mul(V3(0, 0, 1), size);
        m_apexes[2].m_offset = Mul(V3(0, 1, 0), size); m_apexes[3].m_offset = Mul(V3(0, 1, 1), size);
        m_apexes[4].m_offset = Mul(V3(1, 0, 0), size); m_apexes[5].m_offset = Mul(V3(1, 0, 1), size);
        m_apexes[6].m_offset = Mul(V3(1, 1, 0), size); m_apexes[7].m_offset = Mul(V3(1, 1, 1), size);
        SetCentre(V3(0, 0, 0));
    }
};
int main()
{
    static unsigned char raw[sizeof(CAmbientCube)]; std::memset(raw, 0x5a, sizeof raw);
    CAmbientCube* c = new (raw) CAmbientCube(4.0f);
    bool ok = !c->m_level.m_bAngle && c->m_level.m_value == 0.0f && c->m_level.m_target == 0.0f;
    for (int i = 0; i < 8; ++i) {
        const SAmbientApex& a = c->m_apexes[i];
        float x = (i & 4) ? 4.0f : 0.0f, y = (i & 2) ? 4.0f : 0.0f, z = (i & 1) ? 4.0f : 0.0f;
        ok = ok && a.m_offset.X == x && a.m_offset.Y == y && a.m_offset.Z == z;
        ok = ok && a.m_position.X == x && a.m_position.Y == y && a.m_position.Z == z && a.m_minDistance == 0.0f;
    }
    if (!ok) { std::printf("00c45f30_TEST FAIL\n"); return 1; }
    std::printf("00c45f30_TEST PASS\n");
    return 0;
}