// Behaviour test for 00c1bd40: CFloatPair accessors
#include <cstdio>
struct CFloatPair {
    float m_x; float m_y;
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    void SetX(float x) { m_x = x; }
    void SetY(float y) { m_y = y; }
};
int main()
{
    CFloatPair p; p.SetX(1.5f); p.SetY(-2.0f);
    bool ok = p.GetX() > 1.49f && p.GetX() < 1.51f && p.GetY() < -1.99f && p.GetY() > -2.01f && sizeof(p) == 8;
    p.SetX(0.0f); ok = ok && p.m_x == 0.0f && p.m_y < -1.99f;
    if (!ok) { std::printf("00c1bd40_TEST FAIL\n"); return 1; }
    std::printf("00c1bd40_TEST PASS\n");
    return 0;
}
