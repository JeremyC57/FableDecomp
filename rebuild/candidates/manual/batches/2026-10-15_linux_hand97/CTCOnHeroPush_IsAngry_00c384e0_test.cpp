// Behaviour test for 0x00C384E0: true exactly when the two floats differ.
#include <cstdio>
struct CFloatPair {
    float m_current, m_target;
    long IsAtTarget() const { if (m_current == m_target) return 1; return 0; }
    bool IsAngry() const { return !IsAtTarget(); }
};
int main()
{
    CFloatPair p; p.m_current = 1.0f; p.m_target = 1.0f;
    if (p.IsAngry()) { std::printf("00c384e0_TEST FAIL\n"); return 1; }
    p.m_target = 2.0f;
    if (!p.IsAngry()) { std::printf("00c384e0_TEST FAIL\n"); return 1; }
    std::printf("00c384e0_TEST PASS\n");
    return 0;
}
