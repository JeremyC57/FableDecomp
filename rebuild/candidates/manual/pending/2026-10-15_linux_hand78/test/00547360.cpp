#include <cstdio>
static int g_baseCalls = 0;
struct CChangingStateComponent {
    long m_initialValue;
    long m_baseState;
    void Initialise() { ++g_baseCalls; m_baseState = 0; }
};
struct CSwappingStateComponent : CChangingStateComponent {
    long m_swapValue;
    long m_swapMode;
    void Initialise()
    {
        CChangingStateComponent::Initialise();
        m_swapValue = m_initialValue;
        m_swapMode = 13;
    }
};
int main()
{
    CSwappingStateComponent c;
    c.m_initialValue = 42; c.m_baseState = 9; c.m_swapValue = -1; c.m_swapMode = -1;
    c.Initialise();
    if (g_baseCalls != 1 || c.m_baseState != 0 || c.m_swapValue != 42 || c.m_swapMode != 13) { std::printf("00547360_TEST FAIL\n"); return 1; }
    std::printf("00547360_TEST PASS\n");
    return 0;
}
