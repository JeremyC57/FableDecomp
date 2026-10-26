#include <cstdio>
class C3DVector { public: float X, Y, Z; void Set(float x, float y, float z) { X = x; Y = y; Z = z; } };
struct CVtable_012B5A80 {
    void* m_pPrev; void* m_pNext;
    C3DVector m_position, m_velocity, m_axisX, m_axisY, m_axisZ;
    unsigned long m_flags;
    bool Init()
    {
        m_flags &= 0x7fffffff;
        m_position.X = m_position.Y = m_position.Z = 0.0f;
        m_velocity.X = m_velocity.Y = m_velocity.Z = 0.0f;
        m_axisX.Set(0.0f, 0.0f, 1.0f); m_axisY.Set(0.0f, 1.0f, 0.0f); m_axisZ.Set(1.0f, 0.0f, 0.0f);
        return true;
    }
};
int main()
{
    CVtable_012B5A80 a; a.m_flags = 0x80000005; a.m_position.Set(1, 2, 3); a.m_velocity.Set(4, 5, 6); a.m_axisX.Set(7, 7, 7); a.m_axisY.Set(8, 8, 8); a.m_axisZ.Set(9, 9, 9);
    if (!a.Init() || a.m_flags != 5 || a.m_position.X != 0 || a.m_position.Z != 0 || a.m_velocity.Y != 0 || a.m_axisX.Z != 1.0f || a.m_axisX.X != 0
        || a.m_axisY.Y != 1.0f || a.m_axisY.Z != 0 || a.m_axisZ.X != 1.0f || a.m_axisZ.Y != 0) { std::printf("00c40410_TEST FAIL\n"); return 1; }
    std::printf("00c40410_TEST PASS\n");
    return 0;
}