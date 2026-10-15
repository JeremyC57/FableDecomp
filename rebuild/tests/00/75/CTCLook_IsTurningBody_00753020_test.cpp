#include <cstdio>
struct CTCLook {
    float m_xy, m_yz; int calls;
    bool GetTurningAnglesAndCheckForTurningBody(float* angleXY, float* angleYZ)
    {
        ++calls; *angleXY = m_xy; *angleYZ = m_yz;
        return m_xy > 1.0f || m_yz > 1.0f;
    }
    bool IsTurningBody() { float angleXY; float angleYZ; return GetTurningAnglesAndCheckForTurningBody(&angleXY, &angleYZ); }
};
int main()
{
    CTCLook l; l.m_xy = 0.5f; l.m_yz = 0.25f; l.calls = 0;
    if (l.IsTurningBody()) { std::printf("00753020_TEST FAIL\n"); return 1; }
    l.m_xy = 2.0f;
    if (!l.IsTurningBody() || l.calls != 2) { std::printf("00753020_TEST FAIL\n"); return 1; }
    std::printf("00753020_TEST PASS\n");
    return 0;
}