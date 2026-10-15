#include <cstdio>
struct CAnimateThing {
    bool m_queueDone; unsigned char m_markedFinished : 1;
    bool IsAnimationFinished() const { return m_queueDone; }
    bool IsFinished() const { return IsAnimationFinished() || m_markedFinished; }
};
int main()
{
    CAnimateThing a; a.m_queueDone = false; a.m_markedFinished = 0;
    if (a.IsFinished()) { std::printf("00661720_TEST FAIL\n"); return 1; }
    a.m_markedFinished = 1; if (!a.IsFinished()) { std::printf("00661720_TEST FAIL\n"); return 1; }
    a.m_markedFinished = 0; a.m_queueDone = true; if (!a.IsFinished()) { std::printf("00661720_TEST FAIL\n"); return 1; }
    std::printf("00661720_TEST PASS\n");
    return 0;
}
