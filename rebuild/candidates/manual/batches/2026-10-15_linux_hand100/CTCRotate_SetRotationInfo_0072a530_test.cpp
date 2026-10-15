#include <cstdio>
struct C3DVector { float x, y, z; };
long g_GameFramesPerSecond = 15;
struct CTCRotate {
    C3DVector m_axis; float m_speed; long m_unknown1c; long m_framesElapsed; long m_framesTotal;
    void SetRotationInfo(const C3DVector& axis, float speed, float seconds)
    {
        m_axis = axis;
        m_speed = speed;
        m_framesTotal = (long)(seconds * g_GameFramesPerSecond);
    }
};
int main()
{
    CTCRotate r; C3DVector up = { 0.0f, 0.0f, 1.0f };
    r.SetRotationInfo(up, 2.5f, 2.0f);
    if (r.m_axis.z != 1.0f || r.m_axis.x != 0.0f || r.m_speed != 2.5f || r.m_framesTotal != 30) { std::printf("0072a530_TEST FAIL\n"); return 1; }
    r.SetRotationInfo(up, 1.0f, 0.5f);
    if (r.m_framesTotal != 7) { std::printf("0072a530_TEST FAIL\n"); return 1; }
    std::printf("0072a530_TEST PASS\n");
    return 0;
}
