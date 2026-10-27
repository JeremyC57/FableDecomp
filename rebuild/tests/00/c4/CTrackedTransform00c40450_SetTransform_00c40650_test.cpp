// Behaviour test for 0x00C40650 CTrackedTransform_00c40450::SetTransform
#include <cstdio>
struct V { float X, Y, Z; };
struct T {
    V m_position, m_direction, m_up, m_forward; unsigned int m_dirtyFlags; int m_orientCalls;
    void SetOrientationVectors(const V& up, const V& forward) { m_up = up; m_forward = forward; ++m_orientCalls; }
    void SetTransform(const V* t)    // Position, Direction, Up, Forward
    {
        if (m_position.X != t[0].X || m_position.Y != t[0].Y || m_position.Z != t[0].Z) { m_position = t[0]; m_dirtyFlags |= 0x8000; }
        if (m_direction.X != t[1].X || m_direction.Y != t[1].Y || m_direction.Z != t[1].Z) { m_direction = t[1]; m_dirtyFlags |= 0x10000; }
        V forward = t[3]; V up = t[2];
        SetOrientationVectors(up, forward);
    }
};
int main()
{
    T o; V z = { 0, 0, 0 }; o.m_position = z; o.m_direction = z; o.m_up = z; o.m_forward = z; o.m_dirtyFlags = 0; o.m_orientCalls = 0;
    V t[4] = { { 0, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 }, { 1, 0, 0 } };
    o.SetTransform(t);
    if (o.m_dirtyFlags != 0x10000 || o.m_direction.Z != 1 || o.m_up.Y != 1 || o.m_forward.X != 1 || o.m_orientCalls != 1) {
        std::printf("00c40650_TEST FAIL\n"); return 1;
    }
    t[0].Y = 5; o.m_dirtyFlags = 0; o.SetTransform(t);
    if (o.m_dirtyFlags != 0x8000 || o.m_position.Y != 5 || o.m_orientCalls != 2) { std::printf("00c40650_TEST FAIL\n"); return 1; }
    std::printf("00c40650_TEST PASS\n");
    return 0;
}