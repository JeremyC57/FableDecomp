// Behaviour test for 00c34600: CQuadHolder::GetValue -- returns a copy of the held 16-byte value
#include <cstdio>
struct SQuad { float m_x, m_y, m_z, m_w; };
struct CQuadHolder {
    SQuad m_value;
    SQuad GetValue() const { return m_value; }
};
int main()
{
    CQuadHolder h; h.m_value.m_x = 1.0f; h.m_value.m_y = 2.0f; h.m_value.m_z = 3.0f; h.m_value.m_w = 4.0f;
    SQuad q = h.GetValue();
    h.m_value.m_x = 9.0f;
    if (q.m_x != 1.0f || q.m_y != 2.0f || q.m_z != 3.0f || q.m_w != 4.0f) { std::printf("00c34600_TEST FAIL\n"); return 1; }
    std::printf("00c34600_TEST PASS\n");
    return 0;
}